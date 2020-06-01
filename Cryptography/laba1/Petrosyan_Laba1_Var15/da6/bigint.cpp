#include "bigint.hpp"
using namespace std;

BigInt::BigInt(const string &str) {
    for(int i = str.size() - 1; i >= 0; --i) {
        int tmp = 0;
        for(int j = 0;  i >= 0 && j < R; ++j, --i) {
            tmp = tmp + (str[i] - '0') * pow(10, j);
        }
        ++i;
        mData.push_back(tmp);
    }
    DeleteZeros();
}

BigInt::BigInt(const size_t &size) {
    mData.resize(size, 0);
}

BigInt::BigInt(int size) {
    if (!size) {
        mData.push_back(0);
    } else {
        while (size) {
            mData.push_back(size % BASE);
            size /= BASE;
        }
    }
}

BigInt
BigInt::Power(int r) {
    if(r == 0) {
        return BigInt("1");
    }
    if(r % 2 == 0) {
        BigInt res = (*this).Power(r / 2);
        return res * res;
    } else {
        BigInt res = (* this).Power(r - 1);
        return (*this) * res;
    }
}

void
BigInt::DeleteZeros() {
    while(mData.size() > 1 && mData.back() == 0 ) { 
        mData.pop_back();
    }
}


BigInt
BigInt::operator+(const BigInt &other) {
    int k = 0;
    int r = 0;
    size_t maxLen = max(mData.size(), other.size());
    BigInt result(maxLen + 1);
    for(size_t i = 0; i < maxLen; ++i) {
        if(mData.size() <= i) {
            k = other.Get(i);
        } else if(other.size() <= i) {
            k = mData[i];
        } else {
            k = mData[i] + other.Get(i);
        }
        k += r;
        result.Set(i, k % BASE);
        r = k / BASE;
    }
    if(r != 0) {
        result.Set(maxLen, r);
    }
    result.DeleteZeros();
    return result;
}

BigInt
BigInt::operator-(const BigInt &other) {
    int k = 0;
    int r = 0;
    int balance = 0;
    BigInt result(mData.size());
    for(int i = 0; i < mData.size(); ++i) {
        r = i < other.size() ? other.Get(i) : 0;
        k = mData[i] - r + balance;
        balance = k < 0 ? -1 : 0;
        result.Set(i, k < 0 ? k + BASE : k);
    }
    result.DeleteZeros();
    return result;
}

BigInt
BigInt::operator*(const BigInt &other) {
    size_t sizeLeft = size();
    size_t sizeRight = other.size();
    BigInt result(sizeLeft + sizeRight);
    for(int i = 0; i < sizeRight; ++i) {
        if(other.Get(i) == 0) {
            continue;
        }
        int balance = 0;
        for(int j = 0; j < sizeLeft; ++j) {
            int tmp = mData[j] * other.Get(i) + 
            result.Get(i + j) + balance;
            balance = tmp / BASE;
            result.Set(i + j, tmp % BASE);
        }
        result.Set(i + sizeLeft, balance);
    }
    result.DeleteZeros();
    return result;
}

BigInt
BigInt::operator/(BigInt &other) {
    int d = BASE/(other.mData.back() + 1);
    BigInt u = *this * d;
    BigInt v = other * d;
    size_t uSize = u.size();
    BigInt q(uSize);
    BigInt r;
    for(int i = uSize - 1; i >= 0; --i) {
        r = r * BASE;
        r = r + u.Get(i);
        int tmp1 = r.size() <= v.size() ? 0 : r.Get(v.size());
        int tmp2 = r.size() <= v.size() - 1 ? 0 : r.Get(v.size() - 1);
        int q2 = (tmp1 * BASE + tmp2) / v.mData.back();
        BigInt tmp = v * q2;

        while (tmp > r) {
            tmp = tmp - v;
            --q2;
        }
        r = r - tmp;
        q.Set(i, q2);
    }
    q.DeleteZeros();
    return q;
}

bool
BigInt::operator<(const BigInt &other) const {
    size_t n = size();
    if(n!= other.size()) {
        return n < other.size();
    }
    for(int i = n - 1; i >= 0; --i) {
        if(Get(i) != other.Get(i)) {
            return Get(i) < other.Get(i);
        }
    }
    return false;
}

bool
BigInt::operator>(const BigInt &other) const {
    size_t n = size();
    if(n != other.size()) {
        return n > other.size();
    }
    for(int i = n - 1; i >= 0; --i) {
        if(Get(i) != other.Get(i)) {
            return Get(i) > other.Get(i);
        }
    }
    return false;
}

bool
BigInt::operator==(const BigInt &other) const {
    size_t n = size();
    if(n != other.size()) {
        return false;
    }
    for(int i = n - 1; i >= 0; --i) {
        if(Get(i) != other.Get(i)) {
            return false;
        }
    }
    return true;
}

ostream &
operator<< (ostream& stream, const BigInt & number) {
    size_t n = number.size();
    if( n == 0 ) {
        return stream;
    }
    stream << number.mData[n - 1];
    for( int i = n - 2; i >= 0; --i ) {
        stream << setfill('0') << setw(R) << number.mData[i];
    }
    return stream;
}