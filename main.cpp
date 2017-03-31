#include <iostream>

#include "LongInt.h"

int main()
{
    int test1[] = {11111111, 22222222, 3333};
    int test2[] = {11111111, 22222222, 44444444, 55555555, 1111};

    LongInt a(test1, 3, 1);
    LongInt b(test2, 5, -1);

    LongInt* result = a.subtract(&b);

    std::cout<<"RESULT: "; result->print();
    std::cout<<std::endl;

    delete result;

    result = a.add(&b);
    std::cout<<"RESULT: "; result->print();
    std::cout<<std::endl;



    return 0;
}
