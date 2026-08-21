#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// Function declarations for easy integration by team lead
void convertKaratToPercentage(void);
void calculatePureGoldContent(void);

int main() {
    int choice;

    // Simple standalone menu for testing this module
    while (1) {
        printf("\n--- GOLD PURITY CONVERTER & VALUATION ---\n");
        printf("1. Convert Karat to Purity Percentage\n");
        printf("2. Calculate Pure Gold Weight in Item\n");
        printf("3. Exit Purity Module\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &choice);

        if (choice == 1) {
            convertKaratToPercentage();
        } else if (choice == 2) {
            calculatePureGoldContent();
        } else if (choice == 3) {
            printf("Exiting Purity Converter...\n");
            break;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Function 1: Convert Karat to Percentage Purity
void convertKaratToPercentage(void) {
    float karat, percentage;

    printf("\n[Karat to Percentage Conversion]\n");
    printf("Enter gold karat (e.g., 24, 22, 18, 21): ");
    scanf("%f", &karat);

    if (karat <= 0 || karat > 24) {
        printf("Error: Karat must be between 1 and 24!\n");
        return;
    }

    // Formula: (Karat / 24) * 100
    percentage = (karat / 24.0) * 100.0;

    printf("\n--- Results ---");
    printf("\nKarat: %.1fK", karat);
    printf("\nPurity Percentage: %.2f%%\n", percentage);
}

// Function 2: Calculate Pure Gold Weight Contained in an Item
void calculatePureGoldContent(void) {
    float totalWeight, karat, pureGoldWeight;

    printf("\n[Pure Gold Content Calculator]\n");
    printf("Enter total weight of the ornament (grams): ");
    scanf("%f", &totalWeight);

    printf("Enter gold karat of the item (e.g., 22 for 22K): ");
    scanf("%f", &karat);

    if (totalWeight <= 0) {
        printf("Error: Weight must be greater than 0!\n");
        return;
    }

    if (karat <= 0 || karat > 24) {
        printf("Error: Karat must be between 1 and 24!\n");
        return;
    }

    // Formula: Total Weight * (Karat / 24)
    pureGoldWeight = totalWeight * (karat / 24.0);

    printf("\n--- Valuation Breakdown ---");
    printf("\nTotal Item Weight: %.2f grams", totalWeight);
    printf("\nItem Purity: %.1fK", karat);
    printf("\nPure Gold Contained: %.2f grams\n", pureGoldWeight);
}
