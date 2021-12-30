#pragma once

#include <deque>
#include <mutex>

namespace net
{

template<typename T>
class TsQueue final
{
public:
    TsQueue() = default;
    ~TsQueue() = default;

    TsQueue(TsQueue const&) = delete;
    TsQueue& operator=(TsQueue const&) = delete;
    TsQueue(TsQueue&&) noexcept = default;
    TsQueue& operator=(TsQueue&&) noexcept = default;

    [[nodiscard]] auto front() const
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        return m_queue.front();
    }

    void push(T&& item)
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_queue.emplace_back(std::move(item));
    }

    void push(T const& item)
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_queue.push_back(item);
    }


    void pop()
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_queue.pop_front();
    }

    [[nodiscard]] T take_front() &&
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        auto item = std::move(m_queue.front());
        m_queue.pop_front();
        return item;
    }

    std::size_t size() const
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        return m_queue.size();
    }

    bool empty() const
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        return m_queue.empty();
    }

    void clear()
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_queue.clear();
    }

private:
    mutable std::mutex m_mutex;
    std::deque<T> m_queue;
};
} // namespace net
