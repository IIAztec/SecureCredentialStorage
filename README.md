# Password Manager / Secure Credential Storage

Console application that securely stores and manages your passwords for different services.  
Users can register, log in, add, view, edit, and delete credentials. All sensitive data is encrypted using XOR cipher with a key derived from the master password.

## Features

### For Users:
- **Registration** (via separate program `registration.cpp`)
- Login with master password authentication
- Add new entries (service name, login, password)
- View all stored entries (decrypted on the fly)
- Find entry by service name
- Edit login or password for a specific service
- Delete entry
- All data is encrypted before saving to disk

## Security
- **Master password is never stored in plain text!**
- Master password is hashed using the **djb2 algorithm** before storage
- During login, the entered password is hashed and compared to the stored hash
- All service credentials are encrypted using **XOR cipher** with a key derived from the master password
- Encryption and decryption use the same function due to XOR properties
- Each user has a separate encrypted CSV file named `login.csv`

## Technologies
- **Programming Language:** C++
- **Libraries used:**
  - `iostream` – input/output operations
  - `fstream` – file handling
  - `string` – string manipulation
  - `vector` – dynamic array for storing entries
  - `sstream` – string streams
  - `cstdio` – for `remove` and `rename` operations
  - `iomanip` – output formatting (for hash conversion to hex)

## Data Storage
All data is stored in **CSV files** with encryption:

- `base.csv` – user credentials  
  (ID, login, **master password hash**)
- `{login}.csv` – per‑user encrypted credential storage  
  (Number, Service, Login, Password) – each field is XOR‑encrypted

## System Requirements
- C++11 compatible compiler

## Compilation and Execution

### Main Password Manager Program (`passwordManager.cpp`)
```bash
# Compile
g++ -std=c++11 passwordManager.cpp -o passwordManager

# Run (Linux/macOS)
./passwordManager

# Run (Windows)
passwordManager.exe
```
### Registration Tool (`registration.cpp`)
*This separate program creates a new **user account**. It must be run before using the manager for the first time.*
*During registration, the user provides a **login** and a **master password**. The **password** is **hashed** and stored in base.csv.*
*An empty encrypted storage file is created for the new user.*
```bash
# Compile
g++ -std=c++11 registration.cpp -o registration

# Run (Linux/macOS)
./registration

# Run (Windows)
registration.exe
```
## Project Structure
```bash
passwordManager.cpp   # Main password manager logic
registration.cpp      # User registration tool
base.csv              # User database (login + hashed master password)
{login}.csv           # Per‑user encrypted credential files
README.md             # Documentation
passwordManager       # Compiled executable (Linux/macOS)
registration          # Compiled executable (Linux/macOS)
passwordManager.exe   # Compiled executable (Windows)
registration.exe      # Compiled executable (Windows)
```
## Author

**Ignat Danilin**
- GitHub: [@IIAztec](https://github.com/IIAztec)
- Email: danilin.ignat@gmail.com

*Student at SRH University Stuttgart*
