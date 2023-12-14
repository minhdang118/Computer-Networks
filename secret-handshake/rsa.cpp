#include <iostream>
#include "bigint.cpp"

using namespace std;

/* Encryption and Decryption Function */
BigInt &encrypt(BigInt &m, BigInt &e, BigInt &n){
    BigInt &temp = m;
    BigInt &result = m;
    BigInt zero("0");
    BigInt one("1");
    BigInt two("2");

    if (e == one)
    {
        m %= n;
        return m;
    }
    else if (e % two == zero)
    {
        e /= two;
        temp = encrypt(m, e, n);
        result = temp;
        result *= temp;
        result %= n;
        return result;
    } else {
        e -= one;
        temp = encrypt(m, e, n);
        result *= temp;
        result %= n;
        return result;
    }
}

/* Check if input string is number */
bool is_number(string s){
    for (int i = 0; i < s.length(); i++){
        if (!isdigit(s[i])){
            return false;
        }
    }
    return true;
}

/* Main Function */
int main(int argc, char const *argv[])
{
    /* Program greetings */
    cout << "Welcome to the RSA Encryption and Decryption Program!\n\n";

    /* Prompt for user input */
    string n;
    string e;
    string d;
    string m;

    /* Check if input is number */
    do
    {
        cout << "Enter your number n: ";
        cin >> n;
    } while (is_number(n) == false);
    
    do
    {
        cout << "Enter your number e: ";
        cin >> e;
    } while (is_number(e) == false);

    do
    {
        cout << "Enter your number d: ";
        cin >> d;
    } while (is_number(d) == false);

    do
    {
        cout << "Enter your number m: ";
        cin >> m;
    } while (is_number(m) == false);

    /* Convert to BigInt */
    BigInt n_bigint(n);
    BigInt e_bigint(e);
    BigInt d_bigint(d);
    BigInt m_bigint(m);

    /* Encrypt */
    BigInt c = encrypt(m_bigint, e_bigint, n_bigint);
    cout << "\nYour encrypted message is: ";
    cout << c << "\n";

    /* Decrypt */
    BigInt m2 = encrypt(c, d_bigint, n_bigint);
    cout << "\nYour decrypted message is: ";
    cout << m2 << "\n";

    return 0;
}
