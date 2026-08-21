#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// Function Declarations for Feature 1: Installment & Payment Tracking
void trackInstallments(void);
void calculateRemainingDue(void);

// Function Declarations for Feature 2: Gold Purity Converter & Valuation
void convertKaratToPercentage(void);
void calculatePureGoldContent(void);

// Function Declarations for Feature 3: Low Stock Alert System
void checkSingleItemStock(void);
void checkMultipleItemsStock(void);

int main() {
    int mainChoice, subChoice;

    while (1) {
        printf("\n=========================================\n");
        printf("      GOLD MANAGEMENT SYSTEM (MODULE)    \n");
        printf("=========================================\n");
        printf("1. Installment & Payment Tracking\n");
        printf("2. Gold Purity Converter & Valuation\n");
        printf("3. Low Stock Alert System\n");
        printf("4. Exit Program\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &mainChoice);

        if (mainChoice == 1) {
            // Sub-menu for Feature 1
            printf("\n--- INSTALLMENT & PAYMENT TRACKING ---\n");
            printf("1. Track Installment Status\n");
            printf("2. Calculate Remaining Due & Fine\n");
            printf("Enter sub-choice (1-2): ");
            scanf("%d", &subChoice);

            if (subChoice == 1) {
                trackInstallments();
            } else if (subChoice == 2) {
                calculateRemainingDue();
            } else {
                printf("Invalid sub-choice!\n");
            }

        } else if (mainChoice == 2) {
            // Sub-menu for Feature 2
            printf("\n--- GOLD PURITY CONVERTER & VALUATION ---\n");
            printf("1. Convert Karat to Purity Percentage\n");
            printf("2. Calculate Pure Gold Weight in Item\n");
            printf("Enter sub-choice (1-2): ");
            scanf("%d", &subChoice);

            if (subChoice == 1) {
                convertKaratToPercentage();
            } else if (subChoice == 2) {
                calculatePureGoldContent();
            } else {
                printf("Invalid sub-choice!\n");
            }

        } else if (mainChoice == 3) {
            // Sub-menu for Feature 3
            printf("\n--- LOW STOCK ALERT SYSTEM ---\n");
            printf("1. Check Single Item Stock\n");
            printf("2. Check Multiple Items Stock (Batch)\n");
            printf("Enter sub-choice (1-2): ");
            scanf("%d", &subChoice);

            if (subChoice == 1) {
                checkSingleItemStock();
            } else if (subChoice == 2) {
                checkMultipleItemsStock();
            } else {
                printf("Invalid sub-choice!\n");
            }

        } else if (mainChoice == 4) {
            printf("\nThank you for using the Gold Management System. Goodbye!\n");
            break;
        } else {
            printf("\nInvalid main choice! Please enter a number between 1 and 4.\n");
        }
    }

    return 0;
}

// ==========================================
// FEATURE 1 FUNCTIONS: Installment & Payment
// ==========================================

void trackInstallments(void) {
    float totalGoldPrice, totalPaid = 0;
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

    float perInstallmentAmount = totalGoldPrice / totalInstallments;
    totalPaid = perInstallmentAmount * paidInstallments;

    printf("\n--- Payment Summary ---");
    printf("\nTotal Price: %.2f BDT", totalGoldPrice);
    printf("\nInstallments Completed: %d / %d", paidInstallments, totalInstallments);
    printf("\nAmount Paid So Far: %.2f BDT", totalPaid);
    printf("\nRemaining Balance: %.2f BDT\n", totalGoldPrice - totalPaid);
}

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
        due = 0;
    }

    printf("Is there any late payment fine? (y/n): ");
    scanf(" %c", &hasLateFee);

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

// ==========================================
// FEATURE 2 FUNCTIONS: Purity & Valuation
// ==========================================

void convertKaratToPercentage(void) {
    float karat, percentage;

    printf("\n[Karat to Percentage Conversion]\n");
    printf("Enter gold karat (e.g., 24, 22, 18, 21): ");
    scanf("%f", &karat);

    if (karat <= 0 || karat > 24) {
        printf("Error: Karat must be between 1 and 24!\n");
        return;
    }

    percentage = (karat / 24.0) * 100.0;

    printf("\n--- Results ---");
    printf("\nKarat: %.1fK", karat);
    printf("\nPurity Percentage: %.2f%%\n", percentage);
}

void calculatePureGoldContent(void) {
    float totalWeight, karat, pureGoldWeight;

    printf("\n[Pure Gold Content Calculator]\n");
    printf("Enter total weight of the ornament (grams): ");
    scanf("%f", &totalWeight);

    printf("Enter gold karat of the item (e.g., 22 for 22K): ");
    scanf("%f", &karat);

    if (totalWeight <= 0 || karat <= 0 || karat > 24) {
        printf("Error: Invalid weight or karat input!\n");
        return;
    }

    pureGoldWeight = totalWeight * (karat / 24.0);

    printf("\n--- Valuation Breakdown ---");
    printf("\nTotal Item Weight: %.2f grams", totalWeight);
    printf("\nItem Purity: %.1fK", karat);
    printf("\nPure Gold Contained: %.2f grams\n", pureGoldWeight);
}

// ==========================================
// FEATURE 3 FUNCTIONS: Low Stock Alert
// ==========================================

void checkSingleItemStock(void) {
    char itemName[50];
    int currentStock, thresholdLimit;

    printf("\n[Single Item Stock Check]\n");

    while (getchar() != '\n'); // Clear input buffer

    printf("Enter gold item name/type (e.g., Ring, Necklace): ");
    fgets(itemName, sizeof(itemName), stdin);
    itemName[strcspn(itemName, "\n")] = 0;

    printf("Enter current stock quantity (grams or pieces): ");
    scanf("%d", &currentStock);

    printf("Enter minimum threshold limit for alert: ");
    scanf("%d", &thresholdLimit);

    if (currentStock < 0 || thresholdLimit < 0) {
        printf("Error: Values cannot be negative!\n");
        return;
    }

    printf("\n--- Stock Report for [%s] ---", itemName);
    printf("\nCurrent Stock: %d", currentStock);
    printf("\nThreshold Limit: %d", thresholdLimit);

    if (currentStock <= thresholdLimit) {
        printf("\n\n>>> ALERT: LOW STOCK! Immediate restocking recommended! <<<\n");
    } else {
        printf("\n\n>>> Status: Stock level is sufficient. <<<\n");
    }
}

void checkMultipleItemsStock(void) {
    int totalItems, i, threshold, stockQty;
    char itemName[50];

    printf("\n[Batch Stock Check]\n");
    printf("Enter total number of items to check: ");
    scanf("%d", &totalItems);

    if (totalItems <= 0) {
        printf("Error: Number of items must be greater than 0!\n");
        return;
    }

    printf("Enter the safety threshold limit for all items: ");
    scanf("%d", &threshold);

    printf("\n--- Checking Items ---");
    for (i = 1; i <= totalItems; i++) {
        while (getchar() != '\n'); // Clear buffer

        printf("\nEnter name of item %d: ", i);
        fgets(itemName, sizeof(itemName), stdin);
        itemName[strcspn(itemName, "\n")] = 0;

        printf("Enter stock quantity for %s: ", itemName);
        scanf("%d", &stockQty);

        if (stockQty <= threshold) {
            printf("[WARNING] '%s' is LOW in stock! (Qty: %d)\n", itemName, stockQty);
        } else {
            printf("[OK] '%s' stock is safe. (Qty: %d)\n", itemName, stockQty);
        }
    }
    printf("\nBatch stock check completed!\n");
}
