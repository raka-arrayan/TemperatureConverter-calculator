#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

// Function declarations
void createAccount();
bool login(char *email, char *password);
void convertTemperature(char *email, bool isLoggedIn);
int checkCredits(char *email);
bool updateCredits(char *email, int creditChange);
bool isVoucherValid(char *voucherCode);
void useVoucher(char *email, char *voucherCode);
void addCredits(char *email);
float inputSuhu();
char inputSkala();
float konversiSuhu(float suhu, char skalaAwal, char skalaAkhir);
void outputHasil(float suhuAwal, char skalaAwal, float suhuAkhir, char skalaAkhir);
long long int perkalian(int a, int b);
int  kalkulator2Bilangan();

int main() {
    char email[50], password[20];
    int choice;
    bool isLoggedIn = false;
    int conversionAttempts = 0; // Menghitung percobaan konversi tanpa login

    printf("Selamat datang di Kalkulator 2 bilangan dan Suhu\n");

    while (true) {
        printf("1. Buat akun\n");
        printf("2. Login\n");
        printf("3. Konversi Suhu (Maksimal 3 kali tanpa login)\n");
        printf("4. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                printf("Email: ");
                scanf("%s", email);
                printf("Password: ");
                scanf("%s", password);
                isLoggedIn = login(email, password);
                if (isLoggedIn) {
                    printf("Login berhasil. Selamat datang!\n");
                    int loggedInChoice;
                    while (true) {
                        printf("Menu:\n");
                        printf("1. Konversi Suhu\n");
                        printf("2. Tambah Kredit\n");
                        printf("3. Kalkulator 2 Bilangan\n");
                        printf("4. Logout\n");
                        printf("Pilihan Anda: ");
                        scanf("%d", &loggedInChoice);

                        switch (loggedInChoice) {
                            case 1:
                                convertTemperature(email, true);
                                break;
                            case 2:
                                addCredits(email);
                                break;
                            case 3:
                                kalkulator2Bilangan();
                                break;
                            case 4:
                                printf("Anda telah logout.\n");
                                isLoggedIn = false;
                                break;
                            default:
                                printf("Pilihan tidak valid.\n");
                                break;
                        }
                        if (!isLoggedIn) break;
                    }
                } else {
                    printf("Login gagal.\n");
                }
                break;
            case 3:
                if (!isLoggedIn && conversionAttempts < 3) {
                    convertTemperature(email, false);
                    conversionAttempts++;
                } else if (isLoggedIn) {
                    convertTemperature(email, true);
                } else {
                    printf("Anda telah mencapai batas konversi tanpa login. Silakan buat akun atau login untuk melanjutkan.\n");
                }
                break;
            case 4:
                printf("Terima kasih telah menggunakan Kalkulator.\n");
                return 0;
            default:
                printf("Pilihan tidak valid.\n");
        }
    }

    return 0;
}

void createAccount() {
    char email[50], password[20];
    printf("Membuat akun baru\n");
    printf("Masukkan email: ");
    scanf("%s", email);
    printf("Masukkan password: ");
    scanf("%s", password);

    FILE *file = fopen("accounts.txt", "a");
    if (file == NULL){
        printf("Gagal membuka file akun!\n");
        return;
    }
    fprintf(file, "%s %s\n", email, password);
    fclose(file);

    // Setelah membuat akun, tambahkan kredit awal ke file kredit
    file = fopen("credits.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file kredit!\n");
        return;
    }
    fprintf(file, "%s %d\n", email, 10);  // Setiap akun baru mendapatkan 10 kredit
    fclose(file);

    printf("Akun berhasil dibuat dengan 10 kredit awal.\n");
}

