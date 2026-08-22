#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 200
#define MAX_SALES 200
#define LOW_STOCK_QUANTITY 3

/* Structures */
typedef struct {
    char name[100];
    char phoneNumber[12];
} Customer;

typedef struct {
    int id;
    char goldname[50];
    int karat;          // 18, 21, 22, 24
    float weight;       // in grams
    float makingCharge; // per item making cost
    int quantity;
} GoldItem;

typedef struct {
    int invoiceNo;
    Customer customer;
    int itemId;
    char itemName[50];
    int qtySold;
    float totalPrice;
} SaleRecord;

/* Function Prototypes */
void pressEnterToContinue();
int isDuplicateId(int searchId);
int findGoldById(int searchId);
void replace(char *str, char orig, char rep);
void saveInventoryToFile();
void loadInventoryFromFile();
void loadSalesFromFile();
void displayInventory();
void addNewItem();
void updateStockQuantity();
float calculateItemPrice(float weight, int karat, float making);
void profitCounter();
void updateMarketRate();
void oldGoldExchange();
void saleAndReceipt();
void TopSellingItem();
void lowStockAlert();
void convertKaratToPercentage();
void calculatePureGoldContent();
void purityConverterMenu();
void exitProgram();

/* Global State */
GoldItem inventory[MAX_ITEMS];
int itemCount = 0;

SaleRecord sales[MAX_SALES];
int saleCount = 0;

float rate24K = 75.0; // Daily 24K gold rate per gram
int invoiceCounter = 1001;

/* Helper Functions */

/* it is mainly used to help replace '\n' with '\0' in any string that is taken input by fgets()
   becuase fgets() also fetches '\n' from buffer */
void replace(char *str, char orig, char rep) {
    char *ix = str;
    while ((ix = strchr(ix, orig)) != NULL) {
        *ix = rep;
    }
}

void pressEnterToContinue() {
    printf("\n------------------------------------------------------------\n");
    printf("Press ENTER to return to the Main Menu...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();//If I don't use this extra getchar(), the function will not wait 
                //and continue without waiting
}

int isDuplicateId(int searchId) {
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == searchId) {
            return 1;
        }
    }
    return 0;
}
//used to find the gold item in the inventory.txt by iterating through the file
int findGoldById(int searchId) {
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == searchId) {
            return i;
        }
    }
    return -1;
}

/* PRITHU (file management) */
//=============================
//primary functions. without these the program will not run properly

/* this updates the goldItem list along with their quantity */
void saveInventoryToFile() {
    FILE *file = fopen("inventory.txt", "w");
    if (file == NULL) {
        printf("[ERROR] File Permission Error: Unable to open 'inventory.txt' for writing.\n");
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d,%s,%d,%.2f,%.2f,%d\n", 
                inventory[i].id, 
                inventory[i].goldname, 
                inventory[i].karat, 
                inventory[i].weight, 
                inventory[i].makingCharge, 
                inventory[i].quantity);
    }
    fclose(file);
    printf("[SYSTEM INFO] Successfully exported %d inventory records to 'inventory.txt'.\n", itemCount);
}

/*below two functions get called in the beginning of the program 
updating the itemCount and sellCount according to how many items are in the inventory.txt
and sales_history.txt */
void loadInventoryFromFile() {
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("[SYSTEM INFO] 'inventory.txt' not found. Starting with an empty inventory database.\n");
        return;
    }
    itemCount = 0;
    while (fscanf(file, "%d,%49[^,],%d,%f,%f,%d\n", 
                  &inventory[itemCount].id, 
                  inventory[itemCount].goldname, 
                  &inventory[itemCount].karat, 
                  &inventory[itemCount].weight, 
                  &inventory[itemCount].makingCharge, 
                  &inventory[itemCount].quantity) == 6) 
    {
        itemCount++;
        if (itemCount >= MAX_ITEMS) break;
    }
    fclose(file);
    printf("[SYSTEM INFO] Automatically loaded %d item records from 'inventory.txt'.\n", itemCount);
}

