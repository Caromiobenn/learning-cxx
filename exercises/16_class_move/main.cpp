#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

#include "../exercise.h"
#include <utility>   // std::move, std::exchange

class DynFibonacci {
    size_t *cache;
    int cached;      // 已经计算到的最大下标
    int capacity;    // cache 数组容量（元素个数）

public:
    // 动态设置容量的构造器
    DynFibonacci(int cap)
        : cache(nullptr), cached(1), capacity(cap) {
        ASSERT(capacity >= 2, "capacity must be >= 2");
        cache = new size_t[capacity];
        cache[0] = 0;
        cache[1] = 1;
    }

    // 移动构造器
    DynFibonacci(DynFibonacci &&other) noexcept
        : cache(std::exchange(other.cache, nullptr)),
          cached(std::exchange(other.cached, 0)),
          capacity(std::exchange(other.capacity, 0)) {}

    // 移动赋值（需要处理 self-move）
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        if (this == &other) {
            // self-move：保持对象不变最安全
            return *this;
        }

        delete[] cache;

        cache = std::exchange(other.cache, nullptr);//exchange方法交换两个变量的值并返回被交换变量的旧值,是std库中的一个函数 
        cached = std::exchange(other.cached, 0);
        capacity = std::exchange(other.capacity, 0);

        return *this;
    }

    // 析构器
    ~DynFibonacci() {
        delete[] cache;
        cache = nullptr;
        cached = 0;
        capacity = 0;
    }

    // 缓存优化斐波那契计算（非常量版本：可扩展缓存）
    size_t operator[](int i) {
        ASSERT(cache, "object is moved-from");
        ASSERT(i >= 0 && i < capacity, "i out of capacity range");

        // cached 表示“已算到的最大下标”
        // 当 cached < i 时，需要计算 cached+1
        for (; cached < i; ++cached) {
            cache[cached + 1] = cache[cached] + cache[cached - 1];
        }
        return cache[i];
    }

    // 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};


int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
