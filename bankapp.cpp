#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <cctype>

using namespace std;

int accountId = 1;
int clientId = 1;
int transactionId = 1;





// ==================== VALIDATORS ====================//


bool isUpperCase(char c) {
    return isupper(static_cast<unsigned char>(c));
}

bool isLowerCase(char c) {
    return islower(static_cast<unsigned char>(c));
}

bool isDigit(char c) {
    return isdigit(static_cast<unsigned char>(c));
}

bool isSpecialCharacter(char c) {
    return !isalnum(static_cast<unsigned char>(c));
}

bool isPasswordValid(string password) {
    if (password.length() < 8) {
        return false;
    }

    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasDigit = false;
    bool hasSpecialCharacter = false;

    for (char c : password) {
        if (isUpperCase(c)) {
            hasUpperCase = true;
        }
        else if (isLowerCase(c)) {
            hasLowerCase = true;
        }
        else if (isDigit(c)) {
            hasDigit = true;
        }
        else if (isSpecialCharacter(c)) {
            hasSpecialCharacter = true;
        }
    }

    return hasUpperCase && hasLowerCase && hasDigit && hasSpecialCharacter;
}

bool isValidEmail(string& email) {
    // Check if the email is empty
    if (email.empty()) {
        return false;
    }

    // Check if the first character is not '@' or '.'
    if (email[0] == '@' || email[0] == '.') {
        return false;
    }

    // Check if the last character is not '@' or '.'
    if (email.back() == '@' || email.back() == '.') {
        return false;
    }

    // Check for the presence of '@' character
    size_t atSymbolPos = email.find('@');
    if (atSymbolPos == std::string::npos) {
        return false;
    }

    // Check for the presence of '.' character after '@'
    size_t dotAfterAtPos = email.find('.', atSymbolPos);
    if (dotAfterAtPos == std::string::npos) {
        return false;
    }

    // Check if there's at least one character between '@' and '.'
    if (dotAfterAtPos - atSymbolPos == 1) {
        return false;
    }

    // Check for consecutive dots
    if (email.find("..") != std::string::npos) {
        return false;
    }

    // Check for consecutive at symbols
    if (email.find("@@") != std::string::npos) {
        return false;
    }

    // Check for consecutive dots before '@'
    if (atSymbolPos > 0 && email[atSymbolPos - 1] == '.') {
        return false;
    }

    // Check for consecutive dots after '@'
    if (dotAfterAtPos < email.size() - 1 && email[dotAfterAtPos + 1] == '.') {
        return false;
    }

    return true;
}

bool isValidLebanesePhoneNumber(const std::string& phoneNumber) {
    // Check if the phone number is empty
    if (phoneNumber.empty()) {
        return false;
    }

    // Remove spaces and dashes from the phone number
    std::string cleanedPhoneNumber;
    for (char ch : phoneNumber) {
        if (std::isdigit(ch)) {
            cleanedPhoneNumber += ch;
        }
    }
    // dont enter +961
    if (cleanedPhoneNumber.size() != 8) {
        return false;
    }

    // Check if all characters are digits
    if (cleanedPhoneNumber.find_first_not_of("0123456789") != string::npos) {
        return false;
    }

    return true;
}
// =========================  PASSWORD CONF =========================/

string hashPassword(string password) {
    hash<string> hashFunction;

    // Convert the hash value to a string
    size_t hashedValue = hashFunction(password);
    string hashedPasswordGuess = to_string(hashedValue);
    return hashedPasswordGuess;


}

bool comparePasswords(string& hashedPassword, string& newpass) {
    return hashedPassword == hashPassword(newpass);
}

// ====================== Define ALL struct ====================== //
struct date
{
    int day;
    int month;
    int year;
};

struct account
{
    int numId;
    int clientId;
    string type;
    double balance;
    date creation_account;
    vector<double> transactions;
    int nbTransactions = 0;
};

struct client
{
    int id;
    string fname;
    string lname;
    string password;
    string email;
    string phone;
    account* c;
};

