/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>
#include "Image.hpp"
#include "RGB.hpp"
#include "utils/mainMethods.hpp"
#include <libconfig.h++>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

namespace ray {
    class ServerException : public std::exception {
        public:
            ServerException(const std::string& message) : _message(message) {}
            const char* what() const noexcept override { return _message.c_str(); }
        private:
            std::string _message;
    };

    class Server {
        public:
            Server(int port, std::string cfgfile) : sockfd(-1), port(port), queue_size(30)
            {
                init_server();
                create_server_socket();
                bind_server_socket();
                start_server();
                cfg = openFile(cfgfile);
            }

            ~Server()
            {
                if (sockfd != -1) {
#ifdef _WIN32
                    closesocket(sockfd);
#else
                    close(sockfd);
#endif
                }

                for (int client_sockfd : client_sockets) {
                    close_client(client_sockfd);
                }
            }

            void accept_connections(int nb_clients)
            {
                for (int i = 0; i < nb_clients; ++i) {
                    struct sockaddr_in client_addr;
#ifdef _WIN32
                    int client_addr_len = sizeof(client_addr);
#else
                    socklen_t client_addr_len = sizeof(client_addr);
#endif
                    int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
                    if (client_sockfd < 0) {
                        throw ServerException("Failed to accept connection");
                    }
                    std::cout << "Client " << i << " connected" << std::endl;
                    client_sockets.push_back(client_sockfd);
                    send_data(client_sockfd, {"CFG", cfg});

                    std::pair<std::string, std::string> check = receive_data(client_sockfd);
                    if (check.first != "CFG" || check.second != "OK") {
                        throw ServerException("Client not ready");
                    }
                }
            }

            std::string openFile(const std::string& filename)
            {
                std::ifstream file(filename);
                if (!file.is_open()) {
                    throw ServerException("Failed to open file");
                }
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                return content;
            }

            void close_client(int client_sockfd)
            {
                client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), client_sockfd), client_sockets.end());
#ifdef _WIN32
                closesocket(client_sockfd);
#else
                close(client_sockfd);
