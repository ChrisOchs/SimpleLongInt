#include <iostream>
#include <iomanip>

#include "LongInt.h"


LongInt::LongInt(int* arr, size_t arrSize, int sign) {
    this->arr = arr;
    this->arrSize = arrSize;
    this->sign = sign;
}

LongInt::~LongInt() {
    delete [] this->arr;
}

int
LongInt::getSign() {
    return this->sign;
}

void
LongInt::print() {

    if(this->getSign() == -1) {
        std::cout<<'-';
    }

    std::cout<<this->arr[this->arrSize - 1];

    int numEntries = this->arrSize;

    for(int c = numEntries - 2; c >= 0; c--) {
        std::cout<<std::setw(8)<<std::setfill('0')<<this->arr[c];
    }
}

bool
LongInt::equals(LongInt* other) {
    if(this->arrSize != other->arrSize) {
        return false;
    }

    if(this->sign != other->sign) {
        return false;
    }

    for(size_t c = 0; c < this->arrSize; c++) {
        if(this->arr[c] != other->arr[c]) {
            return false;
        }
    }

    return true;
}

bool
LongInt::lessThan(LongInt* other) {

    if(this->getSign() == other->getSign()) {
        if(this->arrSize == other->arrSize) {
            for(size_t c = 0; c < this->arrSize; c++) {
                if(this->arr[c] < other->arr[c]) {
                    return true;
                }
            }

            return false;
        } else {
            return this->arrSize < other->arrSize;
        }
    } else {
        return this->getSign() < other->getSign();
    }
}

bool
LongInt::greaterThan(LongInt* other) {

    if(this->getSign() == other->getSign()) {
        if(this->arrSize == other->arrSize) {
            for(size_t c = 0; c < this->arrSize; c++) {
                if(this->arr[c] > other->arr[c]) {
                    return true;
                }
            }

            return false;
        } else {
            return this->arrSize > other->arrSize;
        }
    } else {
        return this->getSign() > other->getSign();
    }
}

LongInt*
LongInt::abs() {
    return new LongInt(this->arr, this->arrSize, 1);
}

// Computes A + B
LongInt*
LongInt::add(LongInt* other) {
    if(this->getSign() == 1 && other->getSign() == 1) {

        return this->addAbs(other);

    } else if (this->getSign() == -1 && other->getSign() == -1) {

        LongInt* prelimResult = this->addAbs(other);

        return new LongInt(prelimResult->arr, prelimResult->arrSize, -1);

    } else if(this->getSign() == -1 && other->getSign() == 1) {

        return other->subtract(this->abs());

    } else {
        return this->subtract(other->abs());
    }
}


// Computes A - B
LongInt*
LongInt::subtract(LongInt* other) {
    if(this->getSign() == 1 && other->getSign() == 1) {

        LongInt* prelimResult = this->subtractAbs(other);

        int sign = 1;

        if(this->lessThan(other)) {
            sign = -1;
        }

        return new LongInt(prelimResult->arr, prelimResult->arrSize, sign);

    } else if(this->getSign() == -1 && other->getSign() == -1) {

        LongInt* prelimResult = this->subtractAbs(other);

        int sign = -1;

        if(this->greaterThan(other)) {
            sign = 1;
        }

        return new LongInt(prelimResult->arr, prelimResult->arrSize, sign);

    } else if(this->getSign() == 1 && other->getSign() == -1) {

        LongInt* otherAbs = other->abs();

        return add(otherAbs);

    } else {
        LongInt* myAbs = this->abs();

        LongInt* prelimResult = myAbs->add(other);

        return new LongInt(prelimResult->arr, prelimResult->arrSize, -1);
    }
}

LongInt*
LongInt::addAbs(LongInt* other) {
    LongInt* myAbs = this->abs();
    LongInt* otherAbs = other->abs();

    size_t maxResultSize;

    if(myAbs->arrSize > otherAbs->arrSize) {
        maxResultSize = myAbs->arrSize;
    } else {
        maxResultSize = otherAbs->arrSize;
    }

    maxResultSize += 1;

    int* resultArray = new int [maxResultSize];

    size_t i = 0;
    size_t j = 0;
    size_t c = 0;

    int carry = 0;

    while(i < myAbs->arrSize && j < otherAbs->arrSize) {
        int value = myAbs->arr[i] + otherAbs->arr[j] + carry;

        resultArray[c] = underflow(value);

        carry = overflow(value);

        i++;
        j++;
        c++;
    }

    while(i < myAbs -> arrSize) {
        int value = myAbs->arr[i] + carry;

        carry = overflow(value);

        resultArray[c] = underflow(value);

        i++;
        c++;
    }

    while(j < otherAbs ->arrSize) {
        int value = otherAbs->arr[j] + carry;

        carry = overflow(value);

        resultArray[c] = underflow(value);

        j++;
        c++;
    }

    if(carry != 0) {
        resultArray[c] = carry;
        c++;
    }

    int* finalResultArray;

    if(c < maxResultSize) {
        finalResultArray = new int[c];

        for(size_t i = 0; i < c; i++) {
            finalResultArray[i] = resultArray[i];
        }

        delete [] resultArray;
    } else {
        finalResultArray = resultArray;
    }

    return new LongInt(finalResultArray, c, 1);
}

LongInt*
LongInt::subtractAbs(LongInt* other) {

    LongInt* myAbs = this->abs();
    LongInt* otherAbs = other->abs();

    LongInt* bigger;
    LongInt* smaller;

    if(myAbs->greaterThan(otherAbs)) {
        bigger = myAbs;
        smaller = otherAbs;
    } else {
        bigger = otherAbs;
        smaller = myAbs;
    }

    int maxResultSize = bigger->arrSize + 1;
    int* resultArr = new int[maxResultSize];

    int borrow = 0;

    size_t i = 0;
    size_t j = 0;
    size_t c = 0;

    int nonZeroIndex = -1;

    while(i < bigger->arrSize && j < smaller->arrSize) {

        int value = bigger->arr[i] - smaller->arr[j] - borrow;

        if(value < 0) {
            value = (bigger->arr[i] + 100000000) - smaller->arr[j] - borrow;

            borrow = 1;
        } else {
            borrow = 0;
        }

        resultArr[c] = value;

        if(value > 0) {
            nonZeroIndex = c;
        }

        i++;
        j++;
        c++;
    }

    while(i < bigger -> arrSize) {
        int value = bigger->arr[i] - borrow;

        if(value < 0) {
            value = (bigger->arr[i] + 100000000) - borrow;
            borrow = 1;
        } else {
            borrow = 0;
        }

        resultArr[c] = value;

        if(value > 0) {
            nonZeroIndex = c;
        }

        i++;
        c++;
    }

    int* finalResultArray;
    int finalResultSize;

    if(nonZeroIndex == -1) {
        finalResultArray = new int[1];
        finalResultArray[0] = 0;
        finalResultSize = 1;
    } else {
        if( (nonZeroIndex + 1) < c) {
            finalResultArray = new int[nonZeroIndex + 1];

            for(int x = 0; x <= nonZeroIndex; x++) {
                finalResultArray[x] = resultArr[x];
            }

            finalResultSize = nonZeroIndex + 1;

            delete[] resultArr;
        } else {
            finalResultArray = resultArr;
            finalResultSize = c;
        }
    }

    return new LongInt(finalResultArray, finalResultSize, 1);
}

bool
LongInt::isZero(){
    return this->arrSize == 1 && this->arr[0] == 0;
}


int overflow(int value) {
    return value / 100000000;
}

int underflow(int value) {
    return value % 100000000;
}


