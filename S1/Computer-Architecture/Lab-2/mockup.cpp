#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <array>
#include <ctime>
#include <iostream>
#include <cstring>

using std::array;

// Глобальные переменные для подсчета тактов, попаданий, промахи.
uint64_t clocks = 0;
uint32_t hits = 0;
uint32_t misses = 0;

// Структура реализующая кэш.
// Базовые константы взяты из параметров системы.
struct Cache {
    static constexpr size_t CACHE_SETS_COUNT = 64;
    static constexpr size_t CACHE_LINE_SIZE = 16;
    static constexpr uint32_t CACHE_SET_SIZE = 6;
    static constexpr uint32_t CACHE_OFFSET_SIZE = 4;
    
    // Структура задающая кэш-линию.
    // Содержит tag, биты V, D, Dlt.
    // Информация - "Мнимая".
    struct Line {
        uint32_t tag;

        bool dirty = false;
        bool valid = false;

        // Data.

        uint32_t delta = 1;
    };

    // way описывается, как статический массив линий.
    using Way = array<Line, CACHE_SETS_COUNT>;

    // В данной реализции way разделенны в отдельные переменные.
    Way way1{};
    Way way2{};

    // LRU возвращает итератор на замещаймую линию.
    Way::iterator lru(uint32_t set) {
        if (way1[set].valid && way2[set].valid) {
            return way1[set].delta > way2[set].delta ? way1.begin() + set : way2.begin() + set;
        }
        
        return way1[set].valid ? way2.begin() + set : way1.begin() + set;
    }

    // write_back 
    // Пишет линию обратно в память в соответсвии с описанием.
    void write_back(Way::iterator it) {
        if (!it->valid || !it->dirty) {
            return;
        }

        // Do move.
        ++clocks; // Address pass.

        clocks += 100; // Memory await.

        // линия более не валидна.
        it->valid = false;
        it->dirty = false;
        it->delta = 1;

        clocks += 8; // Transmitting.
    }

    // Чтение линии из памяти.
    Way::iterator read_line(uint32_t addr) {
        uint32_t set = addr_set(addr);
        uint32_t tag = addr_tag(addr);

        Way::iterator it = lru(set);

        write_back(it);

        // Ставить Dlt для двух в 1, т.к новая линия будет инициализированна с 0.

        way1[set].delta = 1;
        way2[set].delta = 1;

        // запись в кэш.
        it->tag = tag;
        it->delta = 0;
        it->valid = true;
        it->dirty = false;

        // Do read.

        ++clocks; // Address pass.
        clocks += 100; // Memory await.
        clocks += 8; //fetching.

        return it;
    }

    // Поиск линии в set, если не найдет вернет конец второго way.
    // Иначе итератор на нужную линию.
    Way::iterator find(uint32_t tag, uint32_t set) {
        if (way1[set].valid && way1[set].tag == tag) {
            // Обновляем Dlt.
            if (way2[set].valid) {
                way1[set].delta = 0;
                way2[set].delta = 1;
            }
            ++hits;

            clocks += 6; // Cache hit await.

            return way1.begin() + set;
        }
        else if (way2[set].valid && way2[set].tag == tag) {
            // Обновляем Dlt.
            if (way1[set].valid) {
                way2[set].delta = 0;
                way1[set].delta = 1;
            }
            ++hits;

            clocks += 6; // Cache hit await.

            return way2.begin() + set;
        }
        else {
            ++misses;

            clocks += 4; // Cache miss await.

            return way2.end();
        }
    }

    // Функции разбиения адреса.

    // Fetch Tag from address.
    uint32_t addr_tag(uint32_t addr) {
        return (addr >> CACHE_OFFSET_SIZE >> CACHE_SET_SIZE);
    }

    // Fetch Set from address.
    uint32_t addr_set(uint32_t addr) {
        return (addr >> CACHE_OFFSET_SIZE & ((1 << CACHE_SET_SIZE) - 1));
    }

