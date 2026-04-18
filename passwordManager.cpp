#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>

using namespace std;

struct account // struct to store the user's login and password, the password is not stored in plain text, but in a hashed form using the hashingpass function, the cryptoKey is generated from the user's password and is used to encrypt and decrypt the service data using XOR cipher
{
    string login;
    string password;
};

struct service // struct to store the service data, including the service name, login and password for that service, the password is stored in an encrypted form using XOR cipher with the cryptoKey generated from the user's password
{
    int id;
    string service;
    string serviceLogin;
    string servicePassword;
};

string hashingpass(string pass) { // hashing the password using djb2 algorithm
    unsigned long hash = 5381; // initializing the hash value to a large prime number
    for(char c : pass) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c, shifting the hash value to the left by 5 bits and adding the current character
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash; // converting the hash value to a hexadecimal string
    return ss.str(); // returning the hashed password as a string
}

void XORencryptDeCryptForThreeData(string cryptoKey, string &service, string &servLogin, string &servPass) { // encrypting or decrypting the data using XOR cipher, the same function can be used for both operations because of the properties of XOR
    int key_lenght = cryptoKey.size(); // calculating the length of the crypto key to use it in the encryption/decryption process
    int service_lenght = service.size();
    int servLogin_lenght = servLogin.size();
    int servPass_lenght = servPass.size();
    for(int i = 0 ; i < service_lenght ; i++) { // iterating through each character of the service name and applying XOR operation with the corresponding character of the crypto key, using modulo operator to wrap around the key if it's shorter than the service name
        service[i] ^= cryptoKey[i % key_lenght];
    }
    for(int i = 0 ; i < servLogin_lenght ; i++) { // iterating through each character of the service login and applying XOR operation with the corresponding character of the crypto key, using modulo operator to wrap around the key if it's shorter than the service login
        servLogin[i] ^= cryptoKey[i % key_lenght];
    }
    for(int i = 0 ; i < servPass_lenght ; i++) { // iterating through each character of the service password and applying XOR operation with the corresponding character of the crypto key, using modulo operator to wrap around the key if it's shorter than the service password
        servPass[i] ^= cryptoKey[i % key_lenght];
    }
}

void XORencryptDeCryptForOneData(string cryptoKey, string &data) { // encrypting or decrypting the data using XOR cipher, the same function can be used for both operations because of the properties of XOR
    int key_lenght = cryptoKey.size();
    for(int i = 0 ; i < data.size() ; i++) {
        data[i] ^= cryptoKey[i % key_lenght];
    }
}

bool validInputForLogin(string data[2]) { // checking if the user's input for login and password contains any not allowed characters, if it does, the function returns false and the user is prompted to enter the data again, if it doesn't, the function returns true and the login process continues
    string login = data[0];
    string password = data[1];
    string notAllowedSing = "!@#$%^&*()_-|/.,;:";
    bool validLogin = (notAllowedSing.find_first_of(login) == string::npos); // checking if the login contains any not allowed characters by using the find_first_of function, if it returns npos, it means that none of the not allowed characters were found in the login, and the login is considered valid
    bool validPass = (notAllowedSing.find_first_of(password) == string::npos); // checking if the password contains any not allowed characters by using the find_first_of function, if it returns npos, it means that none of the not allowed characters were found in the password, and the password is considered valid
    bool result = false;
    if(validLogin && validPass) {
        result = true;
        return result; // if both the login and the password are valid, the function returns true and the login process continues
    } else return result; // if either the login or the password contains not allowed characters, the function returns false and the user is prompted to enter the data again
}

bool validInputForDel(int input, bool valid) { // checking if the user's input for confirming the deletion of an entry is valid, if it is, the function returns true and the deletion process continues, if it isn't, the function returns false and the user is prompted to enter the input again
    if(input == 1 || input == 2) {
        valid = true;
    } else {
        cout << "Wrong input" << endl;
    }
    return valid; // if the input is either 1 or 2, the function returns true and the deletion process continues, if it isn't, the function returns false and the user is prompted to enter the input again
}

