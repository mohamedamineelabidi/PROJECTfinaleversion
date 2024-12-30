// medicine.c
#include <stdio.h>
#include <string.h>
#include "medicine.h"

void add_medicine(Medicine medicines[], int *count) {
    printf("Enter medicine name: ");
    scanf("%s", medicines[*count].name);
    printf("Enter price: ");
    scanf("%f", &medicines[*count].price);
    printf("Enter quantity: ");
    scanf("%d", &medicines[*count].quantity);
    (*count)++;
    printf("Medicine added successfully!\n");
}

void view_medicines(Medicine medicines[], int count) {
    printf("\nList of Medicines:\n");
    for (int i = 0; i < count; i++) {
        printf("Name: %s, Price: %.2f, Quantity: %d\n",
               medicines[i].name, medicines[i].price, medicines[i].quantity);
    }
    printf("\n");
}

void delete_medicine(Medicine medicines[], int *count) {
    char name[50];
    printf("Enter the name of the medicine to delete: ");
    scanf("%s", name);

    for (int i = 0; i < *count; i++) {
        if (strcmp(medicines[i].name, name) == 0) {
            for (int j = i; j < *count - 1; j++) {
                medicines[j] = medicines[j + 1];
            }
            (*count)--;
            printf("Medicine deleted successfully!\n");
            return;
        }
    }
    printf("Medicine not found.\n");
}

void sell_medicine(Medicine medicines[], int count) {
    char name[50];
    int quantity;

    printf("Enter the name of the medicine to sell: ");
    scanf("%s", name);
    printf("Enter the quantity to sell: ");
    scanf("%d", &quantity);

    for (int i = 0; i < count; i++) {
        if (strcmp(medicines[i].name, name) == 0) {
            if (medicines[i].quantity >= quantity) {
                float total_price = quantity * medicines[i].price;  // Calculate the total price
                medicines[i].quantity -= quantity;

                printf("Sold %d units of %s for a total of %.2f. Updated quantity: %d\n",
                       quantity, medicines[i].name, total_price, medicines[i].quantity);

                // Log the sale to the invoice
                generate_monthly_invoice("invoice_monthly.txt", name, quantity, total_price);
            } else {
                printf("Insufficient stock. Available quantity: %d\n", medicines[i].quantity);
            }
            return;
        }
    }
    printf("Medicine not found.\n");
}


void save_medicines_to_file(Medicine medicines[], int count) {
    FILE *file = fopen("database.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s \t%.2f \t%d\n", medicines[i].name, medicines[i].price, medicines[i].quantity);
    }

    fclose(file);
    printf("Data saved successfully to file.\n");
}

void load_medicines_from_file(Medicine medicines[], int *count) {
    FILE *file = fopen("database.txt", "r");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }

    *count = 0;
    while (fscanf(file, "%s %f %d", medicines[*count].name, &medicines[*count].price, &medicines[*count].quantity) == 3) {
        (*count)++;
    }

    fclose(file);
    printf("Data loaded successfully from file.\n");
}

void generate_monthly_invoice(const char *filename, const char *medicine_name, int quantity_sold, float total_price) {
    FILE *file = fopen("invoice_monthly.txt", "a");
    if (file == NULL) {
        printf("Error opening invoice file.\n");
        return;
    }

    fprintf(file, "Medicine: %s, Quantity Sold: %d, Total Price: %.2f\n",
            medicine_name, quantity_sold, total_price);

    fclose(file);
    printf("Transaction added to monthly invoice.\n");
}

void view_invoice() {
    FILE *file = fopen("invoice_monthly.txt", "r");
    if (file == NULL) {
        printf("No invoice found.\n");
        return;
    }

    char line[256];
    printf("\nMonthly Invoice:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

void calculate_total_monthly_sales(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No invoice found.\n");
        return;
    }

    float total_monthly_sales = 0.0;
    float total_price;

    // Read each line and extract the total price (the last number on each line)
    while (fscanf(file, "Medicine: %*[^,], Quantity Sold: %*d, Total Price: %f\n", &total_price) == 1) {
        total_monthly_sales += total_price;  // Add the total price of this sale to the monthly total
    }

    fclose(file);

    printf("\nTotal Price of Medicines Sold This Month: %.2f\n", total_monthly_sales);
}