bool login(char *email, char *password) {
    char emailStored[50], passwordStored[20];

    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return false;
    }

    while (fscanf(file, "%s %s", emailStored, passwordStored) != EOF) {
        if (strcmp(email, emailStored) == 0 && strcmp(password, passwordStored) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

void convertTemperature(char *email, bool isLoggedIn) {
    if (isLoggedIn && checkCredits(email) <= 0) {
        printf("Kredit tidak cukup untuk melakukan konversi.\n");
        return;
    }

    float suhuAwal = inputSuhu();
    char skalaAwal = inputSkala();
    char skalaAkhir;

    printf("Masukkan skala tujuan (C/R/K/F): ");
    scanf(" %c", &skalaAkhir);

    float suhuAkhir = konversiSuhu(suhuAwal, skalaAwal, skalaAkhir);

    if (isLoggedIn) {
        updateCredits(email, -1); // Kurangi 1 kredit untuk pengguna yang login
    }
    outputHasil(suhuAwal, skalaAwal, suhuAkhir, skalaAkhir);
}

int checkCredits(char *email) {
    char emailStored[50];
    int credits;

    FILE *file = fopen("credits.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file credits.txt\n");
        return 0;
    }

    while (fscanf(file, "%s %d", emailStored, &credits) != EOF) {
        if (strcmp(email, emailStored) == 0) {
            fclose(file);
            return credits;
        }
    }

    fclose(file);
    return 0; // Tidak ditemukan kredit atau pengguna
}

bool updateCredits(char *email, int creditChange) {
    FILE *file = fopen("credits.txt", "r+");
    if (file == NULL) {
        printf("Gagal membuka file credits.txt\n");
        return false;
    }

    bool updated = false;
    char emailStored[50];
    int credits;

    while (fscanf(file, "%s %d", emailStored, &credits) != EOF) {
        if (strcmp(email, emailStored) == 0) {
            long pos = ftell(file) - (long)sizeof(int); // Seek to credit value position
            fseek(file, pos + 2, SEEK_SET);
            fprintf(file, " %d\n", credits + creditChange); // Update credit and write to file
            updated = true;
            break;
        }
    }

    fclose(file);

    if (updated) {
        printf("Kredit berhasil diperbarui.\n");
    } else {
        printf("Gagal memperbarui kredit.\n");
    }

    return updated;
}

bool isVoucherValid(char *voucherCode) {
    char codeStored[20];

    FILE *file = fopen("vouchers.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return false;
    }

    while (fscanf(file, "%s", codeStored) != EOF) {
        if (strcmp(voucherCode, codeStored) == 0) {
            fclose(file);
            return true; // Voucher ditemukan dan valid
        }
    }

    fclose(file);
    return false; // Voucher tidak ditemukan atau tidak valid
}

void useVoucher(char *email, char *voucherCode) {
    // Logika untuk mencatat penggunaan voucher
    FILE *file = fopen("used_vouchers.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }
    fprintf(file, "%s %s\n", voucherCode, email);
    fclose(file);
}

void addCredits(char *email) {
    char voucherCode[20];
    printf("Masukkan kode voucher: ");
    scanf("%s", voucherCode);

    if (isVoucherValid(voucherCode)) {
        useVoucher(email, voucherCode);
        updateCredits(email, 10); // Misal setiap voucher menambahkan 10 kredit
        printf("Voucher berhasil digunakan. Kredit ditambahkan.\n");
    } else {
        printf("Voucher tidak valid atau sudah digunakan.\n");
    }
}

float inputSuhu() {
    float suhu;
    printf("Masukkan nilai suhu: ");
    scanf("%f", &suhu);
    return suhu;
}

char inputSkala() {
    char skala;
    printf("Masukkan skala awal (C/R/K/F): ");
    scanf(" %c", &skala);
    return skala;
}

float konversiSuhu(float suhu, char skalaAwal, char skalaAkhir) {
    if (skalaAwal == 'C') {
        switch (skalaAkhir) {
            case 'C': return suhu;
            case 'R': return suhu * 4 / 5;
            case 'K': return suhu + 273.15;
            case 'F': return (suhu * 9 / 5) + 32;
        }
    } else if (skalaAwal == 'R') {
        switch (skalaAkhir) {
            case 'C': return suhu * 5 / 4;
            case 'R': return suhu;
            case 'K': return (suhu * 5 / 4) + 273.15;
            case 'F': return (suhu * 9 / 4) + 32;
        }
    } else if (skalaAwal == 'K') {
        switch (skalaAkhir) {
            case 'C': return suhu - 273.15;
            case 'R': return (suhu - 273.15) * 4 / 5;
            case 'K': return suhu;
            case 'F': return (suhu - 273.15) * 9 / 5 + 32;
        }
    } else if (skalaAwal == 'F') {
        switch (skalaAkhir) {
            case 'C': return (suhu - 32) * 5 / 9;
            case 'R': return (suhu - 32) * 4 / 9;
            case 'K': return (suhu - 32) * 5 / 9 + 273.15;
            case 'F': return suhu;
        }
    }
    return 0; // Default return value
}

void outputHasil(float suhuAwal, char skalaAwal, float suhuAkhir, char skalaAkhir) {
    printf("Hasil konversi: %.2f%c menjadi %.2f%c\n", suhuAwal, skalaAwal, suhuAkhir, skalaAkhir);
}

long long int perkalian(int a, int b) {
    long long int result = 0; 
    int *arr = (int *)malloc(b * sizeof(int)); 
    int i;

    if (arr == NULL) {
        printf("Gagal mengalokasikan memori\n");
        return -1;
    }

    for (int i = 0; i < b; i++) {
        arr[i] = a; 
    }

    int from, to, numt, tid; 
    #pragma omp parallel private(tid, numt,i, to, from)
    {
        // Pembagian rentang penjumlahan beruntun pada loop otomatis
        long long int localresult = 0; 
        tid = omp_get_thread_num(); 
        numt = omp_get_num_threads(); 
        from = (b / numt) * tid; 
        to = (b / numt) * (tid + 1) - 1; 
        if (tid == numt - 1) to = b - 1; //ini untuk thread terakhir

        // Membagi penjumlahan beruntun menjadi Task yang bisa diparalelkan
        #pragma omp task 
        {
            #pragma omp reduction(+:localresult) // Menggunakan reduction variable 
            for (i = from; i <= to; i++) {
                localresult += arr[i]; // Melakukan perkalian dengan penjumlahan beruntun dengan array
            }
            // Menambahkan localresult ke dalam hasil akhir 
            result += localresult;
        }
    }

    free(arr); 
    return result;
}

int kalkulator2Bilangan() {
    int angka1, angka2;

    printf("Kalkulator Perkalian Sederhana\n");
    printf("Masukkan dua bilangan (salah satu bilangan harus >= 1000):\n");
    printf("Angka 1: ");
    scanf("%d", &angka1);
    printf("Angka 2: ");
    scanf("%d", &angka2);

    if (angka1 < 1000 && angka2 < 1000) {
        printf("Salah satu bilangan harus >= 1000 !!!\n");
        return 1;
    }

    int besar, kecil;
    if (angka1 > angka2) {
        besar = angka1;
        kecil = angka2;
    } else {
        besar = angka2;
        kecil = angka1;
    }

    int result = perkalian(kecil, besar);

    if (result != -1) { 
        printf("Hasil: %d\n", result);
    }
}