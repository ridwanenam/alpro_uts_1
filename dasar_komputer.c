#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PELANGGAN 100
#define MAX_LAUNDRY 100

#define BELUM_DIKONFIRMASI 0
#define SEDANG_DIKERJAKAN  1
#define SELESAI            2

struct Pelanggan {
    char username[50];
    char password[50];
    double saldo;
    int frekuensi;
};

struct Laundry {
    char jenis[50];
    int durasi;
    double berat;
    double harga;
    int status;
    char username[50];
    int frekuensi;
    int jumlahItem;
};

struct InfoPelanggan {
    char username[50];
    int banyakSetoran;
}; 

int pelangganIndexLogin = -1; 

void bersihkanLayar();

void simpanDataPelanggan(struct Pelanggan *pelanggan, int jumlahPelanggan);
void bacaDataPelanggan(struct Pelanggan *pelanggan, int *jumlahPelanggan);
void simpanDataLaundry(struct Laundry *laundry, int jumlahLaundry);
void bacaDataLaundry(struct Laundry *laundry, int *jumlahLaundry);

void loginAdmin(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int *jumlahPelanggan, int *jumlahLaundry);
void loginPelanggan(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int *jumlahPelanggan, int *jumlahLaundry);
void daftarPelanggan(struct Pelanggan *dataPelanggan, int *jumlahPelanggan);
int cekUsernamePelanggan(struct Pelanggan *dataPelanggan, int jumlahPelanggan, const char *username);
int cekLoginPelanggan(struct Pelanggan *dataPelanggan, int jumlahPelanggan, const char *username, const char *password);

// fungsi pada menu customer atau pelanggan
void isiSaldo(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int *jumlahLaundry);
void setorLaundry(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int *jumlahLaundry);
void lihatStatusLaundry(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int jumlahLaundry);
void cekHarga();

// fungsi pada menu admin
void hapusPelanggan(struct Pelanggan *dataPelanggan, int *jumlahPelanggan);
void lihatPendapatan(struct Laundry *dataLaundry, int jumlahLaundry);
void perbaruiStatusLaundry(struct Laundry *dataLaundry, struct Pelanggan *dataPelanggan, int jumlahLaundry);
void lihatHistoriPelanggan(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int jumlahLaundry);

void tampilkanMenuAdmin();
void tampilkanMenuPelanggan();

int main() {
    struct Pelanggan dataPelanggan[MAX_PELANGGAN];
    struct Laundry dataLaundry[MAX_LAUNDRY];
    int jumlahPelanggan = 0;
    int jumlahLaundry = 0;

    bacaDataPelanggan(dataPelanggan, &jumlahPelanggan);
    bacaDataLaundry(dataLaundry, &jumlahLaundry);

    while (1) {
        bersihkanLayar();
        int tipePengguna;
        printf("=====Selamat datang di D'Laundry :)=====\n");
        printf("Pilih tipe pengguna:\n");
        printf("1. Admin\n");
        printf("2. Pelanggan\n");
        printf("3. Keluar\n");
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &tipePengguna);

        switch (tipePengguna) {
            case 1:
                loginAdmin(dataPelanggan, dataLaundry, &jumlahPelanggan, &jumlahLaundry);
                break;
            case 2:
                printf("Pilih opsi pelanggan:\n");
                printf("1. Login\n");
                printf("2. Daftar\n");
                printf("Masukkan pilihan Anda: ");
                int opsiPelanggan;
                scanf("%d", &opsiPelanggan);

                switch (opsiPelanggan) {
                    case 1:
                        loginPelanggan(dataPelanggan, dataLaundry, &jumlahPelanggan, &jumlahLaundry);
                        break;
                    case 2:
                        daftarPelanggan(dataPelanggan, &jumlahPelanggan);
                        break;
                    default:
                        printf("Opsi tidak valid. Silakan coba lagi.\n");
                        break;
                }
                break;
            case 3:
                simpanDataPelanggan(dataPelanggan, jumlahPelanggan);
                simpanDataLaundry(dataLaundry, jumlahLaundry);
                printf("Terima kasih telah menggunakan D'Laundry. Sampai jumpa!!!\n");
                exit(0);
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                break;
        }
    }

    return 0;
}