bool checkService(service findByService, vector<service> data) { // checking if the service that the user is trying to find or edit exists in the user's storage, if it does, the function returns false and the process continues, if it doesn't, the function returns true and the user is informed that the service was not found
    bool fail = true;
    for(int i = 0 ; i < data.size() ; i++) { // iterating through the vector of services and checking if the service name matches the one that the user is trying to find or edit, if it does, the function returns false and the process continues, if it doesn't, the loop continues until all services have been checked
        if(findByService.service == data[i].service) {
            fail = false;
        } 
    }
    if(fail) { // if the service was not found in the user's storage, the function returns true and the user is informed that the service was not found
        cout << "The program cant find this service in your storage." << endl;
    }
    return fail; // if the service was found in the user's storage, the function returns false and the process continues, if it wasn't, the function returns true and the user is informed that the service was not found
}

service findService(vector<service> data, string cryptoKey) { // finding the service that the user is trying to find or edit in the user's storage, if it is found, the function returns the service data, if it isn't, the function returns an empty service struct and the user is informed that the service was not found
    service findService;
    string userInput;
    cout << "Enter service name: "; cin >> userInput;
    XORencryptDeCryptForOneData(cryptoKey, userInput); // encrypting the user's input for the service name using XOR cipher with the cryptoKey generated from the user's password, this is done to match the encrypted service names stored in the user's storage, if the user's input matches an encrypted service name in the storage, it means that the user has entered the correct service name and the function can return the corresponding service data
    for(int i = 0 ; i < data.size() ; i++) {
        if(userInput == data[i].service) { // if the user's input for the service name matches an encrypted service name in the storage, the function returns the corresponding service data, if it doesn't, the loop continues until all services have been checked
            findService.id = data[i].id;
            findService.service = data[i].service;
            findService.serviceLogin = data[i].serviceLogin;
            findService.servicePassword = data[i].servicePassword;
            break;
        } else {
            continue;
        }
    }
    return findService; // if the service was found in the user's storage, the function returns the corresponding service data, if it wasn't, the function returns an empty service struct and the user is informed that the service was not found
}

service changeLogin(service findByService) { // changing the login for a specific service, the function takes the service data as an argument, prompts the user to enter a new login for that service, updates the service data with the new login, and returns the updated service data
    string newLogin;
    cout << "Your current login is " << findByService.serviceLogin << endl;
    cout << "Enter new login for " << findByService.service << ": "; cin >> newLogin;
    findByService.serviceLogin = newLogin;
    return findByService; // the function returns the updated service data with the new login, this updated data can then be used to update the user's storage with the new login for that service
}

service changePass(service findByService) { // changing the password for a specific service, the function takes the service data as an argument, prompts the user to enter a new password for that service, updates the service data with the new password, and returns the updated service data
    string newPass;
    cout << "Your current password is " << findByService.servicePassword << endl;
    cout << "Enter new password for " << findByService.service << ": "; cin >> newPass;
    findByService.servicePassword = newPass;
    return findByService; // the function returns the updated service data with the new password, this updated data can then be used to update the user's storage with the new password for that service
}


int numberGenerator(account acc) { // generating a new number for a new entry by counting the number of existing entries in the user's storage and adding 1 to it, this ensures that each entry has a unique number that can be used as an identifier for that entry
    ifstream readFile(acc.login + ".csv"); // opening the user's storage file to read the existing entries and count them, the file is named after the user's login and has a .csv extension, if the file cannot be opened, the function informs the user and exits the program
    int number = 0;
    if(readFile.is_open()) {
        readFile.clear();
        readFile.seekg(0);
        string line;
        getline(readFile, line);
        string userServNumber;
        while(getline(readFile, line)) { // iterating through the lines of the user's storage file and counting the number of existing entries by reading the first value of each line, which is the entry number, and adding 1 to it to generate a new number for the new entry, if the file is empty, the function will return 1 as the number for the first entry
            stringstream ss(line);
            getline(ss, userServNumber);
            number += 1;
        }
    } else {
        cout << "Something went wrong. Try again later";
        exit(0);
    }
    readFile.close(); // closing the file after reading it to free up system resources and avoid potential issues with file access in other parts of the program, this is important for maintaining good performance and stability of the program
    return number; // the function returns the generated number for the new entry, this number can then be used as an identifier for that entry in the user's storage
}

