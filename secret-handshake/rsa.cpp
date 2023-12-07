#include <iostream>
#include "bigint.cpp"

using namespace std;

/* Encryption Function */
BigInt encrypt(BigInt m, BigInt e, BigInt n){
    BigInt c;
    if (operator==(operator%(e, BigInt("2")), BigInt("0")))
    {
        return operator%(operator^(encrypt(m, operator/(e, BigInt("2")), n), BigInt("2")), n);
    } 
    else
    {
        return operator*(operator%(m, n), operator%(m, n));
    }
    
    
    c = operator^(m, e);
    c = operator%(c, n);
    return c;
}

int main(int argc, char const *argv[])
{
    /* Program greetings */
    cout << "Welcome to the RSA Encryption Program!\n";

    /* Prompt for user input */
    unsigned long long n;
    unsigned long long e;
    unsigned long long d;
    unsigned long long m;
    cout << "Enter your number n: ";
    cin >> n;
    cout << "Enter your number e: ";
    cin >> e;
    cout << "Enter your number d: ";
    cin >> d;
    cout << "Enter your message m: ";
    cin >> m;

    /* Convert to BigInt */
    BigInt n_bigint(n);
    BigInt e_bigint(e);
    BigInt d_bigint(d);
    BigInt m_bigint(m);




    return 0;
}
