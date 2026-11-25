#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ASSETS 200
#define MAX_LOANS 1000
#define NAME_LEN 80
#define FILE_ASSETS "data/assets.txt"
#define FILE_LOANS  "data/loans.txt"

typedef struct {
    int id;
    char name[NAME_LEN];
    int total;
    int available;
} Asset;

typedef struct {
    int id;
    int asset_id;
    char borrower[NAME_LEN];
    int quantity;
    int active; /* 1 = currently borrowed, 0 = returned */
} Loan;

static Asset assets[MAX_ASSETS];
static Loan loans[MAX_LOANS];
static int asset_count = 0, loan_count = 0;

void trim_newline(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void read_text(const char *prompt, char *out, size_t size) {
    for (;;) {
        printf("%s", prompt);
        if (!fgets(out, (int)size, stdin)) {
            out[0] = '\0';
            return;
        }
        trim_newline(out);
        if (strlen(out) > 0 && strchr(out, '|') == NULL) return;
        printf("Please enter a non-empty value without the | character.\n");
    }
}

int read_int(const char *prompt, int min_value, int max_value) {
    char line[100], *end;
    long value;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return min_value;
        value = strtol(line, &end, 10);
        while (*end && isspace((unsigned char)*end)) end++;
        if (end != line && *end == '\0' && value >= min_value && value <= max_value)
            return (int)value;
        printf("Enter a whole number from %d to %d.\n", min_value, max_value);
    }
}

int find_asset_index(int id) {
    for (int i = 0; i < asset_count; i++)
        if (assets[i].id == id) return i;
    return -1;
}

int find_loan_index(int id) {
    for (int i = 0; i < loan_count; i++)
        if (loans[i].id == id) return i;
    return -1;
}

void save_assets(void) {
    FILE *fp = fopen(FILE_ASSETS, "w");
    if (!fp) {
        printf("Error: could not save inventory.\n");
        return;
    }
    for (int i = 0; i < asset_count; i++)
        fprintf(fp, "%d|%s|%d|%d\n", assets[i].id, assets[i].name,
                assets[i].total, assets[i].available);
    fclose(fp);
}

void save_loans(void) {
    FILE *fp = fopen(FILE_LOANS, "w");
    if (!fp) {
        printf("Error: could not save loan records.\n");
        return;
    }
    for (int i = 0; i < loan_count; i++)
        fprintf(fp, "%d|%d|%s|%d|%d\n", loans[i].id, loans[i].asset_id,
                loans[i].borrower, loans[i].quantity, loans[i].active);
    fclose(fp);
}

void load_data(void) {
    FILE *fp = fopen(FILE_ASSETS, "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp) && asset_count < MAX_ASSETS) {
            Asset a;
            if (sscanf(line, "%d|%79[^|]|%d|%d",
                       &a.id, a.name, &a.total, &a.available) == 4)
                assets[asset_count++] = a;
        }
        fclose(fp);
    }

    fp = fopen(FILE_LOANS, "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp) && loan_count < MAX_LOANS) {
            Loan l;
            if (sscanf(line, "%d|%d|%79[^|]|%d|%d",
                       &l.id, &l.asset_id, l.borrower,
                       &l.quantity, &l.active) == 5)
                loans[loan_count++] = l;
        }
        fclose(fp);
    }

    if (asset_count == 0) {
        Asset starter[] = {
            {1, "Arduino Uno", 5, 5},
            {2, "Digital Multimeter", 8, 8},
            {3, "Breadboard", 12, 12},
            {4, "USB Logic Analyzer", 3, 3},
            {5, "Soldering Station", 2, 2}
        };
        asset_count = (int)(sizeof(starter) / sizeof(starter[0]));
        for (int i = 0; i < asset_count; i++) assets[i] = starter[i];
        save_assets();
    }
}

void show_inventory(void) {
    printf("\n%-5s %-25s %8s %10s\n", "ID", "EQUIPMENT", "TOTAL", "AVAILABLE");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < asset_count; i++)
        printf("%-5d %-25s %8d %10d\n", assets[i].id, assets[i].name,
               assets[i].total, assets[i].available);
    if (asset_count == 0) printf("No equipment has been added yet.\n");
}

void add_asset(void) {
    if (asset_count >= MAX_ASSETS) {
        printf("Inventory is full.\n");
        return;
    }
    Asset a;
    a.id = 1;
    for (int i = 0; i < asset_count; i++)
        if (assets[i].id >= a.id) a.id = assets[i].id + 1;
    read_text("Equipment name: ", a.name, sizeof(a.name));
    a.total = read_int("Quantity: ", 1, 10000);
    a.available = a.total;
    assets[asset_count++] = a;
    save_assets();
    printf("Added successfully. Equipment ID: %d\n", a.id);
}

