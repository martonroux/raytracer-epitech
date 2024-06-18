/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <deque>
#include <thread>
#include <mutex>
#include <cstring>
#include <future>
#include "utils/mainMethods.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

namespace ray {
    class ClientException : public std::exception {
        public:
            ClientException(const std::string& message) : _message(message) {}
            const char* what() const noexcept override { return _message.c_str(); }
        private:
            std::string _message;
    };

    class Client {
        public:
            Client(const std::string& ip, int port) : sockfd(-1), port(port), ip(ip)
            {
                init_client();
                create_client_socket();
                connect_to_server();
                monitor(cfg);
            }

            ~Client()
            {
                if (sockfd != -1) {
#ifdef _WIN32
                    closesocket(sockfd);
#else
                    close(sockfd);
#endif
                }
            }

            std::pair<std::string, std::string> get_next_data()
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                if (data_queue.empty()) {
                    return {"", ""};
                }
                std::pair<std::string, std::string> data = data_queue.front();
                data_queue.pop_front();
                return data;
            }

            void send_data(const std::pair<std::string, std::string>& data)
            {
                std::string to_send = data.first + ":" + data.second + "\r\n";
                if (send(sockfd, to_send.c_str(), to_send.size(), 0) < 0) {
                    throw ClientException("Failed to send data");
                }
            }

            std::pair<std::string, std::string> receive_data()
            {
                char buffer[1024] = {0};
                std::string data;
                while (true) {
                    memset(buffer, 0, sizeof(buffer));
                    int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
                    if (n <= 0) {
#ifdef _WIN32
                        closesocket(sockfd);
#else
                        close(sockfd);
#endif
                        throw ClientException("Failed to receive data or the Server disconnected");
                    }
                    data += buffer;
                    if (data.size() >= 2 && data.substr(data.size() - 2) == "\r\n") {
                        break;
                    }
                    // std::cout << "Received partial data" << std::endl;
                }
                // std::cout << "Received data: \"" << data << "\"" << std::endl;
                size_t pos = data.find(':');
                if (pos == std::string::npos) {
                    throw ClientException("Invalid data format");
                }
                std::string firstPart = data.substr(0, pos);
                std::string secondPart = data.substr(pos + 1, data.size() - pos - 3);

                size_t index = 0;
                std::string from = "\r\n";
                std::string to = ";";
                while (true) {
                    index = secondPart.find(from, index);
                    if (index == std::string::npos) break;

                    secondPart.replace(index, from.length(), to);

                    index += to.length();
                }

                std::string toErase = "RENDER:";
                pos = std::string::npos;
                while ((pos = secondPart.find(toErase)) != std::string::npos) {
                    secondPart.erase(pos, toErase.length());
                }
                while ((pos = secondPart.find(";;")) != std::string::npos) {
                    secondPart.erase(pos, 1);
                }
                return {firstPart, secondPart};
            }

            void render(std::pair<std::string, std::string> data, const std::shared_ptr<ray::IScene>& scene, const std::shared_ptr<ray::ICamera>& camera, unsigned int imageWidth, unsigned int imageHeight)
            {
                RGB backgroundColor = scene->getBackgroundColor();
                // std::cout << "New command : \"" << data.first << "\" \""<< data.second << "\"" << std::endl;
                if (data.first == "RENDER") {
                    // std::cout << "Rendering" << std::endl;
                    std::string coordinates = data.second;
                    std::string response = "";
                    std::vector<std::string> coords = RayTracerUtils::renderTokenSpliter(coordinates, ';');
                    unsigned int numThreads = std::thread::hardware_concurrency();

                    std::vector<std::future<std::string>> futures;

                    size_t coordsPerThread = coords.size() / numThreads;

                    for (unsigned int i = 0; i < numThreads; ++i) {
                        size_t start = i * coordsPerThread;
                        size_t end = (i == numThreads - 1) ? coords.size() : (start + coordsPerThread);

                        futures.push_back(std::async(std::launch::async, [&, start, end] {
                            std::string localResponse;

                            for (size_t j = start; j < end; ++j) {
                                const std::string& coord = coords[j];
                                std::vector<std::string> xy = RayTracerUtils::renderTokenSpliter(coord, ',');
                                if (xy.size() < 2) {
                                    std::cerr << "Invalid coordinate: " << coord << std::endl;
                                    continue;
                                }
                                int x = std::stoi(xy[0]);
                                int y = std::stoi(xy[1]);
                                double u = static_cast<double>(x) / imageWidth;
                                double v = static_cast<double>(y) / imageHeight;
                                RGB color = RayTracerUtils::renderPixel(scene, camera, u, v, backgroundColor);
                                localResponse += std::to_string(x) + "," + std::to_string(y) + ":" + std::to_string(color.R) + "," + std::to_string(color.G) + "," + std::to_string(color.B) + ";";
                            }

                            return localResponse;
                        }));
                    }

                    for (auto& future : futures) {
                        response += future.get();
                    }
                    send_data({"RENDERED", response});
                    // std::cout << "Sent RENDERED:" << response << std::endl;
                }
            }

