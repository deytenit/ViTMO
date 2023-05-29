#include "acp/Pool.hpp"

namespace {
constexpr std::size_t pow2(const unsigned x) {
    return (1ULL << x);
}

constexpr unsigned ceilLog2(const std::size_t x) {
    const unsigned POW_LIMIT = sizeof(std::size_t) * 8;

    for (unsigned i = 0; i < POW_LIMIT; ++i) {
        if (pow2(i) >= x) {
            return i;
        }
    }

    return POW_LIMIT;
}
}  // namespace

void PoolAllocator::block::lock() {
    ptr = ::operator new(size);
}

void PoolAllocator::block::unlock() {
    ::operator delete(ptr);
    ptr = nullptr;
}

PoolAllocator::PoolAllocator(const std::size_t min_pool, const std::size_t max_pool)
    : m_min_pool(min_pool), m_pool_amount(pow2(max_pool - min_pool)) {
    m_pools         = new block[m_pool_amount];
    m_pools[0].size = pow2(max_pool);
}

PoolAllocator::~PoolAllocator() {
    delete[] m_pools;
}

void* PoolAllocator::allocate(const std::size_t size) {
    const unsigned k      = ceilLog2(size);
    const std::size_t pos = find(pow2(k));

    if (pos == npos) {
        throw std::bad_alloc{};
    }

    while (m_pools[pos].size >> 1ULL >= std::max(size, pow2(m_min_pool))) {
        split(pos);
    }

    m_pools[pos].lock();

    return m_pools[pos].ptr;
}

void PoolAllocator::deallocate(const void* ptr) {
    std::size_t pos = find(ptr);

    if (pos == npos) {
        return;
    }

    m_pools[pos].unlock();

    bool has_buddy = true;

    while (has_buddy) {
        has_buddy = false;
        const std::size_t rpos =
            m_pool_amount - pos > m_pools[pos].size >> m_min_pool ? pos + (m_pools[pos].size >> m_min_pool) : npos;
        const std::size_t lpos =
            pos >= m_pools[pos].size >> m_min_pool ? pos - (m_pools[pos].size >> m_min_pool) : npos;

        if (rpos != npos && !m_pools[rpos].ptr && m_pools[pos].size == m_pools[rpos].size) {
            merge(pos);
            has_buddy = true;
            continue;
        }

        if (lpos != npos && !m_pools[lpos].ptr && m_pools[pos].size == m_pools[lpos].size) {
            pos = lpos;
            merge(pos);
            has_buddy = true;
        }
    }
}

std::size_t PoolAllocator::find(const std::size_t size) {
    std::size_t result = npos;
    std::size_t diff   = 0;
    for (std::size_t i = 0; i < m_pool_amount; ++i) {
        if (m_pools[i].size >= size && !m_pools[i].ptr && (result == npos || diff > m_pools[i].size - size)) {
            diff   = m_pools[i].size - size;
            result = i;
        }
    }

    return result;
}

std::size_t PoolAllocator::find(const void* ptr) {
    for (std::size_t i = 0; i < m_pool_amount; ++i) {
        if (m_pools[i].ptr == ptr) {
            return i;
        }
    }

    return npos;
}

void PoolAllocator::split(const std::size_t pos) {
    m_pools[pos].size >>= 1ULL;
    m_pools[pos + (m_pools[pos].size >> m_min_pool)].size = m_pools[pos].size;
}

void PoolAllocator::merge(const std::size_t pos) {
    const std::size_t post_size = m_pools[pos].size;
    m_pools[pos].size <<= 1ULL;
    m_pools[pos + (post_size >> m_min_pool)].size = 0;
}