vector<service> loadData(account acc) { // loading the existing entries from the user's storage file into a vector of service structs, this allows the program to work with the data in memory and perform operations on it, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient
    ifstream readfile(acc.login + ".csv"); // opening the user's storage file to read the existing entries and load them into a vector of service structs, the file is named after the user's login and has a .csv extension, if the file cannot be opened, the function informs the user and exits the program
    vector<service> data; // creating an empty vector of service structs to store the loaded data from the user's storage file, this vector will be populated with the service data read from the file and can then be used for various operations in the program, such as finding, editing, or deleting entries
    if(readfile.is_open()) {
        string line;
        getline(readfile, line);
        string servNum, serv, servLogin, servPass;
        while(getline(readfile, line)) {
            stringstream ss(line);
            getline(ss, servNum, ',');
            getline(ss, serv, ',');
            getline(ss, servLogin, ',');
            getline(ss, servPass, ',');

            service load; // creating a service struct to store the data for each entry read from the user's storage file, this struct will be populated with the data read from the file and then added to the vector of service structs for further operations in the program
            load.id = stoi(servNum);
            load.service = serv;
            load.serviceLogin = servLogin;
            load.servicePassword = servPass;
            data.push_back(load); // adding the loaded service struct to the vector of service structs, this allows the program to work with the data in memory and perform operations on it, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient
        }
    } else {
        cout << "You have to create account first by using - registration in main folder." << endl;
        readfile.close();
        exit(0);
    }
    readfile.close(); // closing the file after reading it to free up system resources and avoid potential issues with file access in other parts of the program, this is important for maintaining good performance and stability of the program
    return data; // the function returns the vector of service structs loaded with the existing entries from the user's storage file, this vector can then be used for various operations in the program, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient
}

vector<service> updateEditedFile(service findByService, vector<service> Alldata) { // updating the vector of service structs with the edited data for a specific service, this function takes the edited service data and the vector of all service data as arguments, iterates through the vector to find the matching service by its name, and updates the login and password for that service with the new values from the edited service data, this allows the program to keep the in-memory data up to date with the changes made by the user, and then this updated vector can be used to write the changes back to the user's storage file
    for(int i = 0 ; i < Alldata.size() ; i++) {
        if(Alldata[i].service == findByService.service) {
            Alldata[i].serviceLogin = findByService.serviceLogin;
            Alldata[i].servicePassword = findByService.servicePassword;
            break;
        }
    }
    return Alldata; // the function returns the updated vector of service structs with the edited data for the specific service, this updated vector can then be used to write the changes back to the user's storage file, ensuring that the user's data is consistent and up to date with the changes made in memory
}

vector<service> menuForEntryEditor(service findByService, vector<service> Alldata, string cryptoKey) { // providing a menu for the user to edit the login and password for a specific service, this function takes the service data for the service being edited, the vector of all service data, and the cryptoKey as arguments, it displays the current login and password for that service, and then prompts the user to choose whether they want to edit the login, edit the password, or save and exit back to the main menu, based on the user's choice, it calls the appropriate functions to change the login or password, updates the in-memory data with the changes made by the user, and then returns the updated vector of service structs with the edited data for that specific service
    int choice;
    do {
        cout << "What would you like to edit(" << findByService.service << "): "  << endl;
        cout << "Current login: " << findByService.serviceLogin << endl;
        cout << "Current password: " << findByService.servicePassword << endl;
        cout << "1. Login" << endl;
        cout << "2. Password" << endl;
        cout << "3. Save and exit(MainMenu)" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1: { // if the user chooses to edit the login, the function calls the changeLogin function to prompt the user for a new login and update the service data with the new login, then it displays the updated login and password for that service, and breaks out of the switch statement to allow the user to make another choice or save and exit
            system("clear");
            findByService = changeLogin(findByService);
            cout << "New data: " << endl;
            cout << findByService.id << " | Service: " << findByService.service << " | Login: " << findByService.serviceLogin << " | Password: " << findByService.servicePassword << " |" << endl;
            break;
        }
        case 2: { // if the user chooses to edit the password, the function calls the changePass function to prompt the user for a new password and update the service data with the new password, then it displays the updated login and password for that service, and breaks out of the switch statement to allow the user to make another choice or save and exit
            system("clear");
            findByService = changePass(findByService);
            cout << "New data: " << endl;
            cout << findByService.id << " | Service: " << findByService.service << " | Login: " << findByService.serviceLogin << " | Password: " << findByService.servicePassword << " |" << endl;  
            break;
        }
        case 3: { // if the user chooses to save and exit, the function calls the XORencryptDeCryptForThreeData function to encrypt the service name, login, and password for that service using XOR cipher with the cryptoKey generated from the user's password, this is done to ensure that the data is stored in an encrypted form in memory and can be written back to the user's storage file in an encrypted form, then it calls the updateEditedFile function to update the vector of service structs with the edited data for that specific service, and returns the updated vector to be used for writing back to the user's storage file
            XORencryptDeCryptForThreeData(cryptoKey, findByService.service, findByService.serviceLogin, findByService.servicePassword);
            Alldata = updateEditedFile(findByService, Alldata);
            break;
        }
        default:
            cout << "Wrong input";
        }
    } while(choice < 3);
    return Alldata; // the function returns the updated vector of service structs with the edited data for that specific service, this updated vector can then be used to write the changes back to the user's storage file, ensuring that the user's data is consistent and up to date with the changes made in memory
}