    // Fetch Offset from address.
    uint32_t addr_offset(uint32_t addr) {
        return (addr & ((1 << CACHE_OFFSET_SIZE) - 1));
    }

    // Чтение 8 бит из кэша, проделываем описанные в предыдущем пункте операции.
    int8_t read_int8(uint32_t addr) {
        clocks += 2; // Address receive.

        uint32_t set = addr_set(addr);
        uint32_t tag = addr_tag(addr);

        // Поиск линии.
        Way::iterator it = find(tag, set);

        // Если не найдена - читаем из памяти.
        if (it == way2.end()) {
            read_line(addr);
        }

        ++clocks; // Response. 

        return 0;
    }

    // Аналогично для 16 бит.
    int16_t read_int16(uint32_t addr) {
        clocks += 2; // Address receive.

        uint32_t set = addr_set(addr);
        uint32_t tag = addr_tag(addr);

        Way::iterator it = find(tag, set);

        if (it == way2.end()) {
            read_line(addr);
        }

        ++clocks; // Response.

        return 0;
    }

    // Аналогично, так-как данные "Мнимые" и букально только запрос данных.
    void write_int32(uint32_t addr, int32_t value) {
        clocks += 2; // Address receive.

        uint32_t set = addr_set(addr);
        uint32_t tag = addr_tag(addr);

        Way::iterator it = find(tag, set);

        if (it == way2.end()) {
            it = read_line(addr);
        }

        clocks++; // Responce;

        it->dirty = true;
    }

    // Выписываем все изменения из кэша обратно в память.
    void goodbye() {
        for (auto it = way1.begin(); it != way1.end(); ++it) {
            write_back(it);
        }

        for (auto it = way2.begin(); it != way2.end(); ++it) {
            write_back(it);
        }
    }
};

// Экземпляр кэша.
Cache cache;

// Константы задания.
constexpr int32_t M = 64;
constexpr int32_t N = 60;
constexpr int32_t K = 32;

// Заменены базовые опрерации, чтобы встроить счетчик тактов.

// Initializer for uint32 with embeded clock tick.
uint32_t init_uint32(uint32_t value) {
    ++clocks;
    return value;
} 

// Initializer for int32 with embeded clock tick.
int32_t init_int32(int32_t value) {
    ++clocks;
    return value;
}

// Default addition with embeded clock tick.
void addition(uint32_t& value, const uint32_t add) {
    ++clocks;
    value += add;
}

// Default signed addition with embeded clock tick.
void addition(int32_t& value, const int32_t add) {
    ++clocks;
    value += add;
}

// Default signed multiplication embeded clock tick.
int32_t multiplication(const int8_t mul_1, const int16_t mul_2) {
    clocks += 5;
    return mul_1 * mul_2;
}

// Just pass the tick for next iteration of function exit.
void next() {
    ++clocks;
}

// Все опреции заменины на соответствующие со встроенным тактовым счетом.

void mmul() {
    uint32_t pa_addr = init_uint32(0); // a begin.

    uint32_t pc_addr = init_uint32(5888); // M * K + 2 bytes * K * N

    for (int y = init_int32(0); y < M; addition(y, 1)) {

        for (int x = init_int32(0); x < N; addition(x, 1)) {
        
            uint32_t pb_addr = init_uint32(2048); // M * K
            int32_t s = init_int32(0);
            
            for (int k = init_int32(0); k < K; addition(k, 1)) {
            
                addition(
                    s, 
                    multiplication(
                        cache.read_int8(pa_addr + k), 
                        cache.read_int16(pb_addr + x * 2)
                    )
                );
                addition(pb_addr, N * 2);
            }
            cache.write_int32(pc_addr + x * 4, s);
        }
        addition(pa_addr, K);
        addition(pc_addr, N * 4);
    }
    next();
}

int main() {

    mmul();

    cache.goodbye();

    std::cout << "Clocks: " << clocks << '\n';
    std::cout << "Hits: " << hits << " | " << static_cast<double>(hits) / static_cast<double>(hits + misses) << '\n';
    std::cout << "Misses: " << misses << std::endl;

    return 0;
}
