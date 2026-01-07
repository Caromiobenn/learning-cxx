#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    // 1个初始的shared，加上ptrs数组里的3个拷贝，共4个
    // weak_ptr 不增加引用计数
    ASSERT(observer.use_count() == 4, "");

    ptrs[0].reset();
    // 减少一个
    ASSERT(observer.use_count() == 3, "");

    ptrs[1] = nullptr;
    // 再减少一个
    ASSERT(observer.use_count() == 2, "");

    ptrs[2] = std::make_shared<int>(*shared);
    // ptrs[2] 指向了新分配的内存，旧内存引用计数再减一
    ASSERT(observer.use_count() == 1, "");

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    // 此时 ptrs[0], ptrs[1], ptrs[2] 都持有对象，计数 = 3

    std::ignore = std::move(ptrs[0]);
    // 重点：std::ignore 的 operator= 是个空操作模板。
    // 它没有接管 ptrs[0] 的资源，所以 ptrs[0] 依然有效！计数依然 = 3

    ptrs[1] = std::move(ptrs[1]);
    // 自移动：标准库 shared_ptr 对此有保护，计数不变 (= 3)

    ptrs[1] = std::move(ptrs[2]);
    // ptr[1] 释放旧指向（其实还是同一个），接管 ptr[2]。
    // 导致 ptr[2] 变为空，但 ptr[0] 和 ptr[1] 依然持有对象。
    // 所以此时计数应该是 2。
    ASSERT(observer.use_count() == 2, "");

    shared = observer.lock();
    // 通过 weak_ptr 提升，shared 重新指向，计数加 1
    ASSERT(observer.use_count() == 3, "");

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    // 所有 shared_ptr 全部释放
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();
    // 计数已为 0，lock() 返回空指针，计数依然为 0
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