// ====================== Define Helper ====================== //
void setTransactionId()
{
    ifstream transactionsFile;
    transactionsFile.open("transactions.csv");
    string line;
    while (getline(transactionsFile, line))
    {
        transactionId = stoi(line.substr(0, line.find(",")));
    }
    transactionsFile.close();
}
date getDate(string str)
{
    date d;
    d.day = stoi(str.substr(0, str.find(":")));
    str.erase(0, str.find(":") + 1);
    d.month = stoi(str.substr(0, str.find(":")));
    str.erase(0, str.find(":") + 1);
    d.year = stoi(str.substr(0, str.find(":")));
    str.erase(0, str.find(":") + 1);
    return d;
}

// ====================== Define All File Writing Functions ====================== //

void writeTransactionsToFile(const account& a, char operation)
{
    ofstream transactionsFile;
    transactionsFile.open("transactions.csv", std::ios::app); // Use std::ios::app to append to the file

    if (!transactionsFile.is_open()) {
        std::cerr << "Error opening transactions.csv" << std::endl;
        return;
    }

    for (int i = 0; i < 1; i++)
    {
        transactionsFile << transactionId << "," << a.numId << "," << operation << "," << a.transactions[i] << std::endl;
    }

    transactionsFile.close();
}
void insertAccountintoFile(account a)
{
    ofstream file;
    file.open("accounts.csv", ios::app); // Open the file in append mode
    file << a.numId << "," << a.clientId << "," << a.type << "," << a.balance << "," << a.creation_account.day << "/" << a.creation_account.month << "/" << a.creation_account.year << endl;
    file.close();
}
void insertClientintoFile(client c)
{
    ofstream file;
    file.open("clients.csv", ios::app); // Open the file in append mode
    file << c.id << "," << c.fname << "," << c.lname << "," << c.password << "," << c.email << "," << c.phone << endl;
    file.close();
}

// ====================== Define All File Reading Functions ====================== //

vector<client> readAllClientsfromFile()
{
    vector<client> clients; // Create an empty vector to store the clients
    ifstream file;
    file.open("clients.csv");
    string line;
    while (getline(file, line))
    {
        client c;
        c.id = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        c.fname = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        c.lname = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        c.password = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        c.email = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        c.phone = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        clients.push_back(c); // Add the client to the vector
        clientId = c.id + 1;
    }
    file.close();
    return clients;
}

vector<double> readTransactionsFromFile(int id)
{
    vector<double> transactions;
    ifstream transactionsFile;
    transactionsFile.open("transactions.csv");
    string line;
    while (getline(transactionsFile, line))
    {
        int aid = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        if (aid == id)
        {
            transactions.push_back(stod(line.substr(0, line.find(","))));
        }
    }
    transactionsFile.close();
    return transactions;
}

vector<client> clients = readAllClientsfromFile();
vector<account> readAllAccountsfromFile()
{
    vector<account> accounts; // Create an empty vector to store the accounts
    ifstream file;
    file.open("accounts.csv");
    string line;
    while (getline(file, line))
    {
        account a;
        a.numId = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        a.clientId = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        a.type = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        a.balance = stod(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        a.creation_account = getDate(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        a.transactions = readTransactionsFromFile(a.numId);
        a.nbTransactions = a.transactions.size();
        accounts.push_back(a); // Add the account to the vector
        accountId = a.numId + 1;
        for (client& client : clients) {
            if (a.clientId == client.id) {
                client.c = new account(a);

                break;
            }
        }
    }
    file.close();

    return accounts;
}
// ================== STATIC VAR =====================  //


vector<account> accounts = readAllAccountsfromFile();

// ====================== Define All Update Functions ====================== //

void updateClientFile(client c)
{
    ifstream file;
    file.open("clients.csv");
    ofstream temp;
    temp.open("temp.csv");
    string line;
    while (getline(file, line))
    {
        int id = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        if (id == c.id)
        {
            temp << c.id << "," << c.fname << "," << c.lname << "," << c.password << "," << c.email << "," << c.phone << endl;
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].id == c.id)
                {
                    clients[i] = c; // Update the client in the vector
                    break;
                }
            }
        }
        else
        {
            temp << line << endl;
        }
    }
    file.close();
    temp.close();
    remove("clients.csv");
    rename("temp.csv", "clients.csv");
}

