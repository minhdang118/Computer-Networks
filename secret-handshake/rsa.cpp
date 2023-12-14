#include <iostream>
#include "bigint.cpp"

using namespace std;

/* Encryption Function */
BigInt &encrypt(BigInt m, BigInt e, BigInt n){
    BigInt c;
    BigInt zero("0");
    BigInt two("2");

    if (e % two == zero)
    {
        /* code */
    }
    
    
    return operator%=(operator^=(m, e), n);
}

/* Decryption Function */
BigInt &decrypt(BigInt c, BigInt d, BigInt n){
    return operator%=(operator^=(c, d), n);
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

    /* Decrypt */
    BigInt m2 = decrypt(c, d_bigint, n_bigint);

    /* Print results */
    cout << "\nYour encrypted message is: ";
    cout << c << "\n";

    cout << "\nYour decrypted message is: ";
    cout << m2 << "\n";

    return 0;
}
