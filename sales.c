#include <stdio.h>
#include <string.h>
#include <time.h>
#include "medicine.h"

#define SALES_FILE "sales.txt"

void save_sale(Sale sale) {
    FILE *file = fopen(SALES_FILE, "a");
    if (file == NULL) {
        printf("Error opening sales file!\n");
        return;
    }

    fprintf(file, "%s|%s|%d|%.2f\n", 
            sale.date,
            sale.medicine_name,
            sale.quantity,
            sale.price);

    fclose(file);
}

float get_total_sales() {
    float total = 0.0f;
    FILE *file = fopen(SALES_FILE, "r");
    if (file == NULL) {
        return total;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char date[20], name[50];
        int quantity;
        float price;
        
        if (sscanf(line, "%[^|]|%[^|]|%d|%f", 
                   date, name, &quantity, &price) == 4) {
            total += price;
        }
    }

    fclose(file);
    return total;
}

void generate_sales_report(const char* start_date, const char* end_date) {
    FILE *sales_file = fopen(SALES_FILE, "r");
    FILE *report_file = fopen("sales_report.txt", "w");
    
    if (sales_file == NULL || report_file == NULL) {
        if (sales_file) fclose(sales_file);
        if (report_file) fclose(report_file);
        return;
    }

    fprintf(report_file, "SALES REPORT\n");
    fprintf(report_file, "============\n\n");
    fprintf(report_file, "Date       | Medicine Name      | Quantity | Amount\n");
    fprintf(report_file, "-----------------------------------------------\n");

    char line[256];
    float total = 0.0f;
    int total_items = 0;

    while (fgets(line, sizeof(line), sales_file)) {
        char date[20], name[50];
        int quantity;
        float price;
        
        if (sscanf(line, "%[^|]|%[^|]|%d|%f", 
                   date, name, &quantity, &price) == 4) {
            // If start_date and end_date are provided, filter by date range
            if (strlen(start_date) > 0 && strlen(end_date) > 0) {
                if (strcmp(date, start_date) < 0 || strcmp(date, end_date) > 0) {
                    continue;
                }
            }

            fprintf(report_file, "%-10s | %-16s | %8d | $%8.2f\n",
                    date, name, quantity, price);
            total += price;
            total_items += quantity;
        }
    }

    fprintf(report_file, "-----------------------------------------------\n");
    fprintf(report_file, "Total Items Sold: %d\n", total_items);
    fprintf(report_file, "Total Sales: $%.2f\n", total);

    fclose(sales_file);
    fclose(report_file);
}