#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "medicine.h"
#include <time.h>

// Window dimensions
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

// Control IDs
#define ID_ADD_BUTTON 101
#define ID_SEARCH_BUTTON 102
#define ID_UPDATE_BUTTON 103
#define ID_SELL_BUTTON 104
#define ID_DELETE_BUTTON 105
#define ID_REPORT_BUTTON 106
#define ID_NAME_INPUT 107
#define ID_PRICE_INPUT 108
#define ID_STOCK_INPUT 109
#define ID_SEARCH_INPUT 110
#define ID_QUANTITY_INPUT 111

HWND hwndList;    // List view
HWND hwndName;    // Name input
HWND hwndPrice;   // Price input
HWND hwndStock;   // Stock input
HWND hwndSearch;  // Search input
HWND hwndQuantity; // Quantity input for sales
HWND hwndTotalSales; // Total sales display

Medicine medicines[MAX_MEDICINES];
int medicine_count = 0;

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void create_controls(HWND hwnd);
void refresh_list_view();
void add_medicine_to_list();
void search_medicines();
void update_medicine_stock();
void sell_selected_medicine();
void delete_selected_medicine();
void generate_report();
void update_total_sales();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PharmacyManager";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClassEx(&wc);

    // Create main window
    HWND hwnd = CreateWindowEx(
        0,
        "PharmacyManager",
        "Pharmacy Management System",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            create_controls(hwnd);
            load_medicines_from_file(medicines, &medicine_count);
            refresh_list_view();
            update_total_sales();
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_ADD_BUTTON:
                    add_medicine_to_list();
                    return 0;
                case ID_SEARCH_BUTTON:
                    search_medicines();
                    return 0;
                case ID_UPDATE_BUTTON:
                    update_medicine_stock();
                    return 0;
                case ID_SELL_BUTTON:
                    sell_selected_medicine();
                    return 0;
                case ID_DELETE_BUTTON:
                    delete_selected_medicine();
                    return 0;
                case ID_REPORT_BUTTON:
                    generate_report();
                    return 0;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void create_controls(HWND hwnd) {
    // Create input fields
    CreateWindow("STATIC", "Name:", WS_CHILD | WS_VISIBLE,
                20, 20, 50, 20, hwnd, NULL, NULL, NULL);
    hwndName = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                70, 20, 150, 20, hwnd, (HMENU)ID_NAME_INPUT, NULL, NULL);

    CreateWindow("STATIC", "Price:", WS_CHILD | WS_VISIBLE,
                240, 20, 50, 20, hwnd, NULL, NULL, NULL);
    hwndPrice = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                290, 20, 100, 20, hwnd, (HMENU)ID_PRICE_INPUT, NULL, NULL);

    CreateWindow("STATIC", "Stock:", WS_CHILD | WS_VISIBLE,
                410, 20, 50, 20, hwnd, NULL, NULL, NULL);
    hwndStock = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                460, 20, 100, 20, hwnd, (HMENU)ID_STOCK_INPUT, NULL, NULL);

    // Create quantity input for sales
    CreateWindow("STATIC", "Quantity:", WS_CHILD | WS_VISIBLE,
                580, 20, 60, 20, hwnd, NULL, NULL, NULL);
    hwndQuantity = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                640, 20, 100, 20, hwnd, (HMENU)ID_QUANTITY_INPUT, NULL, NULL);

    // Create buttons - First row
    CreateWindow("BUTTON", "Add Medicine", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 60, 120, 30, hwnd, (HMENU)ID_ADD_BUTTON, NULL, NULL);
    CreateWindow("BUTTON", "Delete Medicine", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                150, 60, 120, 30, hwnd, (HMENU)ID_DELETE_BUTTON, NULL, NULL);
    CreateWindow("BUTTON", "Update Stock", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                280, 60, 120, 30, hwnd, (HMENU)ID_UPDATE_BUTTON, NULL, NULL);
    CreateWindow("BUTTON", "Sell Medicine", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                410, 60, 120, 30, hwnd, (HMENU)ID_SELL_BUTTON, NULL, NULL);

    // Create search controls - Second row
    CreateWindow("STATIC", "Search:", WS_CHILD | WS_VISIBLE,
                20, 100, 50, 20, hwnd, NULL, NULL, NULL);
    hwndSearch = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                70, 100, 150, 20, hwnd, (HMENU)ID_SEARCH_INPUT, NULL, NULL);
    CreateWindow("BUTTON", "Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                230, 100, 80, 25, hwnd, (HMENU)ID_SEARCH_BUTTON, NULL, NULL);
    CreateWindow("BUTTON", "Generate Report", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                320, 100, 120, 25, hwnd, (HMENU)ID_REPORT_BUTTON, NULL, NULL);

    // Create total sales display
    CreateWindow("STATIC", "Total Sales:", WS_CHILD | WS_VISIBLE,
                460, 100, 70, 20, hwnd, NULL, NULL, NULL);
    hwndTotalSales = CreateWindow("STATIC", "0.00", WS_CHILD | WS_VISIBLE | SS_RIGHT,
                530, 100, 100, 20, hwnd, NULL, NULL, NULL);

    // Create list view
    hwndList = CreateWindowEx(0, WC_LISTVIEW, "",
                            WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_BORDER,
                            20, 140, 940, 500, hwnd, NULL, NULL, NULL);

    // Add columns to list view
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    lvc.iSubItem = 0;
    lvc.pszText = "Name";
    lvc.cx = 250;
    ListView_InsertColumn(hwndList, 0, &lvc);

    lvc.iSubItem = 1;
    lvc.pszText = "Price";
    lvc.cx = 150;
    ListView_InsertColumn(hwndList, 1, &lvc);

    lvc.iSubItem = 2;
    lvc.pszText = "Stock";
    lvc.cx = 150;
    ListView_InsertColumn(hwndList, 2, &lvc);

    lvc.iSubItem = 3;
    lvc.pszText = "Total Sales";
    lvc.cx = 150;
    ListView_InsertColumn(hwndList, 3, &lvc);
}