void addNewData(account acc, vector<service> &data, string cryptoKey) { // adding a new entry for a service by prompting the user to enter the service name, login, and password for that service, then it creates a new service struct with the entered data, encrypts the service name, login, and password using XOR cipher with the cryptoKey generated from the user's password, adds the new service struct to the vector of service structs in memory, and then this updated vector can be used to write the new entry back to the user's storage file
    int number = numberGenerator(acc);
    string servName, servLogin, servPass;
    cout << "Enter service name: "; cin >> servName;
    cout << "Enter your login: "; cin >> servLogin;
    cout << "Enter your password: "; cin >> servPass;

    service newData; // creating a new service struct to store the data for the new entry, this struct will be populated with the data entered by the user and then added to the vector of service structs for further operations in the program, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient
    newData.id = number;
    newData.service = servName;
    newData.serviceLogin = servLogin;
    newData.servicePassword = servPass;
    XORencryptDeCryptForThreeData(cryptoKey, newData.service, newData.serviceLogin, newData.servicePassword); // encrypting the service name, login, and password for the new entry using XOR cipher with the cryptoKey generated from the user's password, this is done to ensure that the data is stored in an encrypted form in memory and can be written back to the user's storage file in an encrypted form, this helps to protect the user's sensitive data from being easily accessed or read by unauthorized parties
    data.push_back(newData); // adding the new service struct to the vector of service structs in memory, this allows the program to work with the data in memory and perform operations on it, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient
}

void updateFile(account acc, vector<service> data) { // updating the user's storage file with the current data in memory, this function takes the user's account information and the vector of service structs as arguments, it opens the user's storage file for writing, writes the header line for the CSV file, and then iterates through the vector of service structs to write each entry's data to the file in CSV format, this ensures that any changes made to the data in memory, such as adding new entries, editing existing entries, or deleting entries, are saved back to the user's storage file so that they can be loaded again when the user logs in next time
    ofstream update(acc.login + ".csv"); // opening the user's storage file for writing, the file is named after the user's login and has a .csv extension, if the file cannot be opened, the function informs the user and exits the program
    update << "Number,Service,Login,Password" << endl;
    for(auto i : data) {
        update << i.id << "," << i.service << "," << i.serviceLogin << "," << i.servicePassword << endl;
    }
    update.close(); // closing the file after writing to it to free up system resources and ensure that the data is properly saved to the file, this is important for maintaining good performance and stability of the program, as well as ensuring that the user's data is not lost or corrupted due to issues with file access or writing
}

