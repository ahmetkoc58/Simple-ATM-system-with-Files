#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char CustomerName[20];
    char AccountNumber[10];
    char Password[10];
    int Balance;
} Customer;

typedef struct {
    int Fifties;
    int Hundreds;
    int TwoHundredLiras;
} ATMCase;

void ChangePassword(Customer *customers, int customerIndex) {
    char oldp[50], newp[50], newp2[50];
    int exit = 0;
    while (!exit) {
        printf("Welcome, please enter your current password: ");
        scanf("%s", oldp);
        if (strcmp(customers[customerIndex].Password, oldp) != 0) {
            printf("Wrong password. Try again.\n");
            continue;
        }
        printf("Enter a new password: ");
        scanf("%s", newp);
        printf("Enter new password again: ");
        scanf("%s", newp2);
        if (strcmp(newp, newp2) == 0) {
            strcpy(customers[customerIndex].Password, newp);
            printf("Password change successful.\n");
            exit = 1;
        } else {
            printf("Passwords do not match. Try again.\n");
        }
    }
}

void deposit(Customer *customers, int customerIndex, ATMCase* atmCase) {
    int fifties, hundreds, twoHundreds;
    printf("Please enter number of 50 TL banknotes: ");
    scanf("%d", &fifties);
    printf("Please enter number of 100 TL banknotes: ");
    scanf("%d", &hundreds);
    printf("Please enter number of 200 TL banknotes: ");
    scanf("%d", &twoHundreds);
    int total = (fifties * 50) + (hundreds * 100) + (twoHundreds * 200);
    printf("\nDeposit summary:\n");
    printf("200 TL banknotes: %d\n", twoHundreds);
    printf("100 TL banknotes: %d\n", hundreds);
    printf("50 TL banknotes: %d\n", fifties);
    printf("Total deposit: %d TL\n", total);
    printf("Type 'e' to confirm deposit or 'x' to cancel: ");
    char confirm;
    scanf(" %c", &confirm);
    if (confirm == 'e' || confirm == 'E') {
        atmCase->TwoHundredLiras += twoHundreds;
        atmCase->Hundreds += hundreds;
        atmCase->Fifties += fifties;
        customers[customerIndex].Balance += total;
        printf("Deposit completed successfully.\n");
    } else if (confirm == 'x' || confirm == 'X') {
        printf("Deposit cancelled.\n");
    } else {
        printf("Invalid input. No action taken.\n");
    }
}

void withdraw(Customer *customers, int customerIndex, ATMCase* atmCase) {
    int exit = 0;
    int available = 0;
    while (!exit) {
        int request;
        printf("Enter the amount to be withdrawn: ");
        scanf("%d", &request);
        if (request > customers[customerIndex].Balance) {
            printf("The requested amount must be less than or equal to your balance.\n");
            continue;
        }
        if (request % 50 != 0) {
            available = request - (request % 50);
            printf("Only multiples of 50 can be withdrawn. Adjusted amount: %d TL\n", available);
        } else {
            available = request;
        }
        int t200 = available / 200;
        int remaining = available % 200;
        int t100 = remaining / 100;
        remaining %= 100;
        int t50 = remaining / 50;
        if (t200 > atmCase->TwoHundredLiras || t100 > atmCase->Hundreds || t50 > atmCase->Fifties) {
            printf("ATM does not have enough banknotes for this transaction.\n");
            continue;
        }
        printf("Amount: %d TL → %d x 200TL, %d x 100TL, %d x 50TL\n", available, t200, t100, t50);
        printf("Type 'e' to confirm withdrawal or 'x' to cancel: ");
        char confirm;
        scanf(" %c", &confirm);
        if (confirm == 'e' || confirm == 'E') {
            atmCase->TwoHundredLiras -= t200;
            atmCase->Hundreds -= t100;
            atmCase->Fifties -= t50;
            customers[customerIndex].Balance -= available;
            printf("Withdrawal completed.\n");
        } else {
            printf("Transaction cancelled.\n");
        }
        exit = 1;
    }
}

