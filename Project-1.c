#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100 
#define NAME_LENGTH 50
#define PASSWORD_LENGTH 20
#define ACCOUNT_NUMBER_LENGTH 10

// Struct to represent a bank account
struct Account {  
    char name[NAME_LENGTH]; // Account holder's name
    char password[PASSWORD_LENGTH]; // Account password
    char accountNumber[ACCOUNT_NUMBER_LENGTH]; // Unique account number
    float balance; // Account balance
};

// Array to hold bank accounts and a counter for the number of accounts
struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function declarations
void createAccount();
int loginAccount();
void withdraw(int accountIndex);
void deposit(int accountIndex);
void transfer(int accountIndex);
void viewBalance(int accountIndex);
void editAccount(int accountIndex);
void deleteAccount(int accountIndex);
void loadAccounts();
void saveAccounts();
int isUniqueAccountNumber(const char* accountNumber);
int isValidName(const char* name);

void printMainMenu();
void printAccountMenu();

int main() {
    loadAccounts(); // Load existing accounts from the file
    int choice;
    
    // Main menu loop
    do {
        printMainMenu();
        printf("Select an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                createAccount(); // Create a new account
                break;
            case 2:
                loginAccount(); // Log into an existing account
                break;
            case 3:
                printf("Exiting the program. Thank you for using our banking app!\n"); // Exit the program
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (choice != 3);
    
    saveAccounts(); // Save accounts to the file before exiting
    return 0;
}

void printMainMenu() {
    printf("\n						====================\n");
    printf("      						     Banking App   \n");
    printf("						====================\n");
    printf("						1. Create Account\n");
    printf("						2. Login Account\n");
    printf("						3. Exit\n");
    printf("						====================\n");
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached.\n");
        return; // Prevent creating more accounts than allowed
    }
    struct Account newAccount; // Create a new account instance
    
    printf("Enter your name: ");
    scanf("%s", newAccount.name);
    while (!isValidName(newAccount.name)) {
        printf("Invalid name. Please enter again (no numbers allowed): ");
        scanf("%s", newAccount.name);
    }
    
    printf("Enter a unique account number: ");
    scanf("%s", newAccount.accountNumber);
    while (!isUniqueAccountNumber(newAccount.accountNumber)) {
        printf("Account number already exists. Please enter a unique account number: ");
        scanf("%s", newAccount.accountNumber);
    }
    
    printf("Enter your password (must be more than 6 characters): ");
    scanf("%s", newAccount.password);
    while (strlen(newAccount.password) <= 6) {
        printf("Invalid password. Please enter again (must be more than 6 characters): ");
        scanf("%s", newAccount.password);
    }
    
    newAccount.balance = 0.0f; // Initialize balance to 0
    accounts[accountCount++] = newAccount; // Add the new account to the array
    printf("Account created successfully!\n");
}

int loginAccount() {
    char accountNumber[ACCOUNT_NUMBER_LENGTH];
    char password[PASSWORD_LENGTH];
    
    printf("Enter account number: ");
    scanf("%s", accountNumber);
    printf("Enter password: ");
    scanf("%s", password);
    
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountNumber, accountNumber) == 0 && strcmp(accounts[i].password, password) == 0) {
            printf("Login successful! Welcome, %s.\n", accounts[i].name);
            int option;
            do {
                printAccountMenu();
                printf("Select an option: ");
                scanf("%d", &option);
                switch (option) {
                    case 1:
                        withdraw(i); // Call withdraw function
                        break;
                    case 2:
                        deposit(i); // Call deposit function
                        break;
                    case 3:
                        transfer(i); // Call transfer function
                        break;
                    case 4:
                        viewBalance(i); // Call view balance function
                        break;
                    case 5:
                        editAccount(i); // Call edit account function break;
                    case 6:
                        deleteAccount(i); // Call delete account function
                        return 0; // Exit login session
                    case 7:
                        printf("Logged out successfully.\n");
                        break;
                    default:
                        printf("Invalid option. Please try again.\n");
                }
            } while (option != 7); // Continue until user logs out
            return 0; // Successful login
        }
    }
    printf("Invalid account number or password.\n"); // Invalid login attempt
    return -1; // Login failed
}

void printAccountMenu() {
    printf("\n						--- Account Menu ---\n");
    printf("						1. Withdraw\n");
    printf("						2. Deposit\n");
    printf("						3. Transfer\n");
    printf("						4. View Balance\n");
    printf("						5. Edit Account\n");
    printf("						6. Delete Account\n");
    printf("						7. Logout\n");
    printf("						---------------------\n");
}