vector<service> deleteEntry(vector<service> &data, string cryptoKey) { // deleting an entry for a service by prompting the user to enter the service name for the entry they want to delete, then it searches for the matching service in the vector of service structs in memory, if it finds a match, it displays the details of that entry and asks the user to confirm if they want to delete it, if the user confirms, it removes that entry from the vector of service structs in memory, and then this updated vector can be used to write the changes back to the user's storage file, ensuring that the deleted entry is removed from the file as well
    bool found = false;
    bool valid = false;
    string userInput;
    cout << "Enter service name to delete: "; cin >> userInput;
    XORencryptDeCryptForOneData(cryptoKey, userInput); // encrypting the user's input for the service name using XOR cipher with the cryptoKey generated from the user's password, this is done to match the encrypted service names stored in the vector of service structs in memory, if the user's input matches an encrypted service name in the vector, it means that the user has entered the correct service name and the function can proceed to find and delete that entry
    for(int i = 0 ; i < data.size() ; i++) {
        if(data[i].service == userInput) { // if the user's input for the service name matches an encrypted service name in the vector of service structs in memory, the function displays the details of that entry and asks the user to confirm if they want to delete it, if the user confirms, it removes that entry from the vector of service structs in memory, and then this updated vector can be used to write the changes back to the user's storage file, ensuring that the deleted entry is removed from the file as well, if it doesn't match, the loop continues until all entries have been checked
            found = true;
            XORencryptDeCryptForThreeData(cryptoKey, data[i].service, data[i].serviceLogin, data[i].servicePassword); // decrypting the service name, login, and password for that entry using XOR cipher with the cryptoKey generated from the user's password, this is done to display the details of that entry in a readable form to the user, as the data is stored in an encrypted form in memory, it needs to be decrypted before it can be displayed to the user, this allows the user to see the details of the entry they are about to delete and confirm that they want to delete it
            cout << data[i].id << " | Service: " << data[i].service << " | Login: " << data[i].serviceLogin << " | Password: " << data[i].servicePassword << " |" << endl;
            int confirm;
            cout << "Are you sure that you want to delete this entry?" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> confirm;
            valid = validInputForDel(confirm, valid); // checking if the user's input for confirming the deletion of the entry is valid, if it is, the function returns true and the deletion process continues, if it isn't, the function returns false and the user is prompted to enter the input again, this ensures that the user can only confirm or cancel the deletion with valid inputs, and prevents accidental deletions due to invalid inputs
            if(valid) {
                if(confirm == 1) {
                    data.erase(data.begin() + i); // removing the entry from the vector of service structs in memory, this allows the program to work with the updated data in memory and perform further operations on it, such as finding, editing, or deleting other entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient, then this updated vector can be used to write the changes back to the user's storage file, ensuring that the deleted entry is removed from the file as well
                    cout << "Entry has been deleted" << endl;
                    break;
                } else {
                    XORencryptDeCryptForThreeData(cryptoKey, data[i].service, data[i].serviceLogin, data[i].servicePassword); 
                    cout << "Go back to main menu" << endl;
                    break;
                }
            }
        }
    }
    return data; // the function returns the updated vector of service structs in memory after the deletion of the entry, this updated vector can then be used to write the changes back to the user's storage file, ensuring that the deleted entry is removed from the file as well, and that the user's data is consistent and up to date with the changes made in memory
}

void showAllData(vector<service> data, string cryptoKey) { // showing all entries for the user by iterating through the vector of service structs in memory, decrypting the service name, login, and password for each entry using XOR cipher with the cryptoKey generated from the user's password, and then displaying the details of each entry in a readable form to the user, this allows the user to see all their stored entries and their details in a clear and organized way
    for(auto& i : data) { // iterating through the vector of service structs in memory, for each entry, it decrypts the service name, login, and password using XOR cipher with the cryptoKey generated from the user's password, this is done to display the details of each entry in a readable form to the user, as the data is stored in an encrypted form in memory, it needs to be decrypted before it can be displayed to the user, this allows the user to see all their stored entries and their details in a clear and organized way
        XORencryptDeCryptForThreeData(cryptoKey, i.service, i.serviceLogin, i.servicePassword);
        cout << i.id << " | Service: " << i.service << " | Login: " << i.serviceLogin << " | Password: " << i.servicePassword << " |" << endl;
    }
}

void changeID(vector<service> &data) { // changing the ID numbers for the entries in the vector of service structs in memory after an entry has been deleted, this function takes the vector of service structs as an argument, it iterates through the vector and updates the ID numbers for each entry to ensure that they are sequential and consistent after an entry has been removed, this is important for maintaining the integrity of the data and ensuring that each entry has a unique identifier that can be used for various operations in the program, such as finding, editing, or deleting entries
    int number = -1;
    for(int i = 0 ; i < data.size() ; i++) {
            number += 1;
            data[i].id = number;
    }
}

