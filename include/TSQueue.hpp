/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** TSQueue.hpp
*/

#ifndef TSQUEUE_HPP
#define TSQUEUE_HPP

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

template <typename T>
class TSQueue {
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;

public:
    void push(T item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(item);
        m_cond.notify_one();
    }
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this]() { return !m_queue.empty(); });
        T item = m_queue.front();
        m_queue.pop();
        return item;
    }
    size_t size()
    {
        return m_queue.size();
    }
};

#endif