#endif
            }

            void send_data(int client_sockfd, const std::pair<std::string, std::string>& data)
            {
                std::string to_send = data.first + ":" + data.second + "\r\n";
                if (send(client_sockfd, to_send.c_str(), to_send.size(), 0) < 0) {
                    throw ServerException("Failed to send data");
                }
                // std::cout << "Sent " << to_send << std::endl;
            }

            std::pair<std::string, std::string> receive_data(int client_sockfd)
            {
                char buffer[1024] = {0};
                std::string data;
                while (true) {
                    memset(buffer, 0, sizeof(buffer));
                    int n = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
                    if (n < 0) {
                        throw ServerException("Failed to receive data");
                    }
                    data += buffer;
                    if (data.size() >= 2 && data.substr(data.size() - 2) == "\r\n") {
                        break;
                    }
                }
                data = data.substr(0, data.size() - 2);
                size_t pos = data.find(':');
                if (pos == std::string::npos) {
                    throw ServerException("Invalid data format");
                }
                if (data.size() == 0) {
                    return {"", ""};
                }
                return {data.substr(0, pos), data.substr(pos + 1)};
            }

            Image orchestrate_rendering(unsigned int width, unsigned int height,
                const std::shared_ptr<ray::IScene>& scene,
                const std::shared_ptr<ray::ICamera>& cam)
            {
                (void)scene;
                (void)cam;

                std::thread monitor_thread(&Server::monitor_clients, this);
                monitor_thread.detach();

                std::vector<Image> images(client_sockets.size(), Image());
                std::deque<std::pair<int, int>> bands;
                std::atomic<int> processed_bands(0);
                int total_bands = width * height;

                for (unsigned int i = 0; i < width; i++) {
                    for (unsigned int j = 0; j < height; j++) {
                        bands.push_back({i, j});
                    }
                }

                std::mutex bands_mutex;
                std::vector<std::thread> send_threads;
                std::vector<std::thread> receive_threads;
                std::vector<std::shared_ptr<std::atomic_bool>> state;
                for (size_t i = 0; i < client_sockets.size(); ++i) {
                    client_data[i] = "";
                    state.push_back(std::make_shared<std::atomic_bool>(true));
                }

                int client_index = 0;
                for (int client_sockfd : client_sockets) {
                    int current_client_index = client_index;
                    int current_client_sockfd = client_sockfd;
                    send_threads.push_back(std::thread([&, current_client_index, current_client_sockfd]() {
                        while (true) {
                            if (*state[current_client_index]) {
                                bands_mutex.lock();
                                if (bands.empty()) {
                                    bands_mutex.unlock();
                                    break;
                                }

                                std::string band_data;
                                int current_x = bands.front().first;

                                while (!bands.empty() && bands.front().first == current_x) {
                                    std::pair<int, int> band = bands.front();
                                    bands.pop_front();
                                    band_data += std::to_string(band.first) + "," + std::to_string(band.second) + ";";
                                }

                                bands_mutex.unlock();

                                send_data(current_client_sockfd, {"RENDER", band_data});
                                *state[current_client_index] = false;
                            }
                        }
                    }));

                    receive_threads.push_back(std::thread([&, current_client_index, current_client_sockfd]() {
                        while (true) {
                            std::string data = client_data[current_client_sockfd];
                            if (!data.empty()) {
                                std::cerr << "Received \"" << data << "\"" << std::endl;
                                std::vector<std::string> datas = RayTracerUtils::renderTokenSpliter(data, ';');
                                for (const auto& d : datas) {
                                    std::vector<std::string> args = RayTracerUtils::renderTokenSpliter(d, ':');
                                    std::vector<std::string> coords = RayTracerUtils::renderTokenSpliter(args[0], ',');
                                    std::vector<std::string> colors = RayTracerUtils::renderTokenSpliter(args[1], ',');
                                    int x = std::stoi(coords[0]);
                                    int y = std::stoi(coords[1]);
                                    RGB color;
                                    color.R = std::stoi(colors[0]);
                                    color.G = std::stoi(colors[1]);
                                    color.B = std::stoi(colors[2]);

                                    images[current_client_index].addPixel(Math::Vector2D{static_cast<double>(x), static_cast<double>(y)}, color);
                                    processed_bands++;
                                }
                                *state[current_client_index] = true;
                                client_data[current_client_sockfd] = "";
                            }
                            std::cout << "Processed " << processed_bands << " bands vs total bands to process : " << total_bands << std::endl;
                            if (processed_bands >= total_bands) {
                                break;
                            }
                        }
                    }));

                    client_index++;
                }

                for (auto& t : send_threads) {
                    t.join();
                }

                for (auto& t : receive_threads) {
                    t.join();
                }

                Image mergedImage;
                for (const auto& img : images) {
                    for (const auto& pixel : img.getMap()) {
                        mergedImage.addPixel(pixel.first, pixel.second);
                    }
                }

                return mergedImage;
            }

        private:
            void init_server()
            {
                addr.sin_port = htons(port);
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = INADDR_ANY;
            }

            void create_server_socket()
            {
#ifdef _WIN32
                WSADATA wsa;
                if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
                    throw ServerException("Failed to initialise winsock");
                }
#endif
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd == -1) {
                    throw ServerException("Socket creation failed");
                }
            }

            void bind_server_socket()
            {
                int e = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
                if (e < 0) {
                    throw ServerException("Bind failed");
                }
            }

            void start_server()
            {
                if (listen(sockfd, queue_size) < 0) {
                    throw ServerException("Listen failed");
                }
            }

            void monitor_clients()
            {
                fd_set read_fds;
                int max_fd = -1;

                while (true) {
                    FD_ZERO(&read_fds);

                    for (int client_sockfd : client_sockets) {
                        FD_SET(client_sockfd, &read_fds);
                        if (client_sockfd > max_fd) {
                            max_fd = client_sockfd;
                        }
                    }

                    int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

                    if (activity < 0) {
                        throw ServerException("Select failed");
                    }

                    if (activity == 0) {
                        continue;
                    }

                    for (int client_sockfd : client_sockets) {
                        if (FD_ISSET(client_sockfd, &read_fds)) {
                            std::pair<std::string, std::string> data = receive_data(client_sockfd);
                            if (data.first == "RENDERED") {
                                client_data[client_sockfd] += data.second;
                            }
                        }
                    }
                }
            }

            int sockfd;
            int port;
            int queue_size;
            struct sockaddr_in addr;
            std::vector<int> client_sockets;
            std::map<int, std::string> client_data;
            std::mutex data_mutex;
            std::string cfg;
    };
}

#endif /* !SERVER_HPP_ */