void login(account &user, string &cryptoKey) { // handling the user login process by prompting the user to enter their login and password, then it checks if the entered login and password are valid by comparing them with the stored login and hashed password in the base.csv file, if the login is successful, it sets the user's account information and generates the cryptoKey for encrypting and decrypting the service data, if the login fails after a certain number of attempts, it exits the program
    ifstream base("base.csv");
    if(base.is_open()) {
        cout << "DATAbase opened" << endl;
    } else {
        cout << "Something went wrong.";
        exit(0);
    }
    int attemps = 3;
    bool fail = true;
    bool validData = false;
    cout << "Please enter your login and password:" << endl;
    while(attemps > 0 && fail) {
        string login;
        string password;
        string data[2];
        cout << "Remaing attemps: " << attemps << endl;
        cout << "----" << endl;
        cout << "Login: ";
        cin >> login;
        cout << "Password: ";
        cin >> password;
        data[0] = login ; data[1] = password;
        base.clear();
        base.seekg(0);
        validData = validInputForLogin(data);
        if(validData) {
            string line;
            getline(base, line);
            string userID, userLogin, userPassword;
            while(getline(base, line)) {
                stringstream ss(line);
                getline(ss, userID, ',');
                getline(ss, userLogin, ',');
                getline(ss, userPassword, ',');
                if(login == userLogin && hashingpass(password) == userPassword) {
                    fail = false;
                    user.login = userLogin; // setting the user's login in the account struct to the login that was entered and matched in the base.csv file, this allows the program to keep track of which user is currently logged in and access their specific data in the user's storage file, as the storage file is named after the user's login, this is important for ensuring that each user can only access their own data and not the data of other users
                    cryptoKey = hashingpass(password); // generating the cryptoKey for encrypting and decrypting the service data using the hashingpass function to hash the user's password, this ensures that the cryptoKey is derived from the user's password and can be used to securely encrypt and decrypt the service data in memory and in the user's storage file, this helps to protect the user's sensitive data from being easily accessed or read by unauthorized parties, as the cryptoKey is not stored in plain text and is generated from the hashed password
                    base.close();
                    break;
                }
            }
            if(!fail) { // if the login is successful, the function sets the user's account information and generates the cryptoKey for encrypting and decrypting the service data, then it closes the base.csv file and breaks out of the login loop to proceed to the main menu, if the login fails, it continues to prompt the user for their login and password until they either succeed or exhaust their attempts
                cout << "Welcome back " << userLogin << endl;
                base.close();
                break;
            } else { // if the login fails, the function decrements the remaining attempts, clears the input stream to handle any potential issues with invalid input, ignores any remaining input to prevent it from affecting subsequent input, informs the user that the login or password is wrong, and clears the console screen for a cleaner user experience, then it continues to prompt the user for their login and password until they either succeed or exhaust their attempts
                attemps --;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Login or password is wrong." << endl;
                system("clear");
            }
        } else { // if the user's input for login or password contains not allowed characters, the function decrements the remaining attempts, clears the input stream to handle any potential issues with invalid input, ignores any remaining input to prevent it from affecting subsequent input, informs the user that not allowed characters were entered, and clears the console screen for a cleaner user experience, then it continues to prompt the user for their login and password until they either succeed or exhaust their attempts
            attemps --;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Not allowed sings." << endl;
            system("clear");
        }
    }
    if(fail) { // if the login fails after exhausting all attempts, the function informs the user that there are too many failed attempts and exits the program, this is a security measure to prevent unauthorized access to the user's data by limiting the number of login attempts and exiting the program if the attempts are exhausted
        cout << "Too many failed attempts. Exiting." << endl;
        base.close();
        exit(0);
    }
}

