#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "products.txt"
#define MAX_LINE_LENGTH 128

// Mahsulot strukturasini aniqlash
typedef struct {
    int id;
    char name[50];
    double price;
    int quantity;
} Product;

// Funksiyalar prototiplari
void show_menu();
void add_product();
void list_products();
void update_product();
void delete_product();
void exit_program();
int load_products();
void save_products();
void clear_buffer();

// Global o'zgaruvchilar
Product products[100];
int product_count = 0;

int main() {
    char command[20];

    printf("Market boshqaruv tizimiga xush kelibsiz!\n");

    // Fayldan mahsulotlarni yuklash
    product_count = load_products();

    while (1) {
        show_menu();
        printf("Buyruqni kiriting: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Yangi satrni olib tashlash

        if (strcmp(command, "add") == 0) {
            add_product();
        } else if (strcmp(command, "list") == 0) {
            list_products();
        } else if (strcmp(command, "update") == 0) {
            update_product();
        } else if (strcmp(command, "delete") == 0) {
            delete_product();
        } else if (strcmp(command, "exit") == 0) {
            exit_program();
        } else {
            printf("Xato: Noto'g'ri buyruq kiritildi.\n");
        }
    }

    return 0;
}

void show_menu() {
    printf("\n--- Menyu ---\n");
    printf("add     - Mahsulot qo'shish\n");
    printf("list    - Mahsulotlarni ko'rish\n");
    printf("update  - Mahsulotni yangilash\n");
    printf("delete  - Mahsulotni o'chirish\n");
    printf("exit    - Dasturdan chiqish\n");
}

void add_product() {
    Product new_product;
    new_product.id = product_count + 1;

    printf("Mahsulot nomini kiriting: ");
    fgets(new_product.name, sizeof(new_product.name), stdin);
    new_product.name[strcspn(new_product.name, "\n")] = 0;

    printf("Mahsulot narxini kiriting: ");
    scanf("%lf", &new_product.price);
    printf("Mahsulot sonini kiriting: ");
    scanf("%d", &new_product.quantity);
    clear_buffer();

    products[product_count++] = new_product;

    save_products();

    printf("Mahsulot muvaffaqiyatli qo'shildi!\n");
}

void list_products() {
    if (product_count == 0) {
        printf("Hozircha mahsulotlar mavjud emas.\n");
        return;
    }

    printf("\n--- Mahsulotlar ro'yxati ---\n");
    printf("ID   Nomi         Narxi      Soni\n");
    for (int i = 0; i < product_count; i++) {
        printf("%-4d %-12s %-10.2f %d\n", 
               products[i].id, 
               products[i].name, 
               products[i].price, 
               products[i].quantity);
    }
}

void update_product() {
    int id;
    printf("Yangilash uchun mahsulot ID sini kiriting: ");
    scanf("%d", &id);
    clear_buffer();

    if (id < 1 || id > product_count) {
        printf("Xato: Noto'g'ri ID kiritildi.\n");
        return;
    }

    Product *product = &products[id - 1];
    printf("Yangi narxni kiriting: ");
    scanf("%lf", &product->price);
    printf("Yangi miqdorni kiriting: ");
    scanf("%d", &product->quantity);
    clear_buffer();

    save_products();

    printf("Mahsulot muvaffaqiyatli yangilandi!\n");
}

void delete_product() {
    int id;
    printf("O'chirish uchun mahsulot ID sini kiriting: ");
    scanf("%d", &id);
    clear_buffer();

    if (id < 1 || id > product_count) {
        printf("Xato: Noto'g'ri ID kiritildi.\n");
        return;
    }

    for (int i = id - 1; i < product_count - 1; i++) {
        products[i] = products[i + 1];
    }
    product_count--;

    save_products();

    printf("Mahsulot muvaffaqiyatli o'chirildi!\n");
}

void exit_program() {
    printf("Dastur tugatildi. Yaxshi kunlar!\n");
    exit(0);
}

int load_products() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d,%49[^,],%lf,%d\n", 
                  &products[count].id, 
                  products[count].name, 
                  &products[count].price, 
                  &products[count].quantity) == 4) {
        count++;
    }

    fclose(file);
    return count;
}

void save_products() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Xato: Faylni yozib bo'lmadi.\n");
        return;
    }

    for (int i = 0; i < product_count; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", 
                products[i].id, 
                products[i].name, 
                products[i].price, 
                products[i].quantity);
    }

    fclose(file);
}

void clear_buffer() {
    while (getchar() != '\n');
}