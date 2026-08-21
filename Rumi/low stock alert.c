#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// Function declarations for team integration
void checkSingleItemStock(void);
void checkMultipleItemsStock(void);

int main() {
    int choice;

    // Simple standalone menu for testing this module
    while (1) {
        printf("\n--- LOW STOCK ALERT SYSTEM ---\n");
        printf("1. Check Single Item Stock Level\n");
        printf("2. Check Multiple Items Stock (Batch Check)\n");
        printf("3. Exit Stock Module\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &choice);

        if (choice == 1) {
            checkSingleItemStock();
        } else if (choice == 2) {
            checkMultipleItemsStock();
        } else if (choice == 3) {
            printf("Exiting Stock Alert System...\n");
            break;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Function 1: Check a single item's stock status against a threshold
void checkSingleItemStock(void) {
    char itemName[50];
    int currentStock, thresholdLimit;

    printf("\n[Single Item Stock Check]\n");

    // Clear input buffer before reading string
    while (getchar() != '\n');

    printf("Enter gold item name/type (e.g., Ring, Necklace): ");
    fgets(itemName, sizeof(itemName), stdin);
    itemName[strcspn(itemName, "\n")] = 0; // Remove trailing newline

    printf("Enter current stock quantity (grams or pieces): ");
    scanf("%d", &currentStock);

    printf("Enter minimum threshold limit for alert: ");
    scanf("%d", &thresholdLimit);

    if (currentStock < 0 || thresholdLimit < 0) {
        printf("Error: Stock and threshold values cannot be negative!\n");
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

// Function 2: Check multiple items using a simple loop (Array-like approach)
void checkMultipleItemsStock(void) {
    int totalItems, i, threshold;
    int stockQty;
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