void menu(account user, string cryptoKey) { // providing the main menu for the user to interact with their stored data, this function takes the user's account information and the cryptoKey as arguments, it displays a menu with options for adding new entries, showing all entries, finding entries by service, editing entries, deleting entries, and exiting the program, based on the user's choice, it calls the appropriate functions to perform the selected operation on the user's data in memory, and then updates the user's storage file with any changes made to ensure that the data is consistent and up to date
    int choice;
    service findByService; // creating a service struct to store the data for the service that the user is trying to find or edit, this struct will be populated with the data for that service if it is found in the user's storage, and can then be used for various operations in the program, such as displaying the details of that service, editing the login or password for that service, or deleting that service from the user's storage
    vector<service> data; // creating a vector of service structs to store the data for all the entries in the user's storage, this vector will be populated with the data loaded from the user's storage file when the user logs in, and can then be used for various operations in the program, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient
    data = loadData(user); // loading the existing entries from the user's storage file into the vector of service structs in memory, this allows the program to work with the data in memory and perform operations on it, such as finding, editing, or deleting entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient, as the user interacts with their data through the menu options
    bool finish = false;
    do { // displaying the main menu options to the user and prompting them to enter their choice, based on the user's input, it calls the appropriate functions to perform the selected operation on the user's data in memory, and then updates the user's storage file with any changes made to ensure that the data is consistent and up to date, this loop continues until the user chooses to exit the program
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Welcome to IgnatPasswordManager." << endl;
        cout << "----" << endl;
        cout << "1. Add new entry" << endl;
        cout << "2. Show all entries" << endl;
        cout << "3. Find by service" << endl;
        cout << "4. Edit entry" << endl;
        cout << "5. Delete entry" << endl;
        cout << "6. Exit" << endl;
        cin >> choice;
        switch (choice)
        {
            case 1: { // if the user chooses to add a new entry, the function calls the addNewData function to prompt the user for the service name, login, and password for the new entry, adds that entry to the vector of service structs in memory, then it calls the updateFile function to write the changes back to the user's storage file, ensuring that the new entry is saved to the file as well, and informs the user that the new entry has been created, then it breaks out of the switch statement to return to the main menu
                system("clear");
                addNewData(user, data, cryptoKey);
                updateFile(user, data);
                cout << "New entry has created." << endl;
                break;
            }
            case 2: { // if the user chooses to show all entries, the function calls the showAllData function to iterate through the vector of service structs in memory, decrypt the service name, login, and password for each entry using XOR cipher with the cryptoKey generated from the user's password, and then display the details of each entry in a readable form to the user, this allows the user to see all their stored entries and their details in a clear and organized way, then it breaks out of the switch statement to return to the main menu
                system("clear");
                showAllData(data, cryptoKey);
                break;
            }
            case 3: { // if the user chooses to find an entry by service, the function calls the findService function to prompt the user for the service name they want to find, searches for that service in the vector of service structs in memory, if it finds a match, it decrypts the service name, login, and password for that entry using XOR cipher with the cryptoKey generated from the user's password, and then displays the details of that entry in a readable form to the user, this allows the user to quickly find and view the details of a specific entry based on the service name, if it doesn't find a match, it informs the user that the service was not found, then it breaks out of the switch statement to return to the main menu
                system("clear");
                findByService = findService(data, cryptoKey);
                bool ok = checkService(findByService, data);
                if(!ok) { // if the service is found, the function decrypts the service name, login, and password for that entry using XOR cipher with the cryptoKey generated from the user's password, and then displays the details of that entry in a readable form to the user, this allows the user to quickly find and view the details of a specific entry based on the service name, if it isn't found, it informs the user that the service was not found, then it breaks out of the switch statement to return to the main menu
                    XORencryptDeCryptForThreeData(cryptoKey, findByService.service, findByService.serviceLogin, findByService.servicePassword);
                    cout << findByService.id << " | Service: " << findByService.service << " | Login: " << findByService.serviceLogin << " | Password: " << findByService.servicePassword << " |" << endl;
                }
                break;
            }
            case 4: { // if the user chooses to edit an entry, the function calls the findService function to prompt the user for the service name they want to edit, searches for that service in the vector of service structs in memory, if it finds a match, it decrypts the service name, login, and password for that entry using XOR cipher with the cryptoKey generated from the user's password, and then displays the details of that entry in a readable form to the user, this allows the user to quickly find and view the details of a specific entry based on the service name before editing it, then it calls the menuForEntryEditor function to provide a menu for the user to edit the login and password for that specific service, after the user finishes editing and saves their changes, it updates the vector of service structs in memory with the edited data for that specific service, and then calls the updateFile function to write the changes back to the user's storage file, ensuring that the edited entry is updated in the file as well, and informs the user that the entry has been edited, then it breaks out of the switch statement to return to the main menu
                system("clear");
                findByService = findService(data, cryptoKey); // calling the findService function to prompt the user for the service name they want to edit, search for that service in the vector of service structs in memory, if it finds a match, it returns the service data for that entry, if it doesn't find a match, it returns an empty service struct, this allows the user to quickly find and view the details of a specific entry based on the service name before editing it, and then proceed to edit that entry if it is found
                bool ok = checkService(findByService, data); // checking if the service was found in the vector of service structs in memory, if it was found, the function returns true and allows the user to proceed with editing that entry, if it wasn't found, the function returns false and informs the user that the service was not found, this ensures that the user can only edit entries that exist in their storage and prevents errors or issues that could arise from trying to edit a non-existent entry
                if(!ok) { // if the service is found, the function decrypts the service name, login, and password for that entry using XOR cipher with the cryptoKey generated from the user's password, and then displays the details of that entry in a readable form to the user, this allows the user to quickly find and view the details of a specific entry based on the service name before editing it, then it calls the menuForEntryEditor function to provide a menu for the user to edit the login and password for that specific service, after the user finishes editing and saves their changes, it updates the vector of service structs in memory with the edited data for that specific service, and then calls the updateFile function to write the changes back to the user's storage file, ensuring that the edited entry is updated in the file as well, and informs the user that the entry has been edited, then it breaks out of the switch statement to return to the main menu, if it isn't found, it informs the user that the service was not found, then it breaks out of the switch statement to return to the main menu
                    XORencryptDeCryptForThreeData(cryptoKey, findByService.service, findByService.serviceLogin, findByService.servicePassword);
                    data = menuForEntryEditor(findByService, data, cryptoKey); // calling the menuForEntryEditor function to provide a menu for the user to edit the login and password for that specific service, this function takes the service data for the service being edited, the vector of all service data, and the cryptoKey as arguments, it displays the current login and password for that service, and then prompts the user to choose whether they want to edit the login, edit the password, or save and exit back to the main menu, based on the user's choice, it calls the appropriate functions to change the login or password, updates the in-memory data with the changes made by the user, and then returns the updated vector of service structs with the edited data for that specific service, this allows the program to keep the in-memory data up to date with the changes made by the user, and then this updated vector can be used to write the changes back to the user's storage file
                    updateFile(user, data); // calling the updateFile function to write the changes back to the user's storage file, this function takes the user's account information and the vector of service structs as arguments, it opens the user's storage file for writing, writes the header line for the CSV file, and then iterates through the vector of service structs to write each entry's data to the file in CSV format, this ensures that any changes made to the data in memory, such as editing an entry, are saved back to the user's storage file so that they can be loaded again when the user logs in next time, this is important for maintaining consistency between the in-memory data and the stored data in the file
                    cout << "Entry has edited." << endl;
                    break;                  
                } else {
                    break;
                }
            }
            case 5: { // if the user chooses to delete an entry, the function calls the deleteEntry function to prompt the user for the service name of the entry they want to delete, searches for that service in the vector of service structs in memory, if it finds a match, it displays the details of that entry and asks the user to confirm if they want to delete it, if the user confirms, it removes that entry from the vector of service structs in memory, then it calls the changeID function to update the ID numbers for the remaining entries in memory to ensure that they are sequential and consistent after the deletion, and then calls the updateFile function to write the changes back to the user's storage file, ensuring that the deleted entry is removed from the file as well, and informs the user that the entry has been deleted, then it breaks out of the switch statement to return to the main menu
                system("clear");
                data = deleteEntry(data, cryptoKey); // calling the deleteEntry function to prompt the user for the service name of the entry they want to delete, search for that service in the vector of service structs in memory, if it finds a match, it displays the details of that entry and asks the user to confirm if they want to delete it, if the user confirms, it removes that entry from the vector of service structs in memory, then it returns the updated vector after deletion, this allows the program to work with the updated data in memory and perform further operations on it, such as finding, editing, or deleting other entries, without having to read from or write to the file every time, this can improve performance and make the program more efficient, then this updated vector can be used to write the changes back to the user's storage file, ensuring that the deleted entry is removed from the file as well
                changeID(data); // calling the changeID function to update the ID numbers for the remaining entries in memory after an entry has been deleted, this ensures that the ID numbers are sequential and consistent after the deletion, which is important for maintaining the integrity of the data and ensuring that each entry has a unique identifier that can be used for various operations in the program, such as finding, editing, or deleting entries
                updateFile(user, data); // calling the updateFile function to write the changes back to the user's storage file, this function takes the user's account information and the vector of service structs as arguments, it opens the user's storage file for writing, writes the header line for the CSV file, and then iterates through the vector of service structs to write each entry's data to the file in CSV format, this ensures that any changes made to the data in memory, such as deleting an entry, are saved back to the user's storage file so that they can be loaded again when the user logs in next time, this is important for maintaining consistency between the in-memory data and the stored data in the file, and ensuring that the deleted entry is removed from the file as well
                break;
            }
            case 6: { // if the user chooses to exit the program, the function informs the user that they are exiting, and then calls the exit function to terminate the program, this allows the user to safely exit the program when they are done managing their stored data
                cout << "Goodbye!" << endl;
                finish = true;
                exit(0);
            }
            default: { // if the user enters an invalid choice that is not among the menu options, the function informs the user that the input is wrong and prompts them to enter a valid choice, this ensures that the user can only select from the available options and prevents errors or issues that could arise from invalid input, then it breaks out of the switch statement to return to the main menu
                cout << "Wrong input." << endl;
                break;
            }
        }
    } while(!finish);
}

int main() {
    account user;
    string cryptoKey;
    login(user, cryptoKey);
    menu(user, cryptoKey);
}
