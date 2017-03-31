#ifndef LONGINT_H
#define LONGINT_H

int overflow(int);
int underflow(int);

class LongInt
{
    private:
        int sign;

        size_t arrSize;
        int * arr;

        LongInt* addAbs(LongInt*);
        LongInt* subtractAbs(LongInt*);

    public:
        LongInt(int* arr, size_t arrSize, int sign);

        ~LongInt();

        int getSign();

        void print();

        bool equals(LongInt*);
        bool lessThan(LongInt*);
        bool greaterThan(LongInt*);

        LongInt* add(LongInt*);
        LongInt* subtract(LongInt*);
        LongInt* abs();

        bool isZero();
};

#endif