void loadSalesFromFile() {
    FILE *file = fopen("sales_history.txt", "r");
    if (file == NULL) {
        printf("[SYSTEM INFO] 'sales_history.txt' not found. Starting with an empty sales history.\n");
        return;
    }
    saleCount = 0;
    while (saleCount < MAX_SALES && 
           fscanf(file, "%d,%99[^,],%d,%49[^,],%d,%f\n", 
                  &sales[saleCount].invoiceNo, 
                  sales[saleCount].customer.name, 
                  &sales[saleCount].itemId, 
                  sales[saleCount].itemName, 
                  &sales[saleCount].qtySold, 
                  &sales[saleCount].totalPrice) == 6) 
    {
        //even if the program starts with invoice 1001, this updates the invoiceNo in the beginning of the program
        if (sales[saleCount].invoiceNo >= invoiceCounter) {
            invoiceCounter = sales[saleCount].invoiceNo + 1;
        }
        saleCount++;
    }
    fclose(file);
    printf("[SYSTEM INFO] Automatically loaded %d sales records from 'sales_history.txt'.\n", saleCount);
}

void displayInventory() {
    printf("\n==================================================================================================\n");
    printf("                                  MODULE: VIEW STOCK INVENTORY                                    \n");
    printf("==================================================================================================\n");
    printf("Prices are dynamically calculated based on Today's 24K Rate ($%.2f/g).\n", rate24K);
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("%-6s | %-20s | %-6s | %-10s | %-12s | %-8s | %-12s\n", 
           "ID", "Item Name", "Karat", "Weight(g)", "Making($)", "Stock", "Unit Price($)");
    printf("--------------------------------------------------------------------------------------------------\n");

    if (itemCount == 0) {
        printf("                              No items available in inventory.                                   \n");
    } else {
        for (int i = 0; i < itemCount; i++) {
            float unitPrice = calculateItemPrice(inventory[i].weight, inventory[i].karat, inventory[i].makingCharge);
            printf("%-6d | %-20s | %-6d | %-10.2f | %-12.2f | %-8d | %-12.2f\n", 
                   inventory[i].id, 
                   inventory[i].goldname, 
                   inventory[i].karat, 
                   inventory[i].weight, 
                   inventory[i].makingCharge, 
                   inventory[i].quantity, 
                   unitPrice);
        }
    }
    printf("--------------------------------------------------------------------------------------------------\n");

    pressEnterToContinue();
}

/* added functionalities*/

void addNewItem() {
    printf("\n============================================================\n");
    printf("                   MODULE: ADD NEW GOLD ITEM                \n");
    printf("============================================================\n");

    if (itemCount >= MAX_ITEMS) {
        printf("[ERROR] Inventory storage capacity (%d items) is full!\n", MAX_ITEMS);
        pressEnterToContinue();
        return;
    }

    GoldItem newItem;
    
    do {
        printf("Step 1/6 - Enter Unique Item ID (e.g., 101): ");
        scanf("%d", &newItem.id);

        if (isDuplicateId(newItem.id)) {
            printf("%d is a duplicate id, try again..\n", newItem.id);
        }
    } while (isDuplicateId(newItem.id));

    printf("Step 2/6 - Enter Item Name: ");
    getchar(); 
    fgets(newItem.goldname, sizeof(newItem.goldname), stdin);
    replace(newItem.goldname, '\n', '\0');

    printf("Step 3/6 - Enter Karat Purity (18, 21, 22, or 24): ");
    scanf("%d", &newItem.karat);

    printf("Step 4/6 - Enter Weight in grams: ");
    scanf("%f", &newItem.weight);

    printf("Step 5/6 - Enter Making Charge in USD ($): ");
    scanf("%f", &newItem.makingCharge);

    printf("Step 6/6 - Enter Initial Stock Quantity: ");
    scanf("%d", &newItem.quantity);

    inventory[itemCount] = newItem;
    itemCount++;

    saveInventoryToFile();

    printf("\n[SUCCESS] New item '%s' (ID: %d) successfully saved.\n", newItem.goldname, newItem.id);
    pressEnterToContinue();
}