void updateAccountFile(account a)
{
    ifstream file;
    file.open("accounts.csv");
    ofstream temp;
    temp.open("temp.csv");
    string line;
    while (getline(file, line))
    {
        cout << line << "\n";
        int id = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        if (id == a.numId)
        {
            temp << a.numId << "," << a.clientId << "," << a.type << "," << a.balance << "," << a.creation_account.day << "/" << a.creation_account.month << "/" << a.creation_account.year << endl;
            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].numId == a.numId)
                {
                    accounts[i] = a; // Update the account in the vector
                    break;
                }
            }
        }
        else
        {
            temp << id << "," << line << endl;
        }
    }
    file.close();
    temp.close();
    remove("accounts.csv");
    rename("temp.csv", "accounts.csv");
}

// ====================== Define All Delete Functions ====================== //

void deleteClientfromFile(int id)
{
    ifstream file;
    file.open("clients.csv");
    ofstream temp;
    temp.open("temp.csv");
    string line;
    while (getline(file, line))
    {
        int cid = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        if (cid != id)
        {
            temp << line << endl;
        }
        else
        {
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].id == id)
                {
                    clients.erase(clients.begin() + i);
                    break;
                }
            }
        }
    }
    file.close();
    temp.close();
    remove("clients.csv");
    rename("temp.csv", "clients.csv");
}

void deleteAccountfromFile(int id)
{
    ifstream file;
    file.open("accounts.csv");
    ofstream temp;
    temp.open("temp.csv");
    string line;
    while (getline(file, line))
    {
        int aid = stoi(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        if (aid != id)
        {
            temp << line << endl;
        }
        else
        {
            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].numId == id)
                {
                    accounts.erase(accounts.begin() + i);
                    break;
                }
            }
        }
    }
    file.close();
    temp.close();
    remove("accounts.csv");
    rename("temp.csv", "accounts.csv");
}


// CLIETN //

client* getClient(int id) {
    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].id == id)
        {
            return &clients[i];
        }
    }

    return NULL;
}
// ====================== Define Account ====================== //



account getAccount(int id) {
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].clientId == id)
        {
            return accounts[i];
        }
    }
    account acc;
    acc.numId = -1;
    return acc;
}

// create account
void createAccount(int id)
{
    account a;
    a.numId = accountId;
    a.clientId = id;
    cout << "Enter the type of the account: ";
    cin >> a.type;
    cout << "Enter the balance of the account: ";
    cin >> a.balance;
    time_t now;
    time(&now);

    struct tm localTime;
    localtime_s(&localTime, &now);
    // Extract day, month, and year
    a.creation_account.day = localTime.tm_mday;
    a.creation_account.month = localTime.tm_mon + 1;
    a.creation_account.year = localTime.tm_year + 1900;

   
    for (client& client : clients) {
        if (a.clientId == client.id) {
            client.c = new account(a);
            break;
        }
    }
    insertAccountintoFile(a);
    
}

// update account
void updateAccount(int id)
{
    int choice;
    cout << "1- Update the type of the account" << endl;
    cout << "2- Update the balance of the account" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        string type;
        cout << "Enter the new type of the account: ";
        cin >> type;
        for (int i = 0; i < accounts.size(); i++)
        {
            if (accounts[i].numId == id)
            {
                accounts[i].type = type;
                updateAccountFile(accounts[i]);
                break;
            }
        }
        break;
    }
    case 2:
    {
        double balance;
        cout << "Enter the new balance of the account: ";
        cin >> balance;
        for (int i = 0; i < accounts.size(); i++)
        {
            if (accounts[i].numId == id)
            {
                accounts[i].balance = balance;
                updateAccountFile(accounts[i]);
                break;
            }
        }
        break;
    }
    default:
    {
        cout << "Wrong choice" << endl;
        updateAccount(id)
            ;
        break;
    }
    }
}