void transferring(Customer *customers, int customerIndex, ATMCase* atmCase, int customerCount) {
    printf("Transfer Funds to Another Account:\n");
    printf("Recipient Account Number: ");
    char an[10];
    scanf("%s", an);
    printf("Amount: ");
    int amount;
    scanf("%d", &amount);
    if (amount > customers[customerIndex].Balance) {
        printf("Insufficient funds.\n");
        return;
    }
    int found = 0;
    int transferIndex = -1;
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].AccountNumber, an) == 0) {
            found = 1;
            transferIndex = i;
            break;
        }
    }
    if (!found) {
        printf("Account not found.\n");
        return;
    } else {
        customers[transferIndex].Balance += amount;
        customers[customerIndex].Balance -= amount;
        printf("Transfer process is finished successfully.\n");
    }
}

int main() {
    printf("Welcome to the online ATM system. Please first enter your account number,\n");
    printf("followed by your password.\n");
    char an[10];
    char pw[10];
    printf("Account Number: ");
    fgets(an, sizeof(an), stdin);
    an[strcspn(an, "\n")] = '\0';
    printf("Password: ");
    fgets(pw, sizeof(pw), stdin);
    pw[strcspn(pw, "\n")] = '\0';

    Customer Customers[100];
    int customerCount = 0;
    FILE* fptr = fopen("musteri.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    while (fscanf(fptr, "%[^;];%[^;];%[^;];%d\n", Customers[customerCount].CustomerName, 
           Customers[customerCount].AccountNumber, Customers[customerCount].Password, 
           &Customers[customerCount].Balance) == 4) {
        customerCount++;
    }
    fclose(fptr);

    ATMCase atmCase;
    FILE* fptr2 = fopen("kasa.txt", "r");
    if (fptr2 == NULL) {
        printf("Error opening case file.\n");
        return 1;
    }
    fscanf(fptr2, "%d;%d;%d\n", &atmCase.Fifties, &atmCase.Hundreds, &atmCase.TwoHundredLiras);
    fclose(fptr2);

    int CustomerIndex = -1;
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(an, Customers[i].AccountNumber) == 0 && strcmp(pw, Customers[i].Password) == 0) {
            CustomerIndex = i;
            break;
        }
    }
    if (CustomerIndex == -1) {
        printf("Invalid account number or password.\n");
        return 1;
    }
    if (strcmp(Customers[CustomerIndex].Password, "abc123") == 0) {
        ChangePassword(Customers, CustomerIndex);
    }

    printf("------------------------------------\n");
    printf("Dear %s, your account balance is %d TL. You can now choose from the following banking operations:\n", 
           Customers[CustomerIndex].CustomerName, Customers[CustomerIndex].Balance);
    printf("1. Deposid Funds\n");
    printf("2. Withdraw Funds\n");
    printf("3. Transfer Funds to another account\n");
    printf("4. Change password\n");
    printf("Your choice: ");
    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            deposit(Customers, CustomerIndex, &atmCase);
            break;
        case 2:
            withdraw(Customers, CustomerIndex, &atmCase);
            break;
        case 3:
            transferring(Customers, CustomerIndex, &atmCase, customerCount);
            break;
        case 4:
            ChangePassword(Customers, CustomerIndex);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }

    FILE* fp = fopen("musteri.txt", "w");
    for (int i = 0; i < customerCount; i++) {
        fprintf(fp, "%s;%s;%s;%d\n", Customers[i].CustomerName, Customers[i].AccountNumber, Customers[i].Password, Customers[i].Balance);
    }
    fclose(fp);

    FILE* fp2 = fopen("kasa.txt", "w");
    fprintf(fp2, "%d;%d;%d\n", atmCase.Fifties, atmCase.Hundreds, atmCase.TwoHundredLiras);
    fclose(fp2);

    return 0;
}
