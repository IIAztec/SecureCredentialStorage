#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>

using namespace std;

struct newAcc
{
    int id;
    string login;
    string password;
};

bool checkSameLogin(string userLogin) {
    ifstream base("base.csv");
    bool fail = false;
    string line;
    getline(base, line);
    string userID, checkLogin;
    while(getline(base, line)) {
        stringstream ss(line);
        getline(ss, userID, ',');
        getline(ss, checkLogin, ',');
        if(userLogin == checkLogin) {
            fail = true;
            break;
        } else {
            continue;
        }
    }
    base.close();
    return fail;
}

string hashingpass(string pass) { // hashing the password using djb2 algorithm
    unsigned long hash = 5381; // initializing the hash value to a large prime number
    for(char c : pass) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c, shifting the hash value to the left by 5 bits and adding the current character
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash; // converting the hash value to a hexadecimal string
    return ss.str(); // returning the hashed password as a string
}

int IDgenerator() {
    ifstream base("base.csv");
    string line;
    getline(base, line);
    int newID;
    string userID;
    while(getline(base, line)) {
        stringstream ss(line);
        getline(ss, userID, ',');
    }
    newID = stoi(userID);
    newID += 1;
    base.close();
    return newID;
}

string XORencrypt(string cryptoKey, string data) {
    int key_lenght = cryptoKey.size();
    for(int i = 0 ; i < data.size() ; i++) {
        data[i] ^= cryptoKey[i % key_lenght];
    }
    return data;
}

void registration(newAcc &account) {
    bool checkSameUserLogin;
    bool accountCreated = false;
    string userLogin;
    string userPass;
    while(!accountCreated) {
        cout << "Create login: "; cin >> userLogin;
        cout << "Create password: "; cin >> userPass;
        checkSameUserLogin = checkSameLogin(userLogin);
        if(checkSameUserLogin) {
            cout << "Data base already has the same login - " << userLogin << endl;
        } else {
            accountCreated = true;
            account.login = userLogin;
            account.password = hashingpass(userPass);
        }
    }
}

void writeFile(newAcc account) {
    ofstream base("base.csv", ios::app);
    base << IDgenerator() << "," << account.login << "," << account.password;
    base.close();
}

void createUserFile(newAcc account, string cryptoKey) {
    string example = "example";
    ofstream newFile(account.login + ".csv");
    newFile << "Number,Login,Password" << endl;
    newFile << 0 << "," << XORencrypt(cryptoKey, example) << "," << XORencrypt(cryptoKey, example) << endl;
    newFile.close();
}

int main() {
    newAcc account;
    cout << "Welocome to registration panel" << endl;
    registration(account);
    writeFile(account);
    string cryptoKey = account.password;
    createUserFile(account, cryptoKey);
}