// ====================== Define All CLIENT Functions ====================== //
void updateClient(int id)
{
    int choice;
    cout << "1- Update the first name of the client" << endl;
    cout << "2- Update the last name of the client" << endl;
    cout << "3- Update the password of the client" << endl;
    cout << "4- Update the email of the client" << endl;
    cout << "5- Update the phone number of the client" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        string fname;
        cout << "Enter the new first name of the client: ";
        cin >> fname;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].id == id)
            {
                clients[i].fname = fname;
                updateClientFile(clients[i]);
                break;
            }
        }
        break;
    }
    case 2:
    {
        string lname;
        cout << "Enter the new last name of the client: ";
        cin >> lname;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].id == id)
            {
                clients[i].lname = lname;
                updateClientFile(clients[i]);
                break;
            }
        }
        break;
    }
    case 3:
    {
        string password;
        cout << "Enter the new password of the client: ";
        cin >> password;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].id == id)
            {
                clients[i].password = password;
                updateClientFile(clients[i]);
                break;
            }
        }
        break;
    }
    case 4:
    {
        string email;
        cout << "Enter the new email of the client: ";
        cin >> email;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].id == id)
            {
                clients[i].email = email;
                updateClientFile(clients[i]);
                break;
            }
        }
        break;
    }
    case 5:
    {
        string phone;
        cout << "Enter the new phone number of the client: ";
        cin >> phone;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].id == id)
            {
                clients[i].phone = phone;
                updateClientFile(clients[i]);
                break;
            }
        }
    }
    }
}
// ====================== Define All AUTH ====================== //

int login()
{
    string email, password;
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your password: ";
    cin >> password;
    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].email == email && comparePasswords(clients[i].password, password))
        {
            return clients[i].id;
        }
    }
    return -1;
}

int createClient() {
    client c;
    c.id = clientId;

    cout << "Enter your first name: ";
    cin >> c.fname;

    cout << "Enter your last name: ";
    cin >> c.lname;

    do {
        cout << "Enter your email: ";
        cin >> c.email;
        if (!isValidEmail(c.email)) {
            cout << "Invalid email. Please enter a valid email." << endl;
        }
    } while (!isValidEmail(c.email));


    // Check if the email already exists in the vector
    for (const auto& existingClient : clients) {
        if (existingClient.email == c.email) {
            cout << "Email already exists." << endl;
            return -1;
        }
    }

    do {
        cout << "Enter your password: ";
        cin >> c.password;
        if (!isPasswordValid(c.password)) {
            cout << "Invalid Password. Please enter a valid Password." << endl;
        }
        // Add password validation logic if needed
    } while (!isPasswordValid(c.password));

    do {
        cout << "Enter your phone number: ";
        cin >> c.phone;
        if (!isValidLebanesePhoneNumber(c.phone)) {
            cout << "Invalid phone number. Please enter a valid Lebanese phone number. It Should Only be the digits, dont start with +961." << endl;
        }
    } while (!isValidLebanesePhoneNumber(c.phone));


    c.password = hashPassword(c.password);
    insertClientintoFile(c);
    clients.push_back(c);
    clientId++;
    return c.id;
}

// ====================== Define All Admin Functions ====================== //

void mainCreateClient()
{
    int id = createClient();
    if (id != -1)
    {
        cout << "Client created successfully." << endl;
    }
    else
    {
        cout << "Client not created." << endl;
    }
}