void checkout_asset(void) {
    if (loan_count >= MAX_LOANS) {
        printf("Loan record limit reached.\n");
        return;
    }
    show_inventory();
    int asset_id = read_int("\nEquipment ID to borrow: ", 1, 1000000);
    int index = find_asset_index(asset_id);
    if (index < 0) {
        printf("Equipment ID not found.\n");
        return;
    }
    if (assets[index].available == 0) {
        printf("Currently unavailable. Try again after a return.\n");
        return;
    }
    int quantity = read_int("Quantity to borrow: ", 1, assets[index].available);
    Loan l;
    l.id = 1;
    for (int i = 0; i < loan_count; i++)
        if (loans[i].id >= l.id) l.id = loans[i].id + 1;
    read_text("Borrower's name: ", l.borrower, sizeof(l.borrower));
    l.asset_id = asset_id;
    l.quantity = quantity;
    l.active = 1;
    loans[loan_count++] = l;
    assets[index].available -= quantity;
    save_assets();
    save_loans();
    printf("Checkout recorded. Loan ID: %d\n", l.id);
}

void return_asset(void) {
    printf("\nACTIVE LOANS\n");
    printf("%-8s %-8s %-22s %-8s\n", "LOAN ID", "ASSET", "BORROWER", "QTY");
    printf("------------------------------------------------\n");
    int active_found = 0;
    for (int i = 0; i < loan_count; i++) {
        if (loans[i].active) {
            printf("%-8d %-8d %-22s %-8d\n", loans[i].id, loans[i].asset_id,
                   loans[i].borrower, loans[i].quantity);
            active_found = 1;
        }
    }
    if (!active_found) {
        printf("There are no active loans.\n");
        return;
    }

    int loan_id = read_int("Loan ID to return: ", 1, 1000000);
    int index = find_loan_index(loan_id);
    if (index < 0 || !loans[index].active) {
        printf("Active loan not found.\n");
        return;
    }
    int asset_index = find_asset_index(loans[index].asset_id);
    if (asset_index < 0) {
        printf("Inventory record is missing; return was not processed.\n");
        return;
    }
    assets[asset_index].available += loans[index].quantity;
    if (assets[asset_index].available > assets[asset_index].total)
        assets[asset_index].available = assets[asset_index].total;
    loans[index].active = 0;
    save_assets();
    save_loans();
    printf("Return recorded successfully.\n");
}

void show_active_loans(void) {
    printf("\nACTIVE CHECKOUTS\n");
    printf("%-8s %-8s %-22s %-8s\n", "LOAN ID", "ASSET", "BORROWER", "QTY");
    printf("------------------------------------------------\n");
    int found = 0;
    for (int i = 0; i < loan_count; i++) {
        if (loans[i].active) {
            printf("%-8d %-8d %-22s %-8d\n", loans[i].id, loans[i].asset_id,
                   loans[i].borrower, loans[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("No active checkouts.\n");
}

void search_asset(void) {
    char key[NAME_LEN];
    read_text("Search equipment name: ", key, sizeof(key));
    int found = 0;
    printf("\nMATCHING EQUIPMENT\n");
    for (int i = 0; i < asset_count; i++) {
        if (strstr(assets[i].name, key) != NULL) {
            printf("ID: %d | %s | Available: %d/%d\n", assets[i].id,
                   assets[i].name, assets[i].available, assets[i].total);
            found = 1;
        }
    }
    if (!found) printf("No matching equipment found. (Search is case-sensitive.)\n");
}

void show_summary(void) {
    int total_units = 0, available_units = 0, active_records = 0;
    for (int i = 0; i < asset_count; i++) {
        total_units += assets[i].total;
        available_units += assets[i].available;
    }
    for (int i = 0; i < loan_count; i++)
        if (loans[i].active) active_records++;

    printf("\nLAB SUMMARY\n");
    printf("Equipment types : %d\n", asset_count);
    printf("Total units     : %d\n", total_units);
    printf("Available units : %d\n", available_units);
    printf("Borrowed units  : %d\n", total_units - available_units);
    printf("Active loans    : %d\n", active_records);
}

void show_menu(void) {
    printf("\n========== LABLEDGER ==========\n");
    printf("1. View equipment inventory\n");
    printf("2. Add equipment\n");
    printf("3. Borrow equipment\n");
    printf("4. Return equipment\n");
    printf("5. View active loans\n");
    printf("6. Search equipment\n");
    printf("7. Lab summary\n");
    printf("0. Exit\n");
    printf("===============================\n");
}

int main(void) {
    int choice;
    load_data();
    printf("Welcome to LabLedger - Offline Lab Equipment Tracker\n");

    do {
        show_menu();
        choice = read_int("Choose an option: ", 0, 7);
        switch (choice) {
            case 1: show_inventory(); break;
            case 2: add_asset(); break;
            case 3: checkout_asset(); break;
            case 4: return_asset(); break;
            case 5: show_active_loans(); break;
            case 6: search_asset(); break;
            case 7: show_summary(); break;
            case 0: printf("Data saved. Goodbye!\n"); break;
        }
    } while (choice != 0);

    return 0;
}