void refresh_list_view() {
    ListView_DeleteAllItems(hwndList);
    
    for (int i = 0; i < medicine_count; i++) {
        LVITEM lvi = {0};
        char buffer[50];

        // Name
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = medicines[i].name;
        ListView_InsertItem(hwndList, &lvi);

        // Price
        sprintf(buffer, "%.2f", medicines[i].price);
        lvi.iSubItem = 1;
        lvi.pszText = buffer;
        ListView_SetItem(hwndList, &lvi);

        // Stock
        sprintf(buffer, "%d", medicines[i].stock);
        lvi.iSubItem = 2;
        lvi.pszText = buffer;
        ListView_SetItem(hwndList, &lvi);

        // Total Sales
        sprintf(buffer, "%.2f", medicines[i].total_sales);
        lvi.iSubItem = 3;
        lvi.pszText = buffer;
        ListView_SetItem(hwndList, &lvi);
    }
}

void add_medicine_to_list() {
    char name[50], price_str[20], stock_str[20];
    
    GetWindowText(hwndName, name, 50);
    GetWindowText(hwndPrice, price_str, 20);
    GetWindowText(hwndStock, stock_str, 20);

    if (strlen(name) == 0 || strlen(price_str) == 0 || strlen(stock_str) == 0) {
        MessageBox(NULL, "Please fill all fields!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (medicine_count >= MAX_MEDICINES) {
        MessageBox(NULL, "Maximum medicine limit reached!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Add new medicine
    strcpy(medicines[medicine_count].name, name);
    medicines[medicine_count].price = (float)atof(price_str);
    medicines[medicine_count].stock = atoi(stock_str);
    medicines[medicine_count].total_sales = 0.0f;

    medicine_count++;
    save_medicines_to_file(medicines, medicine_count);
    refresh_list_view();

    // Clear input fields
    SetWindowText(hwndName, "");
    SetWindowText(hwndPrice, "");
    SetWindowText(hwndStock, "");

    MessageBox(NULL, "Medicine added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
}

void search_medicines() {
    char search_term[50];
    GetWindowText(hwndSearch, search_term, 50);

    if (strlen(search_term) == 0) {
        refresh_list_view();
        return;
    }

    ListView_DeleteAllItems(hwndList);
    
    for (int i = 0; i < medicine_count; i++) {
        if (strstr(medicines[i].name, search_term) != NULL) {
            LVITEM lvi = {0};
            char buffer[50];

            lvi.mask = LVIF_TEXT;
            lvi.iItem = i;
            
            // Name
            lvi.iSubItem = 0;
            lvi.pszText = medicines[i].name;
            ListView_InsertItem(hwndList, &lvi);

            // Price
            sprintf(buffer, "%.2f", medicines[i].price);
            lvi.iSubItem = 1;
            lvi.pszText = buffer;
            ListView_SetItem(hwndList, &lvi);

            // Stock
            sprintf(buffer, "%d", medicines[i].stock);
            lvi.iSubItem = 2;
            lvi.pszText = buffer;
            ListView_SetItem(hwndList, &lvi);

            // Total Sales
            sprintf(buffer, "%.2f", medicines[i].total_sales);
            lvi.iSubItem = 3;
            lvi.pszText = buffer;
            ListView_SetItem(hwndList, &lvi);
        }
    }
}

void update_medicine_stock() {
    int selected = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
    if (selected == -1) {
        MessageBox(NULL, "Please select a medicine to update!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char new_stock_str[20];
    if (GetWindowText(hwndStock, new_stock_str, 20) == 0) {
        MessageBox(NULL, "Please enter new stock value!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    int new_stock = atoi(new_stock_str);
    medicines[selected].stock = new_stock;
    save_medicines_to_file(medicines, medicine_count);
    refresh_list_view();

    SetWindowText(hwndStock, "");
    MessageBox(NULL, "Stock updated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
}

void sell_selected_medicine() {
    int selected = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
    if (selected == -1) {
        MessageBox(NULL, "Please select a medicine to sell!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char quantity_str[20];
    GetWindowText(hwndQuantity, quantity_str, 20);
    if (strlen(quantity_str) == 0) {
        MessageBox(NULL, "Please enter quantity to sell!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    int quantity = atoi(quantity_str);
    if (quantity <= 0) {
        MessageBox(NULL, "Please enter a valid quantity!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (quantity > medicines[selected].stock) {
        MessageBox(NULL, "Not enough stock available!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Update stock and sales
    medicines[selected].stock -= quantity;
    float sale_amount = medicines[selected].price * quantity;
    medicines[selected].total_sales += sale_amount;

    // Create sale record
    Sale sale;
    strcpy(sale.medicine_name, medicines[selected].name);
    sale.quantity = quantity;
    sale.price = sale_amount;
    
    // Get current date
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(sale.date, sizeof(sale.date), "%Y-%m-%d", tm);

    // Save changes
    save_sale(sale);
    save_medicines_to_file(medicines, medicine_count);
    refresh_list_view();
    update_total_sales();

    SetWindowText(hwndQuantity, "");
    char message[100];
    sprintf(message, "Sale completed! Amount: $%.2f", sale_amount);
    MessageBox(NULL, message, "Success", MB_OK | MB_ICONINFORMATION);
}

void delete_selected_medicine() {
    int selected = ListView_GetNextItem(hwndList, -1, LVNI_SELECTED);
    if (selected == -1) {
        MessageBox(NULL, "Please select a medicine to delete!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char message[100];
    sprintf(message, "Are you sure you want to delete %s?", medicines[selected].name);
    if (MessageBox(NULL, message, "Confirm Delete", MB_YESNO | MB_ICONQUESTION) == IDYES) {
        delete_medicine(medicines, &medicine_count, medicines[selected].name);
        save_medicines_to_file(medicines, medicine_count);
        refresh_list_view();
        update_total_sales();
        MessageBox(NULL, "Medicine deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
    }
}

void generate_report() {
    generate_sales_report("", ""); // Generate report for all dates
    MessageBox(NULL, "Sales report has been generated!", "Success", MB_OK | MB_ICONINFORMATION);
}

void update_total_sales() {
    float total = get_total_sales();
    char buffer[50];
    sprintf(buffer, "$%.2f", total);
    SetWindowText(hwndTotalSales, buffer);
}