void withdraw(int accountIndex) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid amount. Please enter a positive value.\n");
        return; // Invalid withdrawal amount
    }
    if (amount > accounts[accountIndex].balance) {
        printf("Insufficient balance.\n"); // Check for sufficient balance
        return;
    }
    accounts[accountIndex].balance -= amount; // Deduct amount from balance
    printf("Withdrawal successful! New balance: %.2f\n", accounts[accountIndex].balance);
}

void deposit(int accountIndex) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid amount. Please enter a positive value.\n"); // Invalid deposit amount
        return;
    }
    accounts[accountIndex].balance += amount; // Add amount to balance
    printf("Deposit successful! New balance: %.2f\n", accounts[accountIndex].balance);
}

void transfer(int accountIndex) {
    char targetAccountNumber[ACCOUNT_NUMBER_LENGTH];
    float amount;
    printf("Enter target account number: ");
    scanf("%s", targetAccountNumber);
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("Invalid amount. Please enter a positive value.\n"); // Invalid transfer amount
        return;
    }
    if (amount > accounts[accountIndex].balance) {
        printf("Insufficient balance.\n"); // Check for sufficient balance
        return;
    }
    
    // Find the target account and transfer the amount
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountNumber, targetAccountNumber) == 0) {
            accounts[accountIndex].balance -= amount; // Deduct from sender's balance
            accounts[i].balance += amount; // Add to receiver's balance
            printf("Transfer successful! New balance: %.2f\n", accounts[accountIndex].balance);
            return;
        }
    }
    printf("Target account not found.\n"); // Target account does not exist
}

void viewBalance(int accountIndex) {
    printf("Current balance: %.2f\n", accounts[accountIndex].balance); // Display current balance
}

void editAccount(int accountIndex) {
    int option;
    do {
        printf("\n--- Edit Account ---\n");
        printf("1. Change Name\n");
        printf("2. Change Password\n");
        printf("3. Exit\n");
        printf("Select an option: ");
        scanf("%d", &option);
        switch (option) {
            case 1: {
                char newName[NAME_LENGTH];
                printf("Enter new name: ");
                scanf("%s", newName);
                while (!isValidName(newName)) {
                    printf("Invalid name. Please enter again (no numbers allowed): ");
                    scanf("%s", newName);
                }
                strcpy(accounts[accountIndex].name, newName); // Update name
                printf("Name updated successfully!\n");
                break;
            }
            case 2: {
                char newPassword[PASSWORD_LENGTH];
                printf("Enter new password (must be more than 6 characters): ");
                scanf("%s", newPassword);
                while (strlen(newPassword) <= 6) {
                    printf("Invalid password. Please enter again (must be more than 6 characters): ");
                    scanf("%s", newPassword);
                }
                strcpy(accounts[accountIndex].password, newPassword); // Update password
                printf("Password updated successfully!\n");
                break;
            }
            case 3:
                printf("Exiting edit account menu.\n"); // Exit the edit menu
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (option != 3); // Continue until user exits the edit menu
}

void deleteAccount(int accountIndex) {
    // Shift accounts to remove the deleted account
    for (int i = accountIndex; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    accountCount--; // Decrease the account count
    printf("Account deleted successfully!\n");
}

void loadAccounts() {
    FILE *file = fopen("accounts.txt", "rb"); // Open the file for reading
    if (file) {
        fread(&accountCount, sizeof(int), 1, file); // Read the number of accounts
        fread(accounts, sizeof(struct Account), accountCount, file); // Read account data
        fclose(file); // Close the file
    }
}

void saveAccounts() {
    FILE *file = fopen("accounts.txt", "wb"); // Open the file for writing
    if (file) {
        fwrite(&accountCount, sizeof(int), 1, file); // Write the number of accounts
        fwrite(accounts, sizeof(struct Account), accountCount, file); // Write account data
        fclose(file); // Close the file
    }
}

int isUniqueAccountNumber(const char* accountNumber) {
    // Check if the account number is unique
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountNumber, accountNumber) == 0) {
            return 0; // Account number already exists
        }
    }
    return 1; // Account number is unique
}

int isValidName(const char* name) {
    // Validate that the name does not contain numbers
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] >= '0' && name[i] <= '9') {
            return 0; // Invalid name
        }
    }
    return 1; // Valid name
}