void updateStockQuantity() {
    printf("\n============================================================\n");
    printf("               MODULE: RESTOCK INVENTORY QUANTITY           \n");
    printf("============================================================\n");

    int searchId, addedQty;
    printf("Enter Item ID to Restock: ");
    scanf("%d", &searchId);

    int pos = findGoldById(searchId);//returns the position of the desired item
    if (pos != -1) {
        printf("Found Item: '%s' | Current Stock: %d\n", inventory[pos].goldname, inventory[pos].quantity);
        printf("Enter quantity to ADD into inventory: ");
        scanf("%d", &addedQty);

        if (addedQty >= 0) {
            inventory[pos].quantity += addedQty;
            printf("\n[SUCCESS] Updated! Item '%s' now has %d total units in stock.\n", 
                   inventory[pos].goldname, inventory[pos].quantity);
            saveInventoryToFile();
        } else {
            printf("\n[ERROR] Quantity added must be zero or positive.\n");
        }
    } else {
        printf("\n[ERROR] Item ID #%d not found in inventory.\n", searchId);
    }

    pressEnterToContinue();
}


/* TAHMID (gold pricing and profit management)*/
//=============================================
float calculateItemPrice(float weight, int karat, float making) {
    float purity = (float)karat / 24.0f;
    float price = weight * rate24K * purity;
    return price + making;
}

void profitCounter() {
    printf("\n================================================================\n");
    printf("              MODULE: SALES HISTORY & REVENUE AUDIT             \n");
    printf("================================================================\n");
    printf("%-10s | %-18s | %-18s | %-5s | %-10s\n", 
           "Invoice #", "Customer", "Item", "Qty", "Total ($)");
    printf("----------------------------------------------------------------\n");

    float grandTotalRevenue = 0;
    if (saleCount == 0) {
        printf("                   No sales history recorded yet.                \n");
    } else {
        for (int i = 0; i < saleCount; i++) {
            printf("%-10d | %-18s | %-18s | %-5d | $%-10.2f\n", 
                   sales[i].invoiceNo, sales[i].customer.name, sales[i].itemName, 
                   sales[i].qtySold, sales[i].totalPrice);
            grandTotalRevenue += sales[i].totalPrice;
        }
    }
    printf("----------------------------------------------------------------\n");
    printf("Cumulative Revenue: $%.2f\n", grandTotalRevenue);

    pressEnterToContinue();
}

void updateMarketRate() {
    printf("\n============================================================\n");
    printf("                MODULE: UPDATE DAILY GOLD RATE              \n");
    printf("============================================================\n");
    printf("Current 24K Gold Rate: $%.2f per gram\n", rate24K);
    printf("Please enter the new rate ($/gram): ");
    
    if (scanf("%f", &rate24K) == 1 && rate24K > 0) {
        printf("\n[SUCCESS] Updated daily 24K rate to $%.2f / gram.\n", rate24K);
    } else {
        while (getchar() != '\n'); 
        printf("\n[ERROR] Invalid entry. Gold rate must be a positive number.\n");
    }

    pressEnterToContinue();
}