            void monitor(std::pair<std::string, std::string> cfg)
            {
                NodeBuilder builder(cfg.second, true);
                const auto& nodes = builder.getRootNodes();
                if (nodes.empty())
                    throw CoreException("No root nodes found in the scene file.");
                image_data_t imageData = builder.getImageData();
                std::shared_ptr<ray::IScene> scene = std::dynamic_pointer_cast<IScene>(RayTracerUtils::getScene(nodes));
                scene->initValues();
                std::shared_ptr<ray::ICamera> camera = RayTracerUtils::getCamera(scene);

                while (true) {
                    fd_set read_fds;
                    FD_ZERO(&read_fds);
                    FD_SET(sockfd, &read_fds);

                    int activity = select(sockfd + 1, &read_fds, NULL, NULL, NULL);

                    if (activity < 0) {
                        throw ClientException("Error in select");
                    }

                    if (FD_ISSET(sockfd, &read_fds)) {
                        // std::cout << "Data received" << std::endl;
                        std::pair<std::string, std::string> data = receive_data();
                        if (data.first == "RENDER")
                            render(data, scene, camera, imageData.width, imageData.height);
                    }
                }
            }

        private:
            void init_client()
            {
                addr.sin_port = htons(port);
                addr.sin_family = AF_INET;
#ifdef _WIN32
                struct sockaddr_in sa;
                INT sa_len = sizeof(sa);
                std::wstring wip(ip.begin(), ip.end());
                if (WSAStringToAddress((LPWSTR)wip.c_str(), AF_INET, NULL, (struct sockaddr*)&sa, &sa_len) == SOCKET_ERROR) {
                    throw ClientException("Invalid IP address");
                }
                addr.sin_addr = sa.sin_addr;
#else
                if (inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr)) <= 0) {
                    throw ClientException("Invalid IP address");
                }
#endif
            }

            void create_client_socket()
            {
#ifdef _WIN32
                WSADATA wsa;
                if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
                    throw ClientException("Failed to initialise winsock");
                }
#endif
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd == -1) {
                    throw ClientException("Socket creation failed");
                }
            }

            void connect_to_server()
            {
                if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                    throw ClientException("Connection to server failed");
                }

                char buffer[1024];
                std::string data;

                while (true) {
                    memset(buffer, 0, sizeof(buffer));
                    ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);

                    if (n < 0) {
                        throw ClientException("Failed to receive data");
                    } else if (n == 0) {
                        break;
                    }

                    data += buffer;

                    if (data.find("\r\n") != std::string::npos) {
                        break;
                    }
                }

                if (data.substr(0, 3) == "CFG") {
                    cfg = { "CFG", data.substr(4) };
                    std::cout << "Received CFG" << std::endl;
                    send_data({"CFG", "OK"});
                }
            }

            int sockfd;
            int port;
            std::string ip;
            struct sockaddr_in addr;
            std::deque<std::pair<std::string, std::string>> data_queue;
            std::pair<std::string, std::string> cfg;
            std::mutex data_mutex;
            std::thread monitor_thread;
    };
}

#endif /* !CLIENT_HPP_ */
