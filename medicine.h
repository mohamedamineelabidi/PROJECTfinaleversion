#ifndef MEDICINE_H
#define MEDICINE_H

#define MAX_MEDICINES 1000

typedef struct {
    char name[50];
    float price;
    int stock;
    float total_sales;
} Medicine;

typedef struct {
    char medicine_name[50];
    int quantity;
    float price;
    char date[20];
} Sale;

// Function prototypes
void add_medicine(Medicine medicines[], int *count);
void view_medicines(Medicine medicines[], int count);
void update_stock(Medicine medicines[], int count);
void save_medicines_to_file(Medicine medicines[], int count);
void load_medicines_from_file(Medicine medicines[], int *count);
void search_medicine(Medicine medicines[], int count, const char* name);
void sell_medicine(Medicine medicines[], int count, const char* name, int quantity);
void save_sale(Sale sale);
void generate_sales_report(const char* start_date, const char* end_date);
float get_total_sales();
void delete_medicine(Medicine medicines[], int *count, const char* name);

#endif