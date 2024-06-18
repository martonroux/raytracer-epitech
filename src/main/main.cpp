/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** main
*/

#include <iostream>

#include "Image.hpp"
#include "parsing.hpp"
#include "../cameras/base/Camera.hpp"
#include "Renderer.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "utils/mainMethods.hpp"
#include "pixelization/Pixelize.hpp"
#include "anti_aliasing/Aliasing.hpp"
#include "TSQueue.hpp"

std::pair<std::pair<int, int>, std::pair<int, int>> getImageIterators(image_data_t imgData)
{
    auto biggest = static_cast<unsigned int>(std::max(imgData.width, imgData.height));
    unsigned int startJ = biggest == imgData.height ? 0 : static_cast<unsigned int>(static_cast<float>(biggest - imgData.height) / 2.f);
    unsigned int startI = biggest == imgData.width ? 0 : static_cast<unsigned int>(static_cast<float>(biggest - imgData.width) / 2.f);

    return {{startI, biggest - startI}, {startJ, biggest - startJ}};
}

Image mergeImages(const std::vector<Image>& images)
{
    Image mergedImage;

    for (const auto& img : images) {
        for (const auto& pixel : img.getMap()) {
            mergedImage.addPixel(pixel.first, pixel.second);
        }
    }
    return mergedImage;
}

Image render(Image img,
    unsigned int biggest,
    unsigned int line,
    unsigned int startJ,
    unsigned int endJ,
    const std::shared_ptr<ray::IScene>& scene,
    const std::shared_ptr<ray::ICamera>& cam)
{
    RGB backgroundColor = scene->getBackgroundColor();

    for (unsigned int j = startJ; j < endJ; j++) {
        double u = double(line) / biggest;
        double v = double(j) / biggest;
        RGB color = ray::RayTracerUtils::renderPixel(scene, cam, u, v, backgroundColor);
        img.addPixel(Math::Vector2D{static_cast<double>(line), static_cast<double>(j - startJ)}, color);
    }
    return img;
}

void handleSingleFile(const char *filename)
{
    unsigned int numThreads = std::thread::hardware_concurrency();
    // unsigned int numThreads = 1; // TODO: TEMP
    std::vector<std::thread> threads(numThreads);
    std::vector<Image> images(numThreads);
    ray::NodeBuilder builder(filename);
    const auto& nodes = builder.getRootNodes();
    std::shared_ptr<ray::IScene> scene;
    std::shared_ptr<ray::ICamera> camera;
    TSQueue<unsigned int> queue;
    image_data_t imageData = builder.getImageData();
    auto biggest = static_cast<unsigned int>(std::max(imageData.width, imageData.height));
    auto [iterI, iterJ] = getImageIterators(imageData);

    if (nodes.empty())
        throw ray::CoreException("No root nodes found in the scene file.");
    scene = std::dynamic_pointer_cast<ray::IScene>(ray::RayTracerUtils::getScene(nodes));
    scene->initValues();
    camera = ray::RayTracerUtils::getCamera(scene);

    for (unsigned int i = iterI.first; i < imageData.width - iterI.first; i++)
        queue.push(i);
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread([&, i]() {
            while (queue.size()) {
                unsigned int line = queue.pop();
                std::cout << "Rendering line " << line - iterI.first + 1 << "/" << iterI.second;
                std::cout << " with thread #" << i + 1 << std::endl;
                images[i] = render(images[i], biggest, line, iterJ.first, iterJ.second, scene, camera);
            }
        });
    }

    for (auto& t : threads)
        t.join();
    Image img = mergeImages(images);
    ray::Renderer renderer;
    std::cout << imageData.antiAliasing << std::endl;
    img = applyAntiAliasing(img, &imageData, imageData.antiAliasing);
    std::cout << imageData.height << std::endl;
    std::cout << imageData.width << std::endl;
    img = applyPalletFilter(img, imageData);
    img = applyFilter(img, imageData);
    renderer.renderPpmImage(img, imageData.filename);
    renderer.renderSfmlImage(img);
}

void handleServer(const char *filename, int port, int nb_clients)
{
    ray::Server server(port, filename);
    server.accept_connections(nb_clients);

    ray::NodeBuilder builder(filename);
    const auto& nodes = builder.getRootNodes();
    image_data_t imageData = builder.getImageData();

    if (nodes.empty()) {
        throw ray::CoreException("No root nodes found in the scene file.");
    }

    std::shared_ptr<ray::IScene> scene = std::dynamic_pointer_cast<ray::IScene>(ray::RayTracerUtils::getScene(nodes));
    scene->initValues();
    std::shared_ptr<ray::ICamera> camera = ray::RayTracerUtils::getCamera(scene);
    Image img = server.orchestrate_rendering(imageData.width, imageData.height, scene, camera);
    ray::Renderer renderer;
    renderer.renderPpmImage(img, imageData.filename);
    renderer.renderSfmlImage(img);
}

void handleClient(const std::string& ip, int port)
{
    ray::Client client(ip, port);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Invalid number of arguments." << std::endl;
        std::cerr << "USAGE: ./raytracer <SCENE_FILE> | <SCENE_FILE> -s <port> <nb clients> | -c <ip> <port>" << std::endl;
        return 84;
    }

    try {
        if (argc == 2) {
            handleSingleFile(argv[1]);
        } else if (argc == 5 && std::string(argv[2]) == "-s") {
            int port = std::stoi(argv[3]);
            int nb_clients = std::stoi(argv[4]);
            handleServer(argv[1], port, nb_clients);
        } else if (argc == 4 && std::string(argv[1]) == "-c") {
            std::string ip = argv[2];
            int port = std::stoi(argv[3]);
            handleClient(ip, port);
        } else {
            std::cerr << "Invalid arguments. Please check the usage." << std::endl;
            return 84;
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}
