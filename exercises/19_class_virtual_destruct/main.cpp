#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段
    static int num_a;//统计A的实例的数量

    A() {
        ++num_a;
    }
    virtual ~A() {
        --num_a;//析构函数也需要声明为虚函数
    }

    virtual char name() const {
        return 'A';
    }
};
struct B final : public A {
    // TODO: 正确初始化静态字段
    static int num_b ;

    B() {
        ++num_b;
        //A有无参构造,所以省略了A()
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};
int A::num_a=0,B::num_b=0;//静态的变量要单独初始化

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B;// 派生类指针可以随意转换为基类指针
    //这一步不会丢弃任何的信息,虚表,成员都是带着过来的
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");
    //这一步体现了多态

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    B &bb = dynamic_cast<B &> (*ab);//ab加星号,就解成了左值引用,然后再转化为B的左值引用;
    //编译器会通过虚表等信息,判断这个转换是否真的可以发生,发生在运行时间段
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab;// 通过指针可以删除指向的对象，即使是多态对象
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}