void oldGoldExchange() {
    int oldKarat, newId, pos;
    float oldWeight, cut;
    float oldValue, newPrice, diff;
    Customer cust;

    printf("\n============================================================\n");
    printf("               MODULE: OLD GOLD EXCHANGE DESK               \n");
    printf("============================================================\n");
    printf("Enter Old Gold Weight (grams): ");
    scanf("%f", &oldWeight);

    printf("Enter Old Gold Karat (18, 21, 22, 24): ");
    scanf("%d", &oldKarat);

    printf("Enter Deduction Percentage (e.g., 10 for 10%%): ");
    scanf("%f", &cut);

    printf("Enter New Item ID to Exchange For: ");
    scanf("%d", &newId);

    pos = findGoldById(newId);

    if (pos == -1) {
        printf("\n[ERROR] New item ID %d not found in inventory.\n", newId);
        pressEnterToContinue();
        return;
    }

    if (inventory[pos].quantity <= 0) {
        printf("\n[ERROR] New item '%s' is out of stock.\n", inventory[pos].goldname);
        pressEnterToContinue();
        return;
    }

    printf("Enter Customer Name: ");
    getchar(); 
    fgets(cust.name, sizeof(cust.name), stdin);
    replace(cust.name, '\n', '\0');

    printf("Enter Customer Phone Number: ");
    fgets(cust.phoneNumber, sizeof(cust.phoneNumber), stdin);
    replace(cust.phoneNumber, '\n', '\0');

    oldValue = oldWeight * rate24K * ((float)oldKarat / 24.0f);
    oldValue = oldValue - (oldValue * cut / 100.0f);

    newPrice = calculateItemPrice(inventory[pos].weight, inventory[pos].karat, inventory[pos].makingCharge);
    diff = newPrice - oldValue;

    inventory[pos].quantity--;
    saveInventoryToFile();

    SaleRecord newExchange;
    newExchange.invoiceNo = invoiceCounter++;
    newExchange.customer = cust;
    newExchange.itemId = inventory[pos].id;
    strcpy(newExchange.itemName, inventory[pos].goldname);
    newExchange.qtySold = 1;
    newExchange.totalPrice = (diff > 0) ? diff : 0.0f;

    if (saleCount < MAX_SALES) {
        sales[saleCount++] = newExchange;
    }

    FILE *file = fopen("sales_history.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d,%s,%d,%s,%d,%.2f\n", 
                newExchange.invoiceNo, newExchange.customer.name, newExchange.itemId, 
                newExchange.itemName, newExchange.qtySold, newExchange.totalPrice);
        fclose(file);
    }

    printf("\n============================================\n");
    printf("            EXCHANGE BILL RECEIPT           \n");
    printf("============================================\n");
    printf("Invoice No     : #%d\n", newExchange.invoiceNo);
    printf("Customer       : %s (%s)\n", cust.name, cust.phoneNumber);
    printf("Old Gold Value : $%.2f (Deducted %.1f%%)\n", oldValue, cut);
    printf("New Item Selected: %s (ID: %d)\n", inventory[pos].goldname, inventory[pos].id);
    printf("New Item Price : $%.2f\n", newPrice);
    printf("--------------------------------------------\n");

    if (diff > 0) {
        printf("Customer Pays  : $%.2f\n", diff);
    } else if (diff < 0) {
        printf("Shop Returns   : $%.2f\n", -diff);
    } else {
        printf("Even Trade     : $0.00\n");
    }
    printf("============================================\n");

    pressEnterToContinue();
}

/* ARIF (Sales) */
//================
void saleAndReceipt() {
    printf("\n============================================================\n");
    printf("                MODULE: SALES DESK & BILLING                \n");
    printf("============================================================\n");

    int searchId, quantityToBuy;
    printf("Step 1/3 - Enter Item ID to Purchase: ");
    scanf("%d", &searchId);

    int itemIndex = findGoldById(searchId);

    if (itemIndex == -1) {
        printf("[ERROR] Item ID #%d not found in inventory.\n", searchId);
        pressEnterToContinue();
        return;
    }

    if (inventory[itemIndex].quantity <= 0) {
        printf("[ERROR] Item '%s' is completely OUT OF STOCK.\n", inventory[itemIndex].goldname);
        pressEnterToContinue();
        return;
    }

    printf("           Item Selected: %s (Available Stock: %d)\n", inventory[itemIndex].goldname, inventory[itemIndex].quantity);
    printf("Step 2/3 - Enter Quantity to Purchase: ");
    scanf("%d", &quantityToBuy);

    if (quantityToBuy > inventory[itemIndex].quantity || quantityToBuy <= 0) {
        printf("[ERROR] Invalid quantity. Entered quantity exceeds stock level or is invalid.\n");
        pressEnterToContinue();
        return;
    }

    char custName[100];
    printf("Step 3/3 - Enter Customer Full Name: ");
    getchar();
    fgets(custName, sizeof(custName), stdin);
    replace(custName, '\n', '\0');

    float unitPrice = calculateItemPrice(inventory[itemIndex].weight, inventory[itemIndex].karat, inventory[itemIndex].makingCharge);
    float totalPrice = unitPrice * quantityToBuy;

    inventory[itemIndex].quantity -= quantityToBuy;
    saveInventoryToFile();

    SaleRecord newSale;
    newSale.invoiceNo = invoiceCounter++;
    strcpy(newSale.customer.name, custName);
    newSale.itemId = inventory[itemIndex].id;
    strcpy(newSale.itemName, inventory[itemIndex].goldname);
    newSale.qtySold = quantityToBuy;
    newSale.totalPrice = totalPrice;

    if (saleCount < MAX_SALES) {
        sales[saleCount++] = newSale;
    }

    FILE *file = fopen("sales_history.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d,%s,%d,%s,%d,%.2f\n", 
                newSale.invoiceNo, newSale.customer.name, newSale.itemId, 
                newSale.itemName, newSale.qtySold, newSale.totalPrice);
        fclose(file);
    }

    printf("\n============================================\n");
    printf("             OFFICIAL SALES INVOICE         \n");
    printf("============================================\n");
    printf("Invoice No  : #%d\n", newSale.invoiceNo);
    printf("Customer    : %s\n", newSale.customer.name);
    printf("Item Purchased: %s (%dK Gold)\n", inventory[itemIndex].goldname, inventory[itemIndex].karat);
    printf("Quantity    : %d unit(s)\n", quantityToBuy);
    printf("Unit Price  : $%.2f\n", unitPrice);
    printf("--------------------------------------------\n");
    printf("TOTAL AMOUNT: $%.2f\n", totalPrice);
    printf("============================================\n");

    pressEnterToContinue();
}

void TopSellingItem() {
    FILE *f = fopen("sales_history.txt", "r");
    if (!f) {
        printf("\n[INFO] There is no selling record found.\n");
        pressEnterToContinue();
        return;
    }

    int invoiceNo, itemId, qtySold;
    char customerName[100], itemName[50];
    float totalPrice;

    int maxQty = 0;
    int topItemId = -1;
    char topItemName[50] = "";

    while (fscanf(f, "%d,%99[^,],%d,%49[^,],%d,%f\n", 
                  &invoiceNo, customerName, &itemId, itemName, &qtySold, &totalPrice) == 6) {
        if (qtySold > maxQty) {
            maxQty = qtySold;
            topItemId = itemId;
            strcpy(topItemName, itemName);
        }
    }
    fclose(f);

    if (maxQty > 0) {
        printf("\n============================================\n");
        printf("              TOP SELLING ITEM              \n");
        printf("============================================\n");
        printf("Item ID   : %d\n", topItemId);
        printf("Item Name : %s\n", topItemName);
        printf("Units Sold: %d\n", maxQty);
        printf("============================================\n");
    } else {
        printf("\nNo recorded sales.\n");
    }

    pressEnterToContinue();
}

/* RUMI (stock and item purity checker) */
void lowStockAlert() {
    printf("\n============================================================\n");
    printf("                MODULE: LOW STOCK SCANNER                   \n");
    printf("============================================================\n");

    int alertCount = 0;
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].quantity <= LOW_STOCK_QUANTITY) {
            printf("[WARNING] Item ID #%d ('%s') is LOW ON STOCK! Only %d units left.\n", 
                   inventory[i].id, inventory[i].goldname, inventory[i].quantity);
            alertCount++;
        }
    }

    if (alertCount == 0) {
        printf("[GOOD] All stock levels are currently healthy.\n");
    }

    pressEnterToContinue();
}

/* these three functions are all part of purity checker */
void convertKaratToPercentage() {
    float karat, percentage;

    printf("\n[Karat to Percentage Conversion]\n");
    printf("Enter gold karat (e.g., 24, 22, 18, 21): ");
    scanf("%f", &karat);

    if (karat <= 0 || karat > 24) {
        printf("Error: Karat must be between 1 and 24!\n");
        return;
    }

    percentage = (karat / 24.0f) * 100.0f;
    printf("\n--- Results ---");
    printf("\nKarat: %.1fK", karat);
    printf("\nPurity Percentage: %.2f%%\n", percentage);
}

void calculatePureGoldContent() {
    float totalWeight, karat, pureGoldWeight;

    printf("\n[Pure Gold Content Calculator]\n");
    printf("Enter total weight of the ornament (grams): ");
    scanf("%f", &totalWeight);

    printf("Enter gold karat of the item (e.g., 22 for 22K): ");
    scanf("%f", &karat);

    if (totalWeight <= 0 || karat <= 0 || karat > 24) {
        printf("Error: Invalid input values!\n");
        return;
    }

    pureGoldWeight = totalWeight * (karat / 24.0f);

    printf("\n--- Valuation Breakdown ---");
    printf("\nTotal Item Weight: %.2f grams", totalWeight);
    printf("\nItem Purity: %.1fK", karat);
    printf("\nPure Gold Contained: %.2f grams\n", pureGoldWeight);
}

void purityConverterMenu() {
    int choice = 0;

    while (1) {
        printf("\n============================================\n");
        printf("   GOLD PURITY CONVERTER & VALUATION MENU   \n");
        printf("============================================\n");
        printf("1. Convert Karat to Purity Percentage\n");
        printf("2. Calculate Pure Gold Weight in Item\n");
        printf("3. Return to Main Menu\n");
        printf("============================================\n");
        printf("Enter your choice (1-3): ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\n[ERROR] Invalid option!\n");
            continue;
        }

        if (choice == 1) {
            convertKaratToPercentage();
            pressEnterToContinue();
        } else if (choice == 2) {
            calculatePureGoldContent();
            pressEnterToContinue();
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}

//end program
void exitProgram() {
    saveInventoryToFile();
    printf("\n============================================\n");
    printf("      GOLD STORE SYSTEM SHUTTING DOWN       \n");
    printf("============================================\n");
    printf("All inventory updates and transactions saved.\n");
    printf("Goodbye!\n");
    printf("============================================\n");
}


int main() {
    loadInventoryFromFile();
    loadSalesFromFile();
    // pressEnterToContinue();

    int choice = -1;

    while (choice != 0) {
        printf("\n============================================\n");
        printf("        GOLD STORE MANAGEMENT SYSTEM        \n");
        printf("============================================\n");
        printf("1. Update Today's 24K Gold Rate ($%.2f/g)\n", rate24K);
        printf("2. View Full Stock Inventory\n");
        printf("3. Add New Item to Inventory\n");
        printf("4. Restock Item Quantity\n");
        printf("5. Gold Purity Convertor\n");
        printf("6. Process Sale & Print Invoice\n");
        printf("7. Old Gold Exchange Desk\n");
        printf("8. View Sales History & Revenue\n");
        printf("9. View Top Selling Item\n");
        printf("10. Check Low Stock Warnings\n");
        printf("0. Save & Exit Program\n");
        printf("============================================\n");
        printf("Enter Choice (0-10): ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            printf("\n[ERROR] Invalid option! Please enter a valid number.\n");
            continue;
        }

        switch (choice) {
            case 1:  updateMarketRate();     break;
            case 2:  displayInventory();     break;
            case 3:  addNewItem();           break;
            case 4:  updateStockQuantity();  break;
            case 5:  purityConverterMenu();  break;
            case 6:  saleAndReceipt();       break;
            case 7:  oldGoldExchange();      break;
            case 8:  profitCounter();        break;
            case 9:  TopSellingItem();       break;
            case 10: lowStockAlert();        break;
            case 0:  exitProgram();          break;
            default: printf("\n[ERROR] Option out of range. Choice must be between 0 and 10.\n"); break;
        }
    }

    return 0;
}