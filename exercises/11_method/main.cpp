#include "../exercise.h"

struct Fibonacci {
    unsigned long long cache[128];
    int cached;
    Fibonacci() {
        cache[0] = 0;
        cache[1] = 1;
        cached = 1;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
        ASSERT(i >= 0 && i < 128, "fibonacci index out of range");

        if (i <= cached) {
            return cache[i];
        }

        for (int k = cached + 1; k <= i; ++k) {
            cache[k] = cache[k - 1] + cache[k - 2];
        }
        cached = i;

        return cache[i];
    }
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib;

    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