// ======Implementasi fungsi-fungsi ======
void bersihkanLayar() {
    system("clear || cls");
}

void simpanDataPelanggan(struct Pelanggan *pelanggan, int jumlahPelanggan) {
    FILE *filePelanggan = fopen("pelanggan.dat", "wb");
    if (filePelanggan == NULL) {
        printf("Gagal membuka file pelanggan.\n");
        exit(1);
    }

    fwrite(&jumlahPelanggan, sizeof(int), 1, filePelanggan);
    fwrite(pelanggan, sizeof(struct Pelanggan), jumlahPelanggan, filePelanggan);
    fclose(filePelanggan);
}

void bacaDataPelanggan(struct Pelanggan *pelanggan, int *jumlahPelanggan) {
    FILE *filePelanggan = fopen("pelanggan.dat", "rb");
    if (filePelanggan == NULL) {
        printf("Gagal membuka file pelanggan.\n");
        return;
    }

    fread(jumlahPelanggan, sizeof(int), 1, filePelanggan);
    fread(pelanggan, sizeof(struct Pelanggan), *jumlahPelanggan, filePelanggan);
    fclose(filePelanggan);
}

void simpanDataLaundry(struct Laundry *laundry, int jumlahLaundry) {
    FILE *fileLaundry = fopen("laundry.dat", "wb");
    if (fileLaundry == NULL) {
        printf("Gagal membuka file laundry.\n");
        exit(1);
    }

    fwrite(&jumlahLaundry, sizeof(int), 1, fileLaundry);
    fwrite(laundry, sizeof(struct Laundry), jumlahLaundry, fileLaundry);
    fclose(fileLaundry);
}

void bacaDataLaundry(struct Laundry *laundry, int *jumlahLaundry) {
    FILE *fileLaundry = fopen("laundry.dat", "rb");
    if (fileLaundry == NULL) {
        printf("Gagal membuka file laundry.\n");
        return;
    }

    fread(jumlahLaundry, sizeof(int), 1, fileLaundry);
    fread(laundry, sizeof(struct Laundry), *jumlahLaundry, fileLaundry);
    fclose(fileLaundry);
}