void mainUpdateAccount(int id)
{
    updateAccount(id)
        ;
    return;
}
void mainDeleteAccount(int id)
{
    deleteAccountfromFile(id)
        ;
    return;
}
void mainDeleteClient(int id)
{
    deleteClientfromFile(id)
        ;
    return;
}
void mainUpdateClient(int id)
{
    updateClient(id)
        ;
    return;
}
void adminOptions()
{
    int id;
    int choice;
    cout << "1- Create Client" << endl;
    cout << "2- Update Client" << endl;
    cout << "3- Update Account" << endl;
    cout << "4- Delete Client" << endl;
    cout << "5- Delete Account" << endl;
    cout << "6- Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        mainCreateClient();
        break;
    }
    case 2:
    {
        cout << "Enter the id of the client:";
        cin >> id;
        mainUpdateClient(id)
            ;
        break;
    }
    case 3:
    {
        cout << "Enter the id of the account:";
        cin >> id;
        mainUpdateAccount(id)
            ;
        break;
    }
    case 4:
    {
        cout << "Enter the id of the client:";
        cin >> id;
        mainDeleteClient(id)
            ;
        break;
    }
    case 5:
    {
        cout << "Enter the id of the account:";
        cin >> id;
        mainDeleteAccount(id)
            ;
        break;
    }
    case 6:
    {
        exit(0);
        break;
    }
    default:
    {
        cout << "Wrong choice" << endl;
        return adminOptions();
        break;
    }
    }
    return adminOptions();
}

// ============================ DEFINE ALL USER FUNCTION ===================================== //

void mainCreateAccount(int id)
{
    createAccount(id);
    return;
}
void withdraw(account acc) {

    int amount;
    cout << "How Much Money Are You Withdrawing: ";
    cin >> amount;

    if (acc.balance < amount) {
        cout << "Insuficient Money";
        return;
    }
    else {
        acc.balance -= amount;
        acc.transactions.push_back(amount);
        updateAccountFile(acc);
        writeTransactionsToFile(acc, '-');
    }

}
void transfer(account acc) {
    int amount, id;
    cout << "How Much Money Are You Transfering: ";
    cin >> amount;
    cout << "Enter The ID of the user you're transfering for: ";
    cin >> id;

    client* cl = getClient(id);
    account secAccount = *cl->c;
    if (secAccount.numId == NULL) {
        cout << "User Doesn't Exist";
        return;
    }
    if (acc.balance < amount) {
        cout << "Insuficient Money";
        return;
    }
    else {
        acc.balance -= amount;
        secAccount.balance += amount;
        updateAccountFile(acc);
        acc.transactions.push_back(amount);
        secAccount.transactions.push_back(amount);
        writeTransactionsToFile(acc, '-');
        updateAccountFile(secAccount);
        writeTransactionsToFile(secAccount, '+');
    }
}
void loggedUser(int id) {

    int choice;
    cout << "1- Create Account" << endl;
    cout << "2- Withdraw" << endl;
    cout << "3- Transfer" << endl;
    cout << "4- Logout" << endl;
    cout << "5- Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    client* cl = getClient(id);
    account* acc;
    switch (choice)
    {
    case 1:
    {
        mainCreateAccount(id);

        break;
    }
    case 2:
    {
        acc = cl->c;
        withdraw(*acc);
        break;
    }
    case 3:
    {
        acc = cl->c;
        transfer(*acc);
        break;
    }
    case 4:
    {
        return;
    }
    case 5:
    {
        exit(0);
    }
    default:
    {
        cout << "Wrong choice" << endl;
        return loggedUser(id);
        
    }
    }
    
}
void clientInterface() {
    int id;
    int choice;
    cout << "1- Login" << endl;
    cout << "2- Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        id = login();
        if (id != -1) {
            loggedUser(id);
        }
        break;
    }
    case 2:
    {
        cout << "Enter the id of the client:";
        cin >> id;
        mainUpdateClient(id)
            ;
        break;
    }
    default:
    {
        cout << "Wrong choice" << endl;
        return clientInterface();
        
    }
    }
    
}


// ====================== DEFINE MAIN ====================== //

// welcome to bank and switch case
void welcome()
{
    int choice;
    cout << "Welcome to our bank" << endl;
    cout << "1- Admin" << endl;
    cout << "2- User" << endl;
    cout << "3- Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        adminOptions();
        break;
    }
    case 2:
    {
        clientInterface();
        break;
    }
    case 3:
    {
        exit(0);
        break;
    }
    default:
    {
        cout << "Wrong choice" << endl;
        welcome();
       
    }
    }
}

// ====================== Define All Static ====================== //

int main()
{

    welcome();
}
