#ifndef ACP_CACHE_HPP
#define ACP_CACHE_HPP

#include <algorithm>
#include <cstddef>
#include <list>
#include <new>
#include <ostream>
#include <queue>

template <class Key, class KeyProvider, class Allocator>
class Cache {
public:
    template <class... AllocArgs>
    explicit Cache(const std::size_t cache_size, AllocArgs &&...alloc_args)
        : m_max_top_size(cache_size), m_max_low_size(cache_size), m_alloc(std::forward<AllocArgs>(alloc_args)...) {}

    [[nodiscard]] std::size_t size() const { return m_low.size() + m_top.size(); }

    [[nodiscard]] bool empty() const { return size() == 0; }

    template <class T>
    T &get(const Key &key);

    std::ostream &print(std::ostream &strm) const;

    friend std::ostream &operator<<(std::ostream &strm, const Cache &cache) { return cache.print(strm); }

private:
    const std::size_t m_max_top_size;
    const std::size_t m_max_low_size;
    Allocator m_alloc;
    std::list<KeyProvider *> m_low;
    std::list<KeyProvider *> m_top;
};

template <class Key, class KeyProvider, class Allocator>
template <class T>
inline T &Cache<Key, KeyProvider, Allocator>::get(const Key &key) {
    const auto top_it = std::find_if(m_top.cbegin(), m_top.cend(), [&key](const KeyProvider *x) { return *x == key; });

    if (top_it != m_top.end()) {
        m_top.splice(m_top.cbegin(), m_top, top_it);
        return *reinterpret_cast<T *>(m_top.front());
    }

    const auto low_it = std::find_if(m_low.cbegin(), m_low.cend(), [&key](const KeyProvider *x) { return *x == key; });

    if (low_it != m_low.end()) {
        m_top.splice(m_top.cbegin(), m_low, low_it);
        if (m_top.size() > m_max_top_size) {
            m_low.splice(m_low.cbegin(), m_top, --m_top.end());
        }
        return *reinterpret_cast<T *>(m_top.front());
    }

    m_low.emplace_front(m_alloc.template create<T>(key));

    if (m_low.size() > m_max_low_size) {
        m_alloc.template destroy<KeyProvider>(m_low.back());
        m_low.pop_back();
    }

    return *reinterpret_cast<T *>(m_low.front());
}

template <class Key, class KeyProvider, class Allocator>
inline std::ostream &Cache<Key, KeyProvider, Allocator>::print(std::ostream &strm) const {
    strm << "Priority:";

    for (const auto it : m_top) {
        strm << ' ' << *it;
    }

    strm << "\nRegular:";

    for (const auto it : m_low) {
        strm << ' ' << *it;
    }

    return strm << "\n";
}

#endif  // ACP_CACHE_HPP
