#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// Function declarations so teammates can call them easily from main.h or main.c
void trackInstallments(void);
void calculateRemainingDue(void);

int main() {
    int choice;

    // Simple menu just to test this module independently
    while (1) {
        printf("\n--- INSTALLMENT & PAYMENT TRACKING ---\n");
        printf("1. Track/View Installment Status\n");
        printf("2. Calculate Remaining Due\n");
        printf("3. Exit Module\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &choice);

        if (choice == 1) {
            trackInstallments();
        } else if (choice == 2) {
            calculateRemainingDue();
        } else if (choice == 3) {
            printf("Exiting Payment Tracker...\n");
            break;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Function 1: Track Installments and Payment History
void trackInstallments(void) {
    float totalGoldPrice, totalPaid = 0, currentPaid;
    int totalInstallments, paidInstallments;

    printf("\n[Installment Tracker]\n");
    printf("Enter total price of the gold item (BDT): ");
    scanf("%f", &totalGoldPrice);

    printf("Enter total number of installments agreed upon: ");
    scanf("%d", &totalInstallments);

    printf("Enter number of installments already paid: ");
    scanf("%d", &paidInstallments);

    if (paidInstallments > totalInstallments) {
        printf("Error: Paid installments cannot exceed total installments!\n");
        return;
    }

    // Assuming equal payment per installment for simplicity
    float perInstallmentAmount = totalGoldPrice / totalInstallments;
    totalPaid = perInstallmentAmount * paidInstallments;

    printf("\n--- Payment Summary ---");
    printf("\nTotal Price: %.2f BDT", totalGoldPrice);
    printf("\nInstallments Completed: %d / %d", paidInstallments, totalInstallments);
    printf("\nAmount Paid So Far: %.2f BDT", totalPaid);
    printf("\nRemaining Balance: %.2f BDT\n", totalGoldPrice - totalPaid);
}

// Function 2: Calculate Remaining Due with a Fine/Late check option
void calculateRemainingDue(void) {
    float totalAmount, paidAmount, due, fine = 0;
    char hasLateFee;

    printf("\n[Due Calculation]\n");
    printf("Enter total bill amount (BDT): ");
    scanf("%f", &totalAmount);

    printf("Enter total amount paid till now (BDT): ");
    scanf("%f", &paidAmount);

    due = totalAmount - paidAmount;

    if (due < 0) {
        due = 0; // Just in case they paid extra
    }

    printf("Is there any late payment fine? (y/n): ");
    scanf(" %c", &hasLateFee); // Notice the space before %c to catch newline buffer

    if (tolower(hasLateFee) == 'y') {
        float finePercentage;
        printf("Enter fine percentage (e.g., 5 for 5%%): ");
        scanf("%f", &finePercentage);

        fine = (due * finePercentage) / 100.0;
        printf("Late Fine Applied: %.2f BDT\n", fine);
    }

    printf("\n------------------------");
    printf("\nFinal Due Amount (including fine): %.2f BDT\n", due + fine);
}
