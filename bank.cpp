#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <limits>

using namespace std;

// Account class to store account information
class Account {
private:
    int accountNumber;
    string holderName;
    string accountType;
    double balance;
    string pin;
    string creationDate;

public:
    Account() : accountNumber(0), balance(0.0) {}

    Account(int accNum, string name, string type, double bal, string p) {
        accountNumber = accNum;
        holderName = name;
        accountType = type;
        balance = bal;
        pin = p;
        
        // Set creation date
        time_t now = time(0);
        tm* ltm = localtime(&now);
        creationDate = to_string(ltm->tm_mday) + "/" + 
                       to_string(1 + ltm->tm_mon) + "/" + 
                       to_string(1900 + ltm->tm_year);
    }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    string getPin() const { return pin; }
    string getCreationDate() const { return creationDate; }

    // Setters
    void setHolderName(string name) { holderName = name; }
    void setBalance(double bal) { balance = bal; }
    void setPin(string p) { pin = p; }
    void setCreationDate(string date) { creationDate = date; }

    // Deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\n✓ Rs. " << fixed << setprecision(2) << amount << " jama ho gaye.\n";
            cout << "Naya Balance: Rs. " << balance << endl;
        } else {
            cout << "\n✗ Invalid amount!\n";
        }
    }

    // Withdraw money
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "\n✓ Rs. " << fixed << setprecision(2) << amount << " nikale gaye.\n";
            cout << "Bacha hua Balance: Rs. " << balance << endl;
            return true;
        } else if (amount > balance) {
            cout << "\n✗ Insufficient balance!\n";
            return false;
        } else {
            cout << "\n✗ Invalid amount!\n";
            return false;
        }
    }

    // Display account details
    void display() const {
        cout << "\n╔════════════════════════════════════════╗\n";
        cout << "║         ACCOUNT DETAILS                ║\n";
        cout << "╠════════════════════════════════════════╣\n";
        cout << "║ Account Number : " << setw(21) << left << accountNumber << "║\n";
        cout << "║ Holder Name    : " << setw(21) << left << holderName << "║\n";
        cout << "║ Account Type   : " << setw(21) << left << accountType << "║\n";
        cout << "║ Balance        : Rs. " << setw(17) << left << fixed << setprecision(2) << balance << "║\n";
        cout << "║ Opening Date   : " << setw(21) << left << creationDate << "║\n";
        cout << "╚════════════════════════════════════════╝\n";
    }

    // Save account to file
    void saveToFile(ofstream& file) const {
        file << accountNumber << "|" << holderName << "|" << accountType << "|" 
             << balance << "|" << pin << "|" << creationDate << "\n";
    }
};

// Bank class to manage all accounts
class Bank {
private:
    vector<Account> accounts;
    string dataFile = "bank_data.txt";
    int nextAccountNumber;

