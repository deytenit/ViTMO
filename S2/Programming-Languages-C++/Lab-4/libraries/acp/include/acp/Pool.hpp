#ifndef ACP_POOL_HPP
#define ACP_POOL_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <new>
#include <vector>

class PoolAllocator {
public:
    static const size_t npos = -1;

    struct block {
        std::size_t size = 0;
        void* ptr        = nullptr;

        ~block() { unlock(); }

        void lock();
        void unlock();
    };

public:
    PoolAllocator(std::size_t min_pool, std::size_t max_pool);

    ~PoolAllocator();

    void* allocate(std::size_t size);

    void deallocate(const void* ptr);

private:
    std::size_t find(std::size_t size);
    std::size_t find(const void* ptr);
    void split(std::size_t pos);
    void merge(std::size_t pos);

private:
    const std::size_t m_min_pool;
    const std::size_t m_pool_amount;

    block* m_pools;
};

#endif  // ACP_POOL_HPP