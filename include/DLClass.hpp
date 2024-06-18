/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-raytracer-robin.glaude
** File description:
** DLClass
*/

#ifndef DLCLASS_HPP_
#define DLCLASS_HPP_

#include <iostream>
#include <string>
#include <exception>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace ray {
    class DLException : public std::exception {
    public:
        DLException(const std::string& message) : _message(message) {}
        const char* what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
    };

    class DynamicPlugin {
        public:
            DynamicPlugin() : handle(nullptr) {}

            ~DynamicPlugin()
            {
                close();
            }

            void open(const std::string& path)
            {
#ifdef _WIN32
                handle = LoadLibraryA(path.c_str());
                if (!handle) {
                    throw ray::DLException("Failed to load plugin: " + std::to_string(GetLastError()));
                }
#else
                handle = dlopen(path.c_str(), RTLD_LAZY);
                if (!handle) {
                    throw ray::DLException("Failed to load plugin: " + std::string(dlerror()));
                }
#endif
            }

            void close() {
                if (handle) {
#ifdef _WIN32
                    FreeLibrary((HMODULE)handle);
#else
                    dlclose(handle);
#endif
                    handle = nullptr;
                }
            }

            template<typename T>
            T getSymbol(const std::string& name)
            {
#ifdef _WIN32
                T result = reinterpret_cast<T>(GetProcAddress((HMODULE)handle, name.c_str()));
                if (!result) {
                    throw ray::DLException("Failed to retrieve symbol: " + std::to_string(GetLastError()));
                }
#else
                dlerror();
                T result = reinterpret_cast<T>(dlsym(handle, name.c_str()));
                const char *dlsym_error = dlerror();
                if (dlsym_error) {
                    throw ray::DLException("Failed to retrieve symbol: " + std::string(dlsym_error));
                }
#endif
                return result;
            }

        private:
#ifdef _WIN32
            HMODULE handle;
#else
            void* handle;
#endif
    };
}

#endif /* !DLCLASS_HPP_ */