    void loadFromFile() {
        ifstream file(dataFile);
        if (!file.is_open()) return;

        string line;
        nextAccountNumber = 1001;

        while (getline(file, line)) {
            if (line.empty()) continue;

            int accNum;
            string name, type, pin, date;
            double bal;

            size_t pos = 0;
            size_t nextPos;

            // Parse account number
            nextPos = line.find('|', pos);
            accNum = stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            // Parse holder name
            nextPos = line.find('|', pos);
            name = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;

            // Parse account type
            nextPos = line.find('|', pos);
            type = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;

            // Parse balance
            nextPos = line.find('|', pos);
            bal = stod(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;

            // Parse pin
            nextPos = line.find('|', pos);
            pin = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;

            // Parse date
            date = line.substr(pos);

            Account acc(accNum, name, type, bal, pin);
            acc.setCreationDate(date);
            accounts.push_back(acc);

            if (accNum >= nextAccountNumber) {
                nextAccountNumber = accNum + 1;
            }
        }
        file.close();
    }

    void saveAllToFile() {
        ofstream file(dataFile);
        for (const auto& acc : accounts) {
            acc.saveToFile(file);
        }
        file.close();
    }

    int findAccountIndex(int accNum) {
        for (size_t i = 0; i < accounts.size(); i++) {
            if (accounts[i].getAccountNumber() == accNum) {
                return i;
            }
        }
        return -1;
    }

    bool verifyPin(int index) {
        string enteredPin;
        cout << "Enter PIN: ";
        cin >> enteredPin;
        return accounts[index].getPin() == enteredPin;
    }

public:
    Bank() : nextAccountNumber(1001) {
        loadFromFile();
    }

    ~Bank() {
        saveAllToFile();
    }

    void createAccount() {
        string name, type, pin;
        double initialDeposit;
        int typeChoice;

        cout << "\n═══════ NEW ACCOUNT ═══════\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Full Name: ";
        getline(cin, name);

        cout << "\nAccount Type:\n";
        cout << "1. Savings Account\n";
        cout << "2. Current Account\n";
        cout << "Choose (1/2): ";
        cin >> typeChoice;

        type = (typeChoice == 1) ? "Savings" : "Current";

        double minDeposit = (typeChoice == 1) ? 500.0 : 1000.0;
        cout << "\nMinimum deposit for " << type << " Account: Rs. " << minDeposit << endl;
        cout << "Enter Initial Deposit: Rs. ";
        cin >> initialDeposit;

        if (initialDeposit < minDeposit) {
            cout << "\n✗ Initial deposit kam hai! Account nahi bana.\n";
            return;
        }

        cout << "Set 4-digit PIN: ";
        cin >> pin;

        if (pin.length() != 4) {
            cout << "\n✗ PIN 4 digit ka hona chahiye!\n";
            return;
        }

        Account newAcc(nextAccountNumber, name, type, initialDeposit, pin);
        accounts.push_back(newAcc);

        cout << "\n╔════════════════════════════════════════╗\n";
        cout << "║     ✓ ACCOUNT CREATED SUCCESSFULLY!    ║\n";
        cout << "╠════════════════════════════════════════╣\n";
        cout << "║ Your Account Number: " << setw(18) << left << nextAccountNumber << "║\n";
        cout << "╚════════════════════════════════════════╝\n";
        cout << "\n⚠ Apna Account Number aur PIN yaad rakhein!\n";

        nextAccountNumber++;
        saveAllToFile();
    }

    void depositMoney() {
        int accNum;
        double amount;

        cout << "\n═══════ DEPOSIT MONEY ═══════\n";
        cout << "Enter Account Number: ";
        cin >> accNum;

        int index = findAccountIndex(accNum);
        if (index == -1) {
            cout << "\n✗ Account nahi mila!\n";
            return;
        }

        if (!verifyPin(index)) {
            cout << "\n✗ Galat PIN!\n";
            return;
        }

        cout << "Enter Amount to Deposit: Rs. ";
        cin >> amount;

        accounts[index].deposit(amount);
        saveAllToFile();
    }

    void withdrawMoney() {
        int accNum;
        double amount;

        cout << "\n═══════ WITHDRAW MONEY ═══════\n";
        cout << "Enter Account Number: ";
        cin >> accNum;

        int index = findAccountIndex(accNum);
        if (index == -1) {
            cout << "\n✗ Account nahi mila!\n";
            return;
        }

        if (!verifyPin(index)) {
            cout << "\n✗ Galat PIN!\n";
            return;
        }

        cout << "Enter Amount to Withdraw: Rs. ";
        cin >> amount;

        accounts[index].withdraw(amount);
        saveAllToFile();
    }

    void checkBalance() {
        int accNum;

        cout << "\n═══════ CHECK BALANCE ═══════\n";
        cout << "Enter Account Number: ";
        cin >> accNum;

        int index = findAccountIndex(accNum);
        if (index == -1) {
            cout << "\n✗ Account nahi mila!\n";
            return;
        }

        if (!verifyPin(index)) {
            cout << "\n✗ Galat PIN!\n";
            return;
        }

        accounts[index].display();
    }

    void transferMoney() {
        int fromAcc, toAcc;
        double amount;

        cout << "\n═══════ TRANSFER MONEY ═══════\n";
        cout << "Enter Your Account Number: ";
        cin >> fromAcc;

        int fromIndex = findAccountIndex(fromAcc);
        if (fromIndex == -1) {
            cout << "\n✗ Aapka account nahi mila!\n";
            return;
        }

        if (!verifyPin(fromIndex)) {
            cout << "\n✗ Galat PIN!\n";
            return;
        }

        cout << "Enter Receiver's Account Number: ";
        cin >> toAcc;

        int toIndex = findAccountIndex(toAcc);
        if (toIndex == -1) {
            cout << "\n✗ Receiver ka account nahi mila!\n";
            return;
        }

        if (fromAcc == toAcc) {
            cout << "\n✗ Apne hi account mein transfer nahi ho sakta!\n";
            return;
        }

        cout << "Enter Amount to Transfer: Rs. ";
        cin >> amount;

        if (amount > accounts[fromIndex].getBalance()) {
            cout << "\n✗ Insufficient balance!\n";
            return;
        }

        if (amount <= 0) {
            cout << "\n✗ Invalid amount!\n";
            return;
        }

        accounts[fromIndex].setBalance(accounts[fromIndex].getBalance() - amount);
        accounts[toIndex].setBalance(accounts[toIndex].getBalance() + amount);

        cout << "\n╔════════════════════════════════════════╗\n";
        cout << "║     ✓ TRANSFER SUCCESSFUL!             ║\n";
        cout << "╠════════════════════════════════════════╣\n";
        cout << "║ Amount Sent   : Rs. " << setw(17) << left << fixed << setprecision(2) << amount << "║\n";
        cout << "║ To Account    : " << setw(21) << left << toAcc << "║\n";
        cout << "║ Your Balance  : Rs. " << setw(17) << left << accounts[fromIndex].getBalance() << "║\n";
        cout << "╚════════════════════════════════════════╝\n";

        saveAllToFile();
    }

    void changePin() {
        int accNum;
        string newPin;

        cout << "\n═══════ CHANGE PIN ═══════\n";
        cout << "Enter Account Number: ";
        cin >> accNum;

        int index = findAccountIndex(accNum);
        if (index == -1) {
            cout << "\n✗ Account nahi mila!\n";
            return;
        }

        cout << "Enter Current PIN: ";
        string oldPin;
        cin >> oldPin;

        if (accounts[index].getPin() != oldPin) {
            cout << "\n✗ Galat PIN!\n";
            return;
        }

        cout << "Enter New 4-digit PIN: ";
        cin >> newPin;

        if (newPin.length() != 4) {
            cout << "\n✗ PIN 4 digit ka hona chahiye!\n";
            return;
        }

        accounts[index].setPin(newPin);
        cout << "\n✓ PIN successfully change ho gaya!\n";
        saveAllToFile();
    }

    void closeAccount() {
        int accNum;

        cout << "\n═══════ CLOSE ACCOUNT ═══════\n";
        cout << "Enter Account Number: ";
        cin >> accNum;

        int index = findAccountIndex(accNum);
        if (index == -1) {
            cout << "\n✗ Account nahi mila!\n";
            return;
        }

        if (!verifyPin(index)) {
            cout << "\n✗ Galat PIN!\n";
            return;
        }

        cout << "\nAccount Balance: Rs. " << fixed << setprecision(2) 
             << accounts[index].getBalance() << endl;
        cout << "Kya aap sure hain? (y/n): ";
        
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            accounts.erase(accounts.begin() + index);
            cout << "\n✓ Account successfully close ho gaya!\n";
            saveAllToFile();
        } else {
            cout << "\nAccount close nahi hua.\n";
        }
    }