void loginAdmin(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int *jumlahPelanggan, int *jumlahLaundry) {
    char username[50];
    char password[50];
    int loginSuccess = 0;
    int attempt = 0;
    const char adminUsername[] = "admin";
    const char adminPassword[] = "passwordadmin";

    do {
        printf("Masukkan username: ");
        scanf("%s", username);
        printf("Masukkan password: ");
        scanf("%s", password);

        if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
            printf("Login berhasil!\n");
            loginSuccess = 1;
        } else {
            printf("Login gagal. Silakan coba lagi.\n");
            attempt++;
        }
    } while (!loginSuccess && attempt < 3);

    if (!loginSuccess) {
        printf("Anda telah melebihi batas percobaan login. Aplikasi akan keluar.\n");
        simpanDataPelanggan(dataPelanggan, *jumlahPelanggan);
        simpanDataLaundry(dataLaundry, *jumlahLaundry);
        exit(0);
    }

    int opsiAdmin;
    do {
        tampilkanMenuAdmin();
        scanf("%d", &opsiAdmin);

        switch (opsiAdmin) {

            case 1:
                hapusPelanggan(dataPelanggan, jumlahPelanggan);
                break;
            case 2:
                lihatPendapatan(dataLaundry, *jumlahLaundry);
                break;
            case 3:
                perbaruiStatusLaundry(dataLaundry, dataPelanggan, *jumlahLaundry);
                break;
            case 4:
                lihatHistoriPelanggan(dataPelanggan, dataLaundry, *jumlahPelanggan, *jumlahLaundry);
                break;
            case 5:
                printf("Keluar dari akun admin.\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (opsiAdmin != 5);
}

void loginPelanggan(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int *jumlahPelanggan, int *jumlahLaundry) {
    char username[50];
    char password[50];
    int loginSuccess = 0;
    int attempt = 0;

    do {
        printf("Masukkan username: ");
        scanf("%s", username);
        printf("Masukkan password: ");
        scanf("%s", password);

        loginSuccess = cekLoginPelanggan(dataPelanggan, *jumlahPelanggan, username, password);

        if (loginSuccess) {
            for (int i = 0; i < *jumlahPelanggan; i++) {
                if (strcmp(dataPelanggan[i].username, username) == 0) {
                    pelangganIndexLogin = i; 
                    break;
                }
            }
        }

        if (!loginSuccess) {
            printf("Login gagal. Silakan coba lagi.\n");
            attempt++;
        }
    } while (!loginSuccess && attempt < 3);

    if (!loginSuccess) {
        printf("Anda telah melebihi batas percobaan login. Aplikasi akan keluar.\n");
        simpanDataPelanggan(dataPelanggan, *jumlahPelanggan);
        simpanDataLaundry(dataLaundry, *jumlahLaundry);
        exit(0);
    }

    int opsiPelanggan;
    do {
        tampilkanMenuPelanggan();
        scanf("%d", &opsiPelanggan);

        switch (opsiPelanggan) {
            case 1:
                isiSaldo(dataPelanggan, dataLaundry, *jumlahPelanggan, jumlahLaundry);
                break;
            case 2:
                setorLaundry(dataPelanggan, dataLaundry, *jumlahPelanggan, jumlahLaundry);
                break;
            case 3:
                lihatStatusLaundry(dataPelanggan, dataLaundry, *jumlahPelanggan, *jumlahLaundry);
                break;
            case 4:
                cekHarga();
                break;
            case 5:
                printf("Keluar dari akun pelanggan.\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (opsiPelanggan != 5);
}


void daftarPelanggan(struct Pelanggan *dataPelanggan, int *jumlahPelanggan) {
    printf("Masukkan username: ");
    scanf("%s", dataPelanggan[*jumlahPelanggan].username);

    while (cekUsernamePelanggan(dataPelanggan, *jumlahPelanggan, dataPelanggan[*jumlahPelanggan].username)) {
        printf("Username sudah digunakan. Masukkan username lain: ");
        scanf("%s", dataPelanggan[*jumlahPelanggan].username);
    }

    printf("Masukkan password: ");
    scanf("%s", dataPelanggan[*jumlahPelanggan].password);
    dataPelanggan[*jumlahPelanggan].saldo = 0;

    (*jumlahPelanggan)++;

    char tambahLagi;
    printf("Tambah pelanggan lagi? (y/n): ");
    scanf(" %c", &tambahLagi);

    if (tambahLagi == 'y' || tambahLagi == 'Y') {
        daftarPelanggan(dataPelanggan, jumlahPelanggan);
    }
}

int cekUsernamePelanggan(struct Pelanggan *dataPelanggan, int jumlahPelanggan, const char *username) {
    for (int i = 0; i < jumlahPelanggan; i++) {
        if (strcmp(dataPelanggan[i].username, username) == 0) {
            return 1; 
        }
    }
    return 0;
}

int cekLoginPelanggan(struct Pelanggan *dataPelanggan, int jumlahPelanggan, const char *username, const char *password) {
    for (int i = 0; i < jumlahPelanggan; i++) {
        if (strcmp(dataPelanggan[i].username, username) == 0 && strcmp(dataPelanggan[i].password, password) == 0) {
            return 1; 
        }
    }
    return 0; 
}


void isiSaldo(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int *jumlahLaundry) {
    if (pelangganIndexLogin != -1) {
        int pilihan;
        printf("Pilih opsi:\n");
        printf("1. Isi Saldo\n");
        printf("2. Cek Saldo\n");
        printf("Masukkan pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: {
                double jumlahSaldoDitambahkan;
                printf("Masukkan jumlah saldo yang ingin diisi: ");
                scanf("%lf", &jumlahSaldoDitambahkan);

                // Tambah saldo
                dataPelanggan[pelangganIndexLogin].saldo += jumlahSaldoDitambahkan;

                printf("Saldo berhasil diisi. Saldo sekarang: %.2lf\n", dataPelanggan[pelangganIndexLogin].saldo);
                break;
            }

            case 2:
                // Cek Saldo
                printf("Saldo pelanggan %s: %.2lf\n", dataPelanggan[pelangganIndexLogin].username,
                       dataPelanggan[pelangganIndexLogin].saldo);
                break;

            default:
                printf("Pilihan tidak valid.\n");
                break;
        }

        for (int i = 0; i < *jumlahLaundry; i++) {
            if (strcmp(dataPelanggan[pelangganIndexLogin].username, dataLaundry[i].jenis) == 0 &&
                dataLaundry[i].status == SELESAI) {
                dataPelanggan[pelangganIndexLogin].saldo -= dataLaundry[i].harga;
                printf("Saldo berkurang setelah laundry selesai. Saldo sekarang: %.2lf\n",
                       dataPelanggan[pelangganIndexLogin].saldo);
            }
        }
    } else {
        printf("Anda belum login sebagai pelanggan.\n");
    }
}


void setorLaundry(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int *jumlahLaundry) {
    if (pelangganIndexLogin == -1) {
        printf("Anda belum login sebagai pelanggan. Silakan login terlebih dahulu.\n");
        return;
    }

    if (dataPelanggan[pelangganIndexLogin].saldo <= 0) {
        printf("Saldo tidak mencukupi untuk melakukan laundry.\n");
        return;
    }

    if (*jumlahLaundry >= MAX_LAUNDRY) {
        printf("Kapasitas laundry penuh. Tidak dapat menyetor laundry lebih banyak.\n");
        return;
    }

    strcpy(dataLaundry[*jumlahLaundry].username, dataPelanggan[pelangganIndexLogin].username);

    printf("Pilih jenis laundry:\n");
    printf("===Cuci Kiloan===\n");
    printf("1. Cuci Kiloan Reguler\n");
    printf("2. Cuci Kiloan Express\n");
    printf("===HouseHold===\n");
    printf("3. Sprei/Selimut/Sarung Bantal & Guling\n");
    printf("4. Bed Cover Kecil\n");
    printf("5. Bed Cover Sedang\n");
    printf("6. Bed Cover Besar\n");
    int jenis;
    printf("Masukkan nomor jenis laundry: ");
    scanf("%d", &jenis);

    double hargaPerKilogram;
    double hargaPerItem;  // Harga per item untuk jenis laundry tertentu
    int jumlahItem;  // Jumlah item untuk jenis laundry tertentu

    switch (jenis) {
    case 1:
        hargaPerKilogram = 6000.0;
        strcpy(dataLaundry[*jumlahLaundry].jenis, "Cuci Kiloan Reguler");
        dataLaundry[*jumlahLaundry].durasi = 2;  // Default durasi
        break;
    case 2:
        hargaPerKilogram = 8000.0;
        strcpy(dataLaundry[*jumlahLaundry].jenis, "Cuci Kiloan Express");
        dataLaundry[*jumlahLaundry].durasi = 1;  // Default durasi
        break;
    case 3:
        hargaPerItem = 15000.0;
        strcpy(dataLaundry[*jumlahLaundry].jenis, "Sprei/Selimut/Sarung Bantal & Guling");
        break;
    case 4:
        hargaPerItem = 10000.0;
        strcpy(dataLaundry[*jumlahLaundry].jenis, "Bed Cover Kecil");
        break;
    case 5:
        hargaPerItem = 13000.0;
        strcpy(dataLaundry[*jumlahLaundry].jenis, "Bed Cover Sedang");
        break;
    case 6:
        hargaPerItem = 16000.0;
        strcpy(dataLaundry[*jumlahLaundry].jenis, "Bed Cover Besar");
        break;
    default:
        printf("Nomor jenis laundry tidak valid.\n");
        return;
    }

    if (jenis >= 3 && jenis <= 6) {
        // Input jumlah item
        printf("Jumlah item: ");
        scanf("%d", &dataLaundry[*jumlahLaundry].jumlahItem);
    }

    // Input berat laundry jika bukan jenis household
    if (jenis == 1 || jenis == 2) {
        printf("Berat laundry (dalam kilogram): ");
        scanf("%lf", &dataLaundry[*jumlahLaundry].berat);
    }

    if (jenis == 1 || jenis == 2) {
        dataLaundry[*jumlahLaundry].harga = dataLaundry[*jumlahLaundry].berat * hargaPerKilogram;
    } else if (jenis >= 3 && jenis <= 6) {
        dataLaundry[*jumlahLaundry].harga = hargaPerItem * dataLaundry[*jumlahLaundry].jumlahItem;
    }


    dataLaundry[*jumlahLaundry].status = BELUM_DIKONFIRMASI;

    dataPelanggan[pelangganIndexLogin].saldo -= dataLaundry[*jumlahLaundry].harga;
    printf("Laundry berhasil disetor. Harga laundry: %.2lf\n", dataLaundry[*jumlahLaundry].harga);

    simpanDataLaundry(dataLaundry, *jumlahLaundry + 1);

    (*jumlahLaundry)++;
}


void lihatStatusLaundry(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int jumlahLaundry) {
    if (pelangganIndexLogin == -1) {
        printf("Anda belum login sebagai pelanggan. Silakan login terlebih dahulu.\n");
        return;
    }

    printf("Status Laundry untuk Pelanggan %s:\n", dataPelanggan[pelangganIndexLogin].username);

    for (int i = 0; i < jumlahLaundry; i++) {
        if (strcmp(dataPelanggan[pelangganIndexLogin].username, dataLaundry[i].username) == 0) {
            printf("Laundry %d:\n", i + 1);
            printf("Status: ");

            if (dataLaundry[i].status == BELUM_DIKONFIRMASI) {
                printf("Belum dikonfirmasi\n");
            } else if (dataLaundry[i].status == SEDANG_DIKERJAKAN) {
                printf("Sedang dikerjakan\n");
            } else {
                printf("Selesai\n");
            }

            if (strcmp(dataLaundry[i].jenis, "Cuci Kiloan Reguler") == 0 || strcmp(dataLaundry[i].jenis, "Cuci Kiloan Express") == 0) {
                // Jenis CUCI KILOAN
                printf("Jenis: %s, Durasi: %d hari, Berat: %.2lf kg, Harga: %.2lf\n",
                       dataLaundry[i].jenis, dataLaundry[i].durasi, dataLaundry[i].berat,
                       dataLaundry[i].harga);
            } else {
                // Jenis HOUSEHOLD
                printf("Jenis: %s, Jumlah Item: %d, Harga: %.2lf\n",
                       dataLaundry[i].jenis, dataLaundry[i].jumlahItem,
                       dataLaundry[i].harga);
            }

            printf("\n");
        }
    }
}

void cekHarga() {
    printf("=== Cek Harga Laundry ===\n");
    printf("Pilih jenis laundry:\n");
    printf("===Cuci Kiloan===\n");
    printf("1. Cuci Kiloan Reguler\n");
    printf("2. Cuci Kiloan Express\n");
    printf("===HouseHold===\n");
    printf("3. Sprei/Selimut/Sarung Bantal & Guling\n");
    printf("4. Bed Cover Kecil\n");
    printf("5. Bed Cover Sedang\n");
    printf("6. Bed Cover Besar\n");

    int jenis;
    printf("Masukkan nomor jenis laundry: ");
    scanf("%d", &jenis);

    switch (jenis) {
        case 1:
            printf("Harga Cuci Kiloan Reguler: Rp. 6000.0 per kilogram\n");
            break;
        case 2:
            printf("Harga Cuci Kiloan Express: Rp. 8000.0 per kilogram\n");
            break;
        case 3:
            printf("Harga Sprei/Selimut/Sarung Bantal & Guling: Rp. 15000.0 per item\n");
            break;
        case 4:
            printf("Harga Bed Cover Kecil: Rp. 10000.0 per item\n");
            break;
        case 5:
            printf("Harga Bed Cover Sedang: Rp. 13000.0 per item\n");
            break;
        case 6:
            printf("Harga Bed Cover Besar: Rp. 16000.0 per item\n");
            break;
        default:
            printf("Nomor jenis laundry tidak valid.\n");
            break;
    }
}


void hapusPelanggan(struct Pelanggan *dataPelanggan, int *jumlahPelanggan) {
    char username[50];

    printf("Masukkan username pelanggan yang akan dihapus: ");
    scanf("%s", username);

    int indexPelanggan = -1;
    for (int i = 0; i < *jumlahPelanggan; i++) {
        if (strcmp(dataPelanggan[i].username, username) == 0) {
            indexPelanggan = i;
            break;
        }
    }

    if (indexPelanggan != -1) {
        if (indexPelanggan < (*jumlahPelanggan) - 1) {
            memmove(&dataPelanggan[indexPelanggan], &dataPelanggan[indexPelanggan + 1],
                    ((*jumlahPelanggan) - indexPelanggan - 1) * sizeof(struct Pelanggan));
        }

        (*jumlahPelanggan)--;

        printf("Akun pelanggan %s berhasil dihapus.\n", username);
    } else {
        printf("Username pelanggan tidak ditemukan.\n");
    }
}


void lihatPendapatan(struct Laundry *dataLaundry, int jumlahLaundry) {
    double totalPendapatan = 0.0;

    for (int i = 0; i < jumlahLaundry; i++) {
        if (dataLaundry[i].status == SELESAI) {
            totalPendapatan += dataLaundry[i].harga;
        }
    }

    printf("Total Pendapatan: Rp. %.2f\n", totalPendapatan);
}


void perbaruiStatusLaundry(struct Laundry *dataLaundry, struct Pelanggan *dataPelanggan, int jumlahLaundry) {
    char username[50];

    printf("Masukkan username pelanggan: ");
    scanf("%s", username);

    int found = 0;
    for (int i = 0; i < jumlahLaundry; i++) {
        if (strcmp(dataLaundry[i].username, username) == 0) {
            printf("Informasi Laundry Sebelum Diperbarui:\n");
            printf("Username: %s, Jenis: %s, ", dataLaundry[i].username, dataLaundry[i].jenis);

            if (strcmp(dataLaundry[i].jenis, "Cuci Kiloan Reguler") == 0 || strcmp(dataLaundry[i].jenis, "Cuci Kiloan Express") == 0) {
                printf("Durasi: %d hari, Berat: %.2f kg, Harga: Rp. %.2f, Status: %d\n",
                       dataLaundry[i].durasi, dataLaundry[i].berat, dataLaundry[i].harga, dataLaundry[i].status);
            } else {
                printf("Jumlah Item: %d, Status: %d, Harga: Rp. %.2lf\n", dataLaundry[i].jumlahItem, dataLaundry[i].status, dataLaundry[i].harga);
            }

            printf("Masukkan status baru (0: belum dikonfirmasi, 1: sedang dikerjakan, 2: selesai): ");
            scanf("%d", &dataLaundry[i].status);

            printf("Informasi Laundry Setelah Diperbarui:\n");
            printf("Username: %s, Jenis: %s, ", dataLaundry[i].username, dataLaundry[i].jenis);

            if (strcmp(dataLaundry[i].jenis, "Cuci Kiloan Reguler") == 0 || strcmp(dataLaundry[i].jenis, "Cuci Kiloan Express") == 0) {
                printf("Durasi: %d hari, Berat: %.2f kg, Harga: Rp. %.2f, Status: %d\n",
                       dataLaundry[i].durasi, dataLaundry[i].berat, dataLaundry[i].harga, dataLaundry[i].status);
            } else {
                printf("Jumlah Item: %d, Status: %d, Harga: Rp. %.2lf\n", dataLaundry[i].jumlahItem, dataLaundry[i].status, dataLaundry[i].harga);
            }

            printf("Status laundry pelanggan %s berhasil diperbarui.\n", username);
            found = 1;

            simpanDataLaundry(dataLaundry, jumlahLaundry);
        }
    }

    if (!found) {
        printf("Laundry untuk pelanggan %s tidak ditemukan.\n", username);
    }
}


void lihatHistoriPelanggan(struct Pelanggan *dataPelanggan, struct Laundry *dataLaundry, int jumlahPelanggan, int jumlahLaundry) {
    printf("Histori Laundry Pelanggan yang Sudah Selesai:\n");

    struct InfoPelanggan infoPelanggan[jumlahPelanggan];
    for (int i = 0; i < jumlahPelanggan; i++) {
        infoPelanggan[i].banyakSetoran = 0;
        strcpy(infoPelanggan[i].username, dataPelanggan[i].username);
    }

    for (int i = 0; i < jumlahLaundry; i++) {
        if (dataLaundry[i].status == SELESAI) {
            for (int j = 0; j < jumlahPelanggan; j++) {
                if (strcmp(dataLaundry[i].username, infoPelanggan[j].username) == 0) {
                    infoPelanggan[j].banyakSetoran++;
                    break;
                }
            }
        }
    }

    // Bubble Sort
    for (int i = 0; i < jumlahPelanggan - 1; i++) {
        for (int j = 0; j < jumlahPelanggan - i - 1; j++) {
            if (infoPelanggan[j].banyakSetoran < infoPelanggan[j + 1].banyakSetoran) {
                struct InfoPelanggan tempInfo = infoPelanggan[j];
                infoPelanggan[j] = infoPelanggan[j + 1];
                infoPelanggan[j + 1] = tempInfo;
            }
        }
    }

    for (int i = 0; i < jumlahPelanggan; i++) {
        printf("Pelanggan: %s\n", infoPelanggan[i].username);
        printf("Banyaknya setoran laundry: %d\n", infoPelanggan[i].banyakSetoran);

        for (int j = 0; j < jumlahLaundry; j++) {
            if (strcmp(dataLaundry[j].username, infoPelanggan[i].username) == 0 && dataLaundry[j].status == SELESAI) {
                printf("Status: Selesai\n");

                if (strcmp(dataLaundry[j].jenis, "Cuci Kiloan Reguler") == 0 || strcmp(dataLaundry[j].jenis, "Cuci Kiloan Express") == 0) {
                    // Jenis CUCI KILOAN
                    printf("Jenis: %s, Durasi: %d hari, Berat: %.2f kg, Harga: Rp. %.2f\n",
                           dataLaundry[j].jenis, dataLaundry[j].durasi, dataLaundry[j].berat,
                           dataLaundry[j].harga);
                } else {
                    // Jenis HOUSEHOLD
                    printf("Jenis: %s, Jumlah Item: %d, Harga: Rp. %.2f\n",
                           dataLaundry[j].jenis, dataLaundry[j].jumlahItem,
                           dataLaundry[j].harga);
                }

                printf("\n");
            }
        }
    }
}

void tampilkanMenuAdmin() {
    printf("Menu Admin:\n");
    printf("1. Hapus Akun Customer\n"); // done
    printf("2. Lihat Pendapatan Laundry\n"); // done
    printf("3. Perbarui Status Laundry Customer\n"); // done
    printf("4. Lihat Histori Pelanggan\n"); // done
    printf("5. Simpan dan Keluar\n"); 
    printf("Masukkan pilihan Anda: ");
}

void tampilkanMenuPelanggan() {
    printf("Menu Pelanggan:\n");
    printf("1. Isi Saldo atau Cek Saldo\n"); // done
    printf("2. Setor Laundry\n"); // done
    printf("3. Lihat Status Laundry\n"); // done
    printf("4. Cek Harga\n"); // done
    printf("5. Simpan dan Keluar\n"); 
    printf("Masukkan pilihan Anda: ");
}