    void displayAllAccounts() {
        if (accounts.empty()) {
            cout << "\nKoi account nahi hai!\n";
            return;
        }

        cout << "\n══════════════════════════════════════════════════════════════════════════\n";
        cout << "                           ALL ACCOUNTS LIST\n";
        cout << "══════════════════════════════════════════════════════════════════════════\n";
        cout << left << setw(12) << "Acc No." << setw(25) << "Holder Name" 
             << setw(12) << "Type" << setw(15) << "Balance" << "Date\n";
        cout << "──────────────────────────────────────────────────────────────────────────\n";

        for (const auto& acc : accounts) {
            cout << left << setw(12) << acc.getAccountNumber()
                 << setw(25) << acc.getHolderName()
                 << setw(12) << acc.getAccountType()
                 << "Rs. " << setw(11) << fixed << setprecision(2) << acc.getBalance()
                 << acc.getCreationDate() << "\n";
        }
        cout << "══════════════════════════════════════════════════════════════════════════\n";
        cout << "Total Accounts: " << accounts.size() << endl;
    }
};

void displayMenu() {
    cout << "\n";
    cout << "╔════════════════════════════════════════╗\n";
    cout << "║      BANK MANAGEMENT SYSTEM            ║\n";
    cout << "╠════════════════════════════════════════╣\n";
    cout << "║  1. Create New Account                 ║\n";
    cout << "║  2. Deposit Money                      ║\n";
    cout << "║  3. Withdraw Money                     ║\n";
    cout << "║  4. Check Balance                      ║\n";
    cout << "║  5. Transfer Money                     ║\n";
    cout << "║  6. Change PIN                         ║\n";
    cout << "║  7. Close Account                      ║\n";
    cout << "║  8. View All Accounts (Admin)          ║\n";
    cout << "║  9. Exit                               ║\n";
    cout << "╚════════════════════════════════════════╝\n";
    cout << "Enter your choice: ";
}

int main() {
    Bank bank;
    int choice;

    cout << "\n★★★ Welcome to Bank Management System ★★★\n";

    do {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n✗ Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2:
                bank.depositMoney();
                break;
            case 3:
                bank.withdrawMoney();
                break;
            case 4:
                bank.checkBalance();
                break;
            case 5:
                bank.transferMoney();
                break;
            case 6:
                bank.changePin();
                break;
            case 7:
                bank.closeAccount();
                break;
            case 8:
                bank.displayAllAccounts();
                break;
            case 9:
                cout << "\n★★★ Thank you for using our Bank! ★★★\n";
                cout << "Goodbye!\n\n";
                break;
            default:
                cout << "\n✗ Invalid choice! 1-9 choose karein.\n";
        }
    } while (choice != 9);

    return 0;
}
