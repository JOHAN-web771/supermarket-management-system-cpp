#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>

using namespace std;


void bersihkanInput(){
    cin.clear();
    cin.ignore(10000, '\n'); // buang sisa input sampai 10000 karakter atau sampai newline
}

int nomorStrukSekarang = 1;

int ambilNomorBaru(){
    return nomorStrukSekarang++;
}

string llToStr(long long angka){
    char buf[32];
    sprintf(buf, "%lld", angka);
    return string(buf);
}

string intToStr(int angka){
    char buf[16];
    sprintf(buf, "%d", angka);
    return string(buf);
}

string formatRupiah(long long angka){
    string s = llToStr(angka);
    string hasil = "";
    int hitung = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--){
        hasil = s[i] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i != 0)
            hasil = "." + hasil;
    }
    return hasil;
}

// ======================================================
// STRUCT Global
// ======================================================


struct Barang{
    string id;
    string nama;
    long long harga;
    int stok;
    string jenis;
    int terjual; // total unit yang sudah pernah terjual
};

struct ItemTransaksi{
    string nama;
    long long harga;
    int qty;
    long long subtotal;
};

// STRUCT Global

struct Supplier{
    string nama;
    string alamat;
    string telepon;
};


struct RiwayatTransaksi{
    string noStruk;
    long long subtotal;
    long long diskon;
    long long total;
    string metode;
    int jumlahItem;
    
    // Tambahan
    string idTransaksi;
    string tanggal;
    double totalBelanja;
};

struct DataRetur{
    string noStruk;
    string namaBarang;
    string alasan;
    string status;
};

const int MAX_BARANG    = 100;
const int MAX_SUPPLIER  = 100;
const int MAX_TRANSAKSI = 100;
const int MAX_RETUR     = 100;

Barang           daftarBarang[MAX_BARANG];
Supplier         daftarSupplier[MAX_SUPPLIER];
RiwayatTransaksi daftarTransaksi[MAX_TRANSAKSI];
DataRetur        daftarRetur[MAX_RETUR];

int jumlahBarang    = 0;
int jumlahSupplier  = 0;
int jumlahTransaksi = 0;
int jumlahRetur     = 0;


// ======================================================
// STRUCT CUSTOMER : Johan Hanggara Hasiholan
// ======================================================

struct DataUtama {
    string nama;
    string email;
    string noHp;      
    string username;
    string password;
    string ulang;
};

// STRUCT RETUR : Johan Hanggara Hasiholan


// ======================================================
// STRUCT ADMIN : Ahmad Faeruz Salim
// ======================================================

struct Karyawan {
    string id;
    string nama;
    string role;
    string shift;
};

struct Promo {
    string kodePromo;
    double diskonPersen;
    string status;
};

struct Delivery {
    string idResi;
    string namaPenerima;
    string kotaTujuan;
    double ongkosKirim;
};

//struct RiwayatTransaksi {
//    string idTransaksi;
//    string tanggal;
//    double totalBelanja;
//};
//
//struct DataRetur {
//    string idRetur;
//    string idTransaksi;
//    string skuBarang;
//    string alasan;
//};



// ======================================================
// CLASS ADMIN : Ahmad Faeruz Salim
// ======================================================

class MenuAdmin {
private:
    string usernameAdmin;
    string pinAdmin;
    
    Karyawan daftarKaryawan[100];
    int jumlahKaryawan;
    
    Promo daftarPromo[50];
    int jumlahPromo;

    Delivery daftarDelivery[100];
    int jumlahDelivery;
    
    RiwayatTransaksi daftarTransaksi[100];
    int jumlahTransaksi;

    DataRetur daftarRetur[100];
    int jumlahRetur;

	// Fungsi Login Admin (Validasi username dan PIN admin)
	// Banyak baris kode : 31 baris
    void validasiKeamanan() {
        string inputUser, inputPin;
        bool aksesDiberikan = false;
        
        while (!aksesDiberikan) {
            cout << "\n======================================" << endl;
            cout << "          LOGIN ADMIN SISTEM          " << endl;
            cout << "======================================" << endl;
            cout << "Username : ";
            cin >> inputUser;
            cout << "PIN      : ";
            cin >> inputPin;
            
            if (inputUser == usernameAdmin && inputPin == pinAdmin) {
                aksesDiberikan = true;
                cout << "\nLogin Berhasil! Selamat datang." << endl;
            } else {
                cout << "Akses Ditolak! Kredensial salah." << endl;
            }
        }
    }

	// Fungsi Load Data Karyawan (Membaca data karyawan dari file)
	// Banyak baris kode : 26 baris
    void loadDataKaryawan() {
        ifstream file("dataKaryawan.txt");
        jumlahKaryawan = 0;
        if (file.is_open()) {
            string id, nama, role, shift;
            while (file >> id >> nama >> role >> shift) {
                if (jumlahKaryawan < 100) {
                    daftarKaryawan[jumlahKaryawan].id = id;
                    daftarKaryawan[jumlahKaryawan].nama = nama;
                    daftarKaryawan[jumlahKaryawan].role = role;
                    daftarKaryawan[jumlahKaryawan].shift = shift;
                    jumlahKaryawan++;
                }
            }
            file.close();
        }
    }

	 // Fungsi Save Data Karyawan (Menyimpan data karyawan ke file)
	// Banyak baris kode : 18 baris
    void saveDataKaryawan() {
        ofstream file("dataKaryawan.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahKaryawan; i++) {
                file << daftarKaryawan[i].id << ";" 
                     << daftarKaryawan[i].nama << ";" 
                     << daftarKaryawan[i].role << ";" 
                     << daftarKaryawan[i].shift << "\n";
            }
            file.close();
        }
    }

	// Fungsi Load Data Promo (Membaca data promo dari file)
	// Banyak baris kode : 25 baris
    void loadDataPromo() {
        ifstream file("data_promo.txt");
        jumlahPromo = 0;
        if (file.is_open()) {
            string kode, status;
            double persen;
            while (file >> kode >> persen >> status) {
                if (jumlahPromo < 50) {
                    daftarPromo[jumlahPromo].kodePromo = kode;
                    daftarPromo[jumlahPromo].diskonPersen = persen;
                    daftarPromo[jumlahPromo].status = status;
                    jumlahPromo++;
                }
            }
            file.close();
        }
    }

	// Fungsi Save Data Promo (Menyimpan data promo ke file)
	// Banyak baris kode : 18 baris
    void saveDataPromo() {
        ofstream file("data_promo.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahPromo; i++) {
                file << daftarPromo[i].kodePromo << ";" 
                     << daftarPromo[i].diskonPersen << ";" 
                     << daftarPromo[i].status << "\n";
            }
            file.close();
        }
    }

	// Fungsi Load Data Delivery (Membaca data pengiriman)
	// Banyak baris kode : 28 baris
    void loadDataDelivery() {
        ifstream file("data_delivery.txt");
        jumlahDelivery = 0;
        if (file.is_open()) {
            string resi, nama, kota;
            double ongkir;
            while (file >> resi >> nama >> kota >> ongkir) {
                if (jumlahDelivery < 100) {
                    daftarDelivery[jumlahDelivery].idResi = resi;
                    daftarDelivery[jumlahDelivery].namaPenerima = nama;
                    daftarDelivery[jumlahDelivery].kotaTujuan = kota;
                    daftarDelivery[jumlahDelivery].ongkosKirim = ongkir;
                    jumlahDelivery++;
                }
            }
            file.close();
        }
    }

	// Fungsi Save Data Delivery (Menyimpan data pengiriman)
	// Banyak baris kode : 20 baris
    void saveDataDelivery() {
        ofstream file("data_delivery.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahDelivery; i++) {
                file << daftarDelivery[i].idResi << ";" 
                     << daftarDelivery[i].namaPenerima << ";" 
                     << daftarDelivery[i].kotaTujuan << ";" 
                     << daftarDelivery[i].ongkosKirim << "\n";
            }
            file.close();
        }
    }

	// Fungsi Load Data Transaksi (Membaca riwayat transaksi)
	// Banyak baris kode : 25 baris
    void loadDataTransaksi() {
        ifstream file("transaksi.txt");
        jumlahTransaksi = 0;
        if (file.is_open()) {
            string id, tgl;
            double total;
            while (file >> id >> tgl >> total) {
                if (jumlahTransaksi < 100) {
                    daftarTransaksi[jumlahTransaksi].idTransaksi = id;
                    daftarTransaksi[jumlahTransaksi].tanggal = tgl;
                    daftarTransaksi[jumlahTransaksi].totalBelanja = total;
                    jumlahTransaksi++;
                }
            }
            file.close();
        }
    }

	// Fungsi Load Data Retur (Membaca data retur barang)
	// Banyak baris kode : 25 baris
    void loadDataRetur() {
        ifstream file("retur.txt");
        jumlahRetur = 0;
        if (file.is_open()) {
            string idR, idT, sku, alsn;
            while (file >> idR >> idT >> sku >> alsn) {
                if (jumlahRetur < 100) {
                    daftarRetur[jumlahRetur].idRetur = idR;
                    daftarRetur[jumlahRetur].idTransaksi = idT;
                    daftarRetur[jumlahRetur].skuBarang = sku;
                    daftarRetur[jumlahRetur].alasan = alsn;
                    jumlahRetur++;
                }
            }
            file.close();
        }
    }

	// Fungsi Save Data Retur (Menyimpan data retur barang)
	// Banyak baris kode : 20 baris
    void saveDataRetur() {
        ofstream file("retur.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahRetur; i++) {
                file << daftarRetur[i].idRetur << ";" 
                     << daftarRetur[i].idTransaksi << ";" 
                     << daftarRetur[i].skuBarang << ";" 
                     << daftarRetur[i].alasan << "\n";
            }
            file.close();
        }
    }

	// Fungsi Pause Layar (Memberi jeda sebelum lanjut)
	// Banyak baris kode : 5 baris
    void pauseLayar() {
        cout << "\nTekan Enter untuk melanjutkan...";
        cin.ignore(10000, '\n');
        cin.get();
    }

	// Fungsi Bersihkan Input (Membersihkan buffer input)
	// Banyak baris kode : 5 baris
    void bersihkanInput() {
        cin.clear();
        cin.ignore(10000, '\n');
    }

public:
	// Konstruktor Menu Admin (Inisialisasi data admin)
	// Banyak baris kode : 10 baris
    MenuAdmin() {
        usernameAdmin = "salim";
        pinAdmin = "1207";
        
        loadDataKaryawan();
        loadDataPromo();
        loadDataDelivery();
        loadDataTransaksi();
        loadDataRetur();
    }

	// Fungsi Registrasi Karyawan (Menambahkan data karyawan)
	// Banyak baris kode : 33 baris
    void registrasiKaryawan() {
        cout << "\n=== [1] REGISTRASI KARYAWAN ===" << endl;
        
        if (jumlahKaryawan >= 100) {
            cout << "Kapasitas data karyawan penuh!" << endl;
            pauseLayar();
            return;
        }

        cout << "ID Karyawan     : ";
        cin >> daftarKaryawan[jumlahKaryawan].id;
        cout << "Nama (1 Kata)   : ";
        cin >> daftarKaryawan[jumlahKaryawan].nama;
        cout << "Posisi/Role     : ";
        cin >> daftarKaryawan[jumlahKaryawan].role;
        
        daftarKaryawan[jumlahKaryawan].shift = "Belum_Diatur";
        
        jumlahKaryawan++;
        saveDataKaryawan();
        
        cout << "Data berhasil disimpan ke sistem!" << endl;
        pauseLayar();
    }

	// Fungsi Atur Shift Karyawan (Mengatur jadwal shift kerja)
	// Banyak baris kode : 95 baris
    void aturShiftKaryawan() {
        bool menuShift = true;
        while (menuShift) {
            cout << "\n=== [2] ATUR SHIFT KARYAWAN ===" << endl;
            cout << "1. Lihat Jadwal Shift" << endl;
            cout << "2. Ubah Shift Karyawan" << endl;
            cout << "3. Kembali ke Menu Utama" << endl;
            cout << "Pilih : ";
            
            int pilihan;
            if (!(cin >> pilihan)) {
                bersihkanInput();
                continue;
            }

            if (pilihan == 1) {
                if (jumlahKaryawan == 0) {
                    cout << "Data karyawan kosong." << endl;
                } else {
                    cout << "\n" << left << setw(5) << "No" << setw(15) << "ID" << setw(20) << "Nama" << setw(15) << "Shift" << endl;
                    cout << "--------------------------------------------------------" << endl;
                    for (int i = 0; i < jumlahKaryawan; i++) {
                        cout << left << setw(5) << i + 1 
                             << setw(15) << daftarKaryawan[i].id 
                             << setw(20) << daftarKaryawan[i].nama 
                             << setw(15) << daftarKaryawan[i].shift << endl;
                    }
                }
                pauseLayar();
            } else if (pilihan == 2) {
                if (jumlahKaryawan == 0) {
                    cout << "Data karyawan kosong." << endl;
                    pauseLayar();
                    continue;
                }
                
                int noKar;
                cout << "Masukkan Nomor Urut Karyawan : ";
                cin >> noKar;
                
                if (noKar > 0 && noKar <= jumlahKaryawan) {
                    int index = noKar - 1;
                    int shiftPil;
                    cout << "1. Pagi (07:00-15:00)\n2. Siang (15:00-23:00)\n3. Malam (23:00-07:00)\nPilih Shift : ";
                    cin >> shiftPil;

                    if (shiftPil == 1) daftarKaryawan[index].shift = "Pagi";
                    else if (shiftPil == 2) daftarKaryawan[index].shift = "Siang";
                    else if (shiftPil == 3) daftarKaryawan[index].shift = "Malam";
                    else cout << "Pilihan tidak valid." << endl;
                    
                    saveDataKaryawan();
                    cout << "Shift " << daftarKaryawan[index].nama << " berhasil diubah!" << endl;
                } else {
                    cout << "Nomor tidak valid!" << endl;
                }
                pauseLayar();
            } else if (pilihan == 3) {
                menuShift = false;
            }
        }
    }

	// Fungsi Kelola Diskon Promo (Mengatur promo dan diskon)
	// Banyak baris kode : 84 baris
    void kelolaDiskonPromo() {
        bool menuPromo = true;
        while (menuPromo) {
            cout << "\n=== [3] KELOLA DISKON & PROMO ===" << endl;
            cout << "1. Tambah Promo Baru" << endl;
            cout << "2. Lihat Daftar Promo" << endl;
            cout << "3. Kembali ke Menu Utama" << endl;
            cout << "Pilih : ";
            
            int pilihan;
            if (!(cin >> pilihan)) {
                bersihkanInput();
                continue;
            }

            if (pilihan == 1) {
                if (jumlahPromo >= 50) {
                    cout << "Kapasitas data promo penuh!" << endl;
                } else {
                    cout << "Kode Promo (Tanpa Spasi) : ";
                    cin >> daftarPromo[jumlahPromo].kodePromo;
                    cout << "Besaran Diskon (%)       : ";
                    cin >> daftarPromo[jumlahPromo].diskonPersen;
                    daftarPromo[jumlahPromo].status = "Aktif";
                    
                    jumlahPromo++;
                    saveDataPromo();
                    cout << "Promo berhasil ditambahkan dan diaktifkan!" << endl;
                }
                pauseLayar();
            } else if (pilihan == 2) {
                if (jumlahPromo == 0) {
                    cout << "Belum ada promo yang terdaftar." << endl;
                } else {
                    cout << "\n" << left << setw(20) << "Kode Promo" << setw(15) << "Diskon (%)" << setw(15) << "Status" << endl;
                    cout << "--------------------------------------------------" << endl;
                    for (int i = 0; i < jumlahPromo; i++) {
                        cout << left << setw(20) << daftarPromo[i].kodePromo 
                             << setw(15) << daftarPromo[i].diskonPersen 
                             << setw(15) << daftarPromo[i].status << endl;
                    }
                }
                pauseLayar();
            } else if (pilihan == 3) {
                menuPromo = false;
            }
        }
    }
    
    
	// Fungsi Kelola Delivery (Mengatur pengiriman barang)
	// Banyak baris kode : 96 baris
    void kelolaDelivery() {
        bool menuDelivery = true;
        while (menuDelivery) {
            cout << "\n=== [4] KELOLA DELIVERY ===" << endl;
            cout << "1. Input Resi Pengiriman Baru" << endl;
            cout << "2. Lihat Daftar Pengiriman" << endl;
            cout << "3. Kembali ke Menu Utama" << endl;
            cout << "Pilih : ";
            
            int pilihan;
            if (!(cin >> pilihan)) {
                bersihkanInput();
                continue;
            }

            if (pilihan == 1) {
                if (jumlahDelivery >= 100) {
                    cout << "Kapasitas data pengiriman penuh!" << endl;
                } else {
                    cout << "Nomor Resi             : ";
                    cin >> daftarDelivery[jumlahDelivery].idResi;
                    cout << "Nama Penerima (1 Kata) : ";
                    cin >> daftarDelivery[jumlahDelivery].namaPenerima;
                    cout << "Kota Tujuan (1 Kata)   : ";
                    cin >> daftarDelivery[jumlahDelivery].kotaTujuan;
                    
                    double jarak;
                    cout << "Estimasi Jarak (Km)    : ";
                    cin >> jarak;
                    
                    daftarDelivery[jumlahDelivery].ongkosKirim = jarak * 2500;
                    
                    jumlahDelivery++;
                    saveDataDelivery();
                    cout << "Data pengiriman disimpan. Ongkos kirim: Rp " << fixed << setprecision(0) << daftarDelivery[jumlahDelivery-1].ongkosKirim << endl;
                }
                pauseLayar();
            } else if (pilihan == 2) {
                if (jumlahDelivery == 0) {
                    cout << "Belum ada data pengiriman." << endl;
                } else {
                    cout << "\n" << left << setw(15) << "Resi" << setw(20) << "Penerima" << setw(15) << "Kota" << setw(15) << "Ongkir (Rp)" << endl;
                    cout << "-----------------------------------------------------------------" << endl;
                    for (int i = 0; i < jumlahDelivery; i++) {
                        cout << left << setw(15) << daftarDelivery[i].idResi 
                             << setw(20) << daftarDelivery[i].namaPenerima 
                             << setw(15) << daftarDelivery[i].kotaTujuan 
                             << setw(15) << fixed << setprecision(0) << daftarDelivery[i].ongkosKirim << endl;
                    }
                }
                pauseLayar();
            } else if (pilihan == 3) {
                menuDelivery = false;
            }
        }
    }

	// Fungsi Laporan Penjualan (Menampilkan data penjualan)
	// Banyak baris kode : 38 baris
    void lihatLaporanPenjualan() {
        cout << "\n=== [5] LIHAT LAPORAN PENJUALAN ===" << endl;
        if (jumlahTransaksi == 0) {
            cout << "Belum ada data transaksi penjualan." << endl;
        } else {
            cout << left << setw(5) << "No" << setw(20) << "ID Transaksi" << setw(15) << "Tanggal" << setw(20) << "Total Belanja (Rp)" << endl;
            cout << "------------------------------------------------------------" << endl;
            double totalKeseluruhan = 0;
            for (int i = 0; i < jumlahTransaksi; i++) {
                cout << left << setw(5) << i + 1 
                     << setw(20) << daftarTransaksi[i].idTransaksi 
                     << setw(15) << daftarTransaksi[i].tanggal 
                     << setw(20) << fixed << setprecision(0) << daftarTransaksi[i].totalBelanja << endl;
                totalKeseluruhan += daftarTransaksi[i].totalBelanja;
            }
            cout << "------------------------------------------------------------" << endl;
            cout << "TOTAL PENDAPATAN KESELURUHAN : Rp " << fixed << setprecision(0) << totalKeseluruhan << endl;
        }
        pauseLayar();
    }

	// Fungsi Laporan Keuangan (Menghitung laba dan pengeluaran)
	// Banyak baris kode : 52 baris
    void lihatLaporanKeuangan() {
        cout << "\n============================================================" << endl;
        cout << "             LAPORAN KEUANGAN SISTEM KASIR                  " << endl;
        cout << "============================================================" << endl;

        double totalPendapatan = 0;
        for (int i = 0; i < jumlahTransaksi; i++) {
            totalPendapatan += daftarTransaksi[i].totalBelanja;
        }

        double bebanDelivery = 0;
        for (int i = 0; i < jumlahDelivery; i++) {
            bebanDelivery += daftarDelivery[i].ongkosKirim;
        }

        double bebanRetur = jumlahRetur * 50000; 
        double totalPengeluaran = bebanDelivery + bebanRetur;
        double labaBersih = totalPendapatan - totalPengeluaran;

        cout << "[1] PENDAPATAN" << endl;
        cout << "    Total Penjualan Barang    : Rp " << fixed << setprecision(0) << totalPendapatan << endl;
        cout << "------------------------------------------------------------" << endl;

        cout << "\n[2] PENGELUARAN (ESTIMASI)" << endl;
        cout << "    Beban Ongkos Delivery     : Rp " << fixed << setprecision(0) << bebanDelivery << endl;
        cout << "    Beban Proses Retur        : Rp " << fixed << setprecision(0) << bebanRetur << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << "    TOTAL PENGELUARAN         : Rp " << fixed << setprecision(0) << totalPengeluaran << endl;

        cout << "\n============================================================" << endl;
        cout << "LABA BERSIH                   : Rp " << fixed << setprecision(0) << labaBersih << endl;
        if (labaBersih >= 0) {
            cout << "STATUS                        : UNTUNG" << endl;
        } else {
            cout << "STATUS                        : RUGI" << endl;
        }
        cout << "============================================================" << endl;
        
        pauseLayar();
    }

	// Fungsi Kelola Retur (Mengatur retur barang customer)
	// Banyak baris kode : 88 baris
    void kelolaRetur() {
        bool menuRetur = true;
        while (menuRetur) {
            cout << "\n=== [7] KELOLA RETUR ===" << endl;
            cout << "1. Input Data Retur Baru" << endl;
            cout << "2. Lihat Daftar Retur" << endl;
            cout << "3. Kembali ke Menu Utama" << endl;
            cout << "Pilih : ";
            
            int pilihan;
            if (!(cin >> pilihan)) {
                bersihkanInput();
                continue;
            }

            if (pilihan == 1) {
                if (jumlahRetur >= 100) {
                    cout << "Kapasitas data retur penuh!" << endl;
                } else {
                    cout << "ID Retur               : ";
                    cin >> daftarRetur[jumlahRetur].idRetur;
                    cout << "ID Transaksi Terkait   : ";
                    cin >> daftarRetur[jumlahRetur].idTransaksi;
                    cout << "SKU Barang             : ";
                    cin >> daftarRetur[jumlahRetur].skuBarang;
                    cout << "Alasan (1 Kata)        : ";
                    cin >> daftarRetur[jumlahRetur].alasan;
                    
                    jumlahRetur++;
                    saveDataRetur();
                    cout << "Data retur berhasil dicatat ke sistem!" << endl;
                }
                pauseLayar();
            } else if (pilihan == 2) {
                if (jumlahRetur == 0) {
                    cout << "Belum ada data retur." << endl;
                } else {
                    cout << "\n" << left << setw(15) << "ID Retur" << setw(15) << "ID Transaksi" << setw(15) << "SKU Barang" << setw(20) << "Alasan" << endl;
                    cout << "----------------------------------------------------------------" << endl;
                    for (int i = 0; i < jumlahRetur; i++) {
                        cout << left << setw(15) << daftarRetur[i].idRetur 
                             << setw(15) << daftarRetur[i].idTransaksi 
                             << setw(15) << daftarRetur[i].skuBarang 
                             << setw(20) << daftarRetur[i].alasan << endl;
                    }
                }
                pauseLayar();
            } else if (pilihan == 3) {
                menuRetur = false;
            }
        }
    }

	// Fungsi Menu Utama Admin (Menampilkan seluruh menu admin)
	// Banyak baris kode : 58 baris
    void tampilkanMenu() {
        validasiKeamanan(); 
        
        int pilihan;
        bool isLogin = true;

        while (isLogin) {
            cout << "\n======================================" << endl;
            cout << "              MENU ADMIN" << endl;
            cout << "======================================" << endl;
            cout << "1. Registrasi Karyawan" << endl;
            cout << "2. Atur Shift Karyawan" << endl;
            cout << "3. Kelola Diskon & Promo" << endl;
            cout << "4. Kelola Delivery" << endl;
            cout << "5. Lihat Laporan Penjualan" << endl;
            cout << "6. Lihat Laporan Keuangan" << endl;
            cout << "7. Kelola Retur" << endl;
            cout << "8. Logout" << endl;
            cout << "======================================" << endl;
            cout << "Pilih Menu : ";
            
            if (!(cin >> pilihan)) {
                bersihkanInput();
                pilihan = 0;
            }

            switch (pilihan) {
                case 1: registrasiKaryawan(); break;
                case 2: aturShiftKaryawan(); break;
                case 3: kelolaDiskonPromo(); break;
                case 4: kelolaDelivery(); break;
                case 5: lihatLaporanPenjualan(); break;
                case 6: lihatLaporanKeuangan(); break;
                case 7: kelolaRetur(); break;
                case 8: 
                    cout << "\nMenutup sesi... Sampai jumpa!" << endl;
                    isLogin = false; 
                    break;
                default: 
                    cout << "\nPilihan tidak valid. Silakan coba lagi." << endl; 
                    pauseLayar();
                    break;
            }
        }
    }
};


// ======================================================
// CLASS KASIR : Hera Agung Nugroho
// ======================================================

class Kasir{
public:
	
	 // Konstruktor Kasir (Inisialisasi data transaksi & keranjang)
    // Banyak baris kode : 11 baris
    static const int MAX_KERANJANG = 50;
    ItemTransaksi keranjang[MAX_KERANJANG];
    int jumlahKeranjang;
    long long subtotal;
    long long nominalDiskon;
    string kodeDiskonAktif;

    Kasir(){
        jumlahKeranjang = 0;
        subtotal = 0;
        nominalDiskon = 0;
        kodeDiskonAktif = "-";
    }

	 // Fungsi Input Transaksi (Menambahkan barang ke keranjang transaksi)
    // Banyak baris kode : 88 baris
    void inputTransaksi(){
        jumlahKeranjang = 0;
        subtotal = 0;
        nominalDiskon = 0;
        kodeDiskonAktif = "-";

        cout << "\n========================================";
        cout << "\n           INPUT TRANSAKSI";
        cout << "\n========================================\n";

        char lanjut = 'y';
        while (lanjut == 'y' || lanjut == 'Y'){
            if (jumlahKeranjang >= MAX_KERANJANG){
                cout << "\n[GAGAL] Keranjang sudah penuh (maks " << MAX_KERANJANG << " item)!\n";
                break;
            }

            char sumber;
            cout << "\nAmbil barang dari data Gudang? (y/n) : ";
            cin >> sumber;

            ItemTransaksi item;

            if (sumber == 'y' || sumber == 'Y'){
                // -------- ambil barang langsung dari stok gudang --------
                string id;
                cout << "ID Barang di Gudang : ";
                cin >> id;

                int idx = -1;
                for (int i = 0; i < jumlahBarang; i++){
                    if (daftarBarang[i].id == id){ idx = i; break; }
                }

                if (idx == -1){
                    cout << "\n[GAGAL] Barang dengan ID tersebut tidak ada di gudang!\n";
                    cout << "Tambah barang lain? (y/n): ";
                    cin >> lanjut;
                    continue;
                }

                cout << "Nama Barang   : " << daftarBarang[idx].nama;
                cout << "\nHarga         : Rp" << formatRupiah(daftarBarang[idx].harga);
                cout << "\nStok Tersedia : " << daftarBarang[idx].stok;
                cout << "\nJumlah Beli   : ";
                cin >> item.qty;

                if (item.qty <= 0 || item.qty > daftarBarang[idx].stok){
                    cout << "\n[GAGAL] Jumlah tidak valid atau melebihi stok!\n";
                    cout << "Tambah barang lain? (y/n): ";
                    cin >> lanjut;
                    continue;
                }

                item.nama     = daftarBarang[idx].nama;
                item.harga    = daftarBarang[idx].harga;
                item.subtotal = item.harga * item.qty;

                daftarBarang[idx].stok    -= item.qty;
                daftarBarang[idx].terjual += item.qty;
            }else{
                // -------- input manual (barang di luar data gudang) --------
                bersihkanInput();
                cout << "Nama Barang   : ";
                getline(cin, item.nama);

                cout << "Harga Barang  : Rp";
                cin >> item.harga;

                cout << "Jumlah Barang : ";
                cin >> item.qty;

                item.subtotal = item.harga * item.qty;
            }

            keranjang[jumlahKeranjang] = item;
            jumlahKeranjang++;
            subtotal += item.subtotal;

            cout << "\n[ TAMBAH ] " << item.nama << " x" << item.qty
                 << " = Rp" << formatRupiah(item.subtotal);

            cout << "\nTambah barang lain? (y/n): ";
            cin >> lanjut;
        }

        cout << "\n========================================";
        cout << "\nSubtotal sementara: Rp" << formatRupiah(subtotal);
        cout << "\n========================================\n";
    }

	 // Fungsi Tampil Keranjang (Menampilkan isi keranjang belanja)
    // Banyak baris kode : 25 baris
    void tampilKeranjang(){
        cout << "\n========================================";
        cout << "\n              ISI KERANJANG";
        cout << "\n========================================\n";

        if (jumlahKeranjang == 0){
            cout << "Keranjang masih kosong.\n";
        }else{
            for (int i = 0; i < jumlahKeranjang; i++){
                cout << (i + 1) << ". " << left << setw(20) << keranjang[i].nama
                     << keranjang[i].qty << " x Rp" << formatRupiah(keranjang[i].harga)
                     << " = Rp" << formatRupiah(keranjang[i].subtotal) << "\n";
            }
            cout << "----------------------------------------\n";
            cout << "Subtotal sementara : Rp" << formatRupiah(subtotal) << "\n";
        }
        cout << "========================================\n";
    }

	  // Fungsi Void Item Keranjang (Menghapus item dari keranjang)
    // Banyak baris kode : 21 baris
    void voidItemKeranjang(){
        tampilKeranjang();
        if (jumlahKeranjang == 0) return;

        cout << "Pilih nomor item yang mau dibatalkan (0 = batal) : ";
        int no;
        cin >> no;

        if (no < 1 || no > jumlahKeranjang){
            cout << "\nTidak ada perubahan.\n";
            return;
        }

        int idx = no - 1;
        cout << "\n[ DIBATALKAN ] " << keranjang[idx].nama << " dikeluarkan dari keranjang.\n";
        subtotal -= keranjang[idx].subtotal;

        for (int i = idx; i < jumlahKeranjang - 1; i++){
            keranjang[i] = keranjang[i + 1];
        }
        jumlahKeranjang--;
    }

	// Fungsi Input Diskon (Validasi dan penerapan kode promo)
    // Banyak baris kode : 33 baris
    void inputDiskon(){
        cout << "\n========================================";
        cout << "\n             INPUT DISKON";
        cout << "\n========================================\n";

        if (jumlahKeranjang == 0){
            cout << "Belum ada transaksi berjalan. Input transaksi dahulu!\n";
            return;
        }

        string kode;
        cout << "Kode Diskon : ";
        cin >> kode;

        int persen = 0;
        if      (kode == "DISKON10") persen = 10;
        else if (kode == "DISKON20") persen = 20;
        else if (kode == "MEMBER5")  persen = 5;
        else                         persen = -1;

        if (persen == -1){
            cout << "\n[GAGAL] Kode diskon tidak valid.\n";
            kodeDiskonAktif = "-";
            nominalDiskon = 0;
        }else{
            kodeDiskonAktif = kode;
            nominalDiskon = subtotal * persen / 100;
            cout << "\n[VALIDASI BERHASIL] Diskon " << persen << "% diterapkan.";
            cout << "\nNominal Diskon : Rp" << formatRupiah(nominalDiskon);
        }
        cout << "\n========================================\n";
    }

	// Fungsi Input Pembayaran (Proses pembayaran transaksi)
    // Banyak baris kode : 57 baris
    void inputPembayaran()
    {
        if (jumlahKeranjang == 0){
            cout << "\nBelum ada transaksi berjalan. Input transaksi dahulu!\n";
            return;
        }

        long long total = subtotal - nominalDiskon;

        cout << "\n========================================";
        cout << "\n          INPUT PEMBAYARAN";
        cout << "\n========================================\n";
        cout << "Total yang harus dibayar : Rp" << formatRupiah(total) << "\n";

        string metode;
        cout << "Metode Pembayaran (CASH/QRIS/DEBIT) : ";
        cin >> metode;

        long long bayar;
        cout << "Nominal Bayar : Rp";
        cin >> bayar;

        if (bayar < total){
            cout << "\n[GAGAL] Nominal bayar kurang dari total belanja!\n";
            return;
        }

        long long kembalian = bayar - total;
        int noStrukInt = ambilNomorBaru();

        string noStruk = "TRX";
        if (noStrukInt < 10)       noStruk += "00" + intToStr(noStrukInt);
        else if (noStrukInt < 100) noStruk += "0"  + intToStr(noStrukInt);
        else                       noStruk +=        intToStr(noStrukInt);

        tampilStruk(noStruk, metode, total);

        if (jumlahTransaksi < MAX_TRANSAKSI){
            RiwayatTransaksi trx;
            trx.noStruk    = noStruk;
            trx.subtotal   = subtotal;
            trx.diskon     = nominalDiskon;
            trx.total      = total;
            trx.metode     = metode;
            trx.jumlahItem = jumlahKeranjang;
            daftarTransaksi[jumlahTransaksi] = trx;
            jumlahTransaksi++;
        }

        cout << "\n[ PROSES BERHASIL ]";
        cout << "\nKembalian : Rp" << formatRupiah(kembalian);
        cout << "\n========================================\n";

        jumlahKeranjang = 0;
        subtotal = 0;
        nominalDiskon = 0;
        kodeDiskonAktif = "-";
    }

	  // Fungsi Split Bill (Membagi total pembayaran beberapa orang)
    // Banyak baris kode : 25 baris
    void inputSplitBill(){
        cout << "\n========================================";
        cout << "\n             SPLIT BILL";
        cout << "\n========================================\n";

        long long totalBelanja;
        int jumlahOrang;

        cout << "Total Belanja : Rp";
        cin >> totalBelanja;

        cout << "Jumlah Orang  : ";
        cin >> jumlahOrang;

        if (jumlahOrang <= 0){
            cout << "\n[GAGAL] Jumlah orang harus lebih dari 0.\n";
            return;
        }

        outputSplitBill(totalBelanja, jumlahOrang);
    }

  	// Fungsi Input Retur (Mengajukan retur barang customer)
    // Banyak baris kode : 31 baris
    void inputRetur(){
        cout << "\n========================================";
        cout << "\n            RETUR BARANG";
        cout << "\n========================================\n";

        string noStruk, namaBarang, alasan;

        cout << "Nomor Struk  : ";
        cin >> noStruk;

        bersihkanInput();
        cout << "Nama Barang  : ";
        getline(cin, namaBarang);

        cout << "Alasan       : ";
        getline(cin, alasan);

        if (jumlahRetur < MAX_RETUR){
            DataRetur retur;
            retur.noStruk    = noStruk;
            retur.namaBarang = namaBarang;
            retur.alasan     = alasan;
            retur.status     = "Disetujui";
            daftarRetur[jumlahRetur] = retur;
            jumlahRetur++;
        }

        tampilNotaRetur(noStruk, namaBarang, "Disetujui");
    }

	// Fungsi Riwayat Transaksi (Menampilkan seluruh transaksi)
    // Banyak baris kode : 23 baris
    void riwayatTransaksi(){
        cout << "\n========================================";
        cout << "\n         RIWAYAT TRANSAKSI";
        cout << "\n========================================\n";

        if (jumlahTransaksi == 0){
            cout << "Belum ada riwayat transaksi.\n";
            cout << "========================================\n";
            return;
        }

        for (int i = 0; i < jumlahTransaksi; i++){
            RiwayatTransaksi &trx = daftarTransaksi[i];
            cout << left << setw(10) << trx.noStruk
                 << "Rp" << setw(12) << formatRupiah(trx.total)
                 << setw(8) << trx.metode
                 << trx.jumlahItem << " item\n";
        }
        cout << "========================================\n";
    }

	// Fungsi Cari Transaksi (Mencari transaksi berdasarkan no struk)
    // Banyak baris kode : 29 baris
    void cariTransaksi(){
        cout << "\n========================================";
        cout << "\n        CARI RIWAYAT TRANSAKSI";
        cout << "\n========================================\n";

        string noStruk;
        cout << "No Struk : ";
        cin >> noStruk;

        for (int i = 0; i < jumlahTransaksi; i++){
            if (daftarTransaksi[i].noStruk == noStruk){
                RiwayatTransaksi &trx = daftarTransaksi[i];
                cout << "\nNo Struk     : " << trx.noStruk;
                cout << "\nSubtotal     : Rp" << formatRupiah(trx.subtotal);
                cout << "\nDiskon       : Rp" << formatRupiah(trx.diskon);
                cout << "\nTotal        : Rp" << formatRupiah(trx.total);
                cout << "\nMetode Bayar : " << trx.metode;
                cout << "\nJumlah Item  : " << trx.jumlahItem;
                cout << "\n========================================\n";
                return;
            }
        }
        cout << "\n[GAGAL] Transaksi dengan no struk tersebut tidak ditemukan!\n";
    }

	 // Fungsi Statistik Penjualan (Menampilkan statistik transaksi)
    // Banyak baris kode : 40 baris
    void statistikPenjualan(){
        cout << "\n========================================";
        cout << "\n          STATISTIK PENJUALAN";
        cout << "\n========================================\n";

        if (jumlahTransaksi == 0){
            cout << "Belum ada transaksi.\n";
            cout << "========================================\n";
            return;
        }

        long long totalOmzet         = 0;
        long long transaksiTertinggi = daftarTransaksi[0].total;
        long long transaksiTerendah  = daftarTransaksi[0].total;
        string    noStrukTertinggi   = daftarTransaksi[0].noStruk;

        for (int i = 0; i < jumlahTransaksi; i++){
            totalOmzet += daftarTransaksi[i].total;

            if (daftarTransaksi[i].total > transaksiTertinggi){
                transaksiTertinggi = daftarTransaksi[i].total;
                noStrukTertinggi   = daftarTransaksi[i].noStruk;
            }
            if (daftarTransaksi[i].total < transaksiTerendah){
                transaksiTerendah = daftarTransaksi[i].total;
            }
        }

        long long rataRata = totalOmzet / jumlahTransaksi;

        cout << "Total Transaksi     : " << jumlahTransaksi;
        cout << "\nTotal Omzet         : Rp" << formatRupiah(totalOmzet);
        cout << "\nRata-rata/Transaksi : Rp" << formatRupiah(rataRata);
        cout << "\nTransaksi Tertinggi : Rp" << formatRupiah(transaksiTertinggi)
             << " (" << noStrukTertinggi << ")";
        cout << "\nTransaksi Terendah  : Rp" << formatRupiah(transaksiTerendah);
        cout << "\n========================================\n";
    }

	 // Fungsi Tampil Struk (Menampilkan struk pembayaran)
    // Banyak baris kode : 26 baris
    void tampilStruk(string noStruk, string metode, long long total){
        cout << "\n========================================";
        cout << "\n     ALFAMIDI KI AGENG PEMANAHAN";
        cout << "\n========================================";
        cout << "\nNo Struk : " << noStruk;
        cout << "\nKasir    : Mukhamma\n";

        for (int i = 0; i < jumlahKeranjang; i++){
            ItemTransaksi &item = keranjang[i];
            cout << "\n" << left << setw(20) << item.nama
                 << item.qty << " x Rp" << formatRupiah(item.harga)
                 << " = Rp" << formatRupiah(item.subtotal);
        }

        cout << "\n\nSubtotal      : Rp" << formatRupiah(subtotal);
        cout << "\nDiskon (" << kodeDiskonAktif << ") : Rp" << formatRupiah(nominalDiskon);
        cout << "\nTotal Belanja : Rp" << formatRupiah(total);
        cout << "\nPembayaran    : " << metode;
        cout << "\n========================================\n";
    }

	// Fungsi Tampil Nota Retur (Menampilkan nota retur barang)
    // Banyak baris kode : 14 baris
    void tampilNotaRetur(string noStruk, string namaBarang, string status){
        cout << "\n========================================";
        cout << "\n             NOTA RETUR";
        cout << "\n========================================";
        cout << "\nNo Struk : " << noStruk;
        cout << "\nBarang   : " << namaBarang;
        cout << "\nStatus   : " << status;
        cout << "\n========================================\n";
    }

	// Fungsi Output Split Bill (Menghitung pembagian pembayaran)
    // Banyak baris kode : 22 baris
    void outputSplitBill(long long totalBelanja, int jumlahOrang){
        long long perOrang = totalBelanja / jumlahOrang;
        long long sisa     = totalBelanja % jumlahOrang;

        cout << "\n========================================";
        cout << "\n           HASIL SPLIT BILL";
        cout << "\n========================================";
        cout << "\nTotal Belanja : Rp" << formatRupiah(totalBelanja);
        cout << "\nJumlah Orang  : " << jumlahOrang;
        cout << "\n----------------------------------------";

        for (int i = 1; i <= jumlahOrang; i++){
            long long bayar = perOrang + (i == jumlahOrang ? sisa : 0);
            cout << "\nOrang " << i << " : Rp" << formatRupiah(bayar);
        }
        cout << "\n========================================\n";
    }
};



// ======================================================
// CLASS GUDANG : Hera Agung Nugroho
// ======================================================

class Gudang{
public:

    // MEMBACA DATA DARI FILE
    void bacaDariFile(){

        ifstream inFile("katalogProduk.txt");

        if(!inFile.is_open()){
            return;
        }

        jumlahBarang = 0;

        string line;

        while(getline(inFile, line)){

            stringstream ss(line);

            Barang b;

            string hargaStr;
            string stokStr;
            string terjualStr;

            getline(ss, b.id, ';');
            getline(ss, b.nama, ';');
            getline(ss, b.jenis, ';');
            
            getline(ss, hargaStr, ';');
            getline(ss, stokStr, ';');
            getline(ss, terjualStr, ';');

            stringstream(hargaStr) >> b.harga;
            stringstream(stokStr) >> b.stok;
            stringstream(terjualStr) >> b.terjual;

            daftarBarang[jumlahBarang] = b;
            jumlahBarang++;
        }

        inFile.close();
    }
    
    	

    // SIMPAN DATA KE FILE
    void simpanKeFile(){

        ofstream outFile("katalogProduk.txt");

        if(!outFile.is_open()){
            return;
        }

        for(int i = 0; i < jumlahBarang; i++){

            outFile
            << daftarBarang[i].id << ";"
            << daftarBarang[i].nama << ";"
            << daftarBarang[i].jenis << ";"
            << daftarBarang[i].harga << ";"
            << daftarBarang[i].stok << ";"
            << daftarBarang[i].terjual
            << endl;
        }

        outFile.close();
    }
	
	// Fungsi Refresh Barang
		void refreshBarang(){

    	bacaDariFile();
	}


    int cariIndex(string id){
        for (int i = 0; i < jumlahBarang; i++){
            if (daftarBarang[i].id == id) return i;
        }
        return -1;
    }

	
    void inputBarangBaru(){
        Barang b;
        
        cout << "\n========================================";
        cout << "\n        INPUT BARANG BARU";
        cout << "\n========================================\n";

        cout << "ID Barang   : ";
        cin >> b.id;

        if (cariIndex(b.id) != -1){
            cout << "\n[GAGAL] ID barang sudah dipakai!\n";
            return;
        }

        bersihkanInput();
        
        cout << "Nama Barang : ";
        getline(cin, b.nama);
        
        cout << "Jenis Barang :";
        getline(cin, b.jenis);

        cout << "Harga       : Rp";
        cin >> b.harga;
        
        if(b.harga <= 0){
        	cout << "\n[GAGAL] Harga tidak valid\n";
        	return;
		}
        
        
        cout << "Stok        : ";
        cin >> b.stok;
        
        if(b.stok < 0){
        	cout << "\n[GAGAL] Stok tidak valid\n";
        	return;
		}
        
        

        b.terjual = 0;

        if (jumlahBarang >= MAX_BARANG){
            cout << "\n[GAGAL] Gudang penuh, tidak bisa menambah barang baru!\n";
            return;
        }

        daftarBarang[jumlahBarang] = b;
        jumlahBarang++;
        
        simpanKeFile();

        cout << "\n[ TERSIMPAN ] Barang berhasil ditambahkan ke gudang.";
        cout << "\n========================================\n";
    }


    void hapusBarang(){
        cout << "\n========================================";
        cout << "\n             HAPUS BARANG";
        cout << "\n========================================\n";

        string id;
        cout << "ID Barang : ";
        cin >> id;

        int idx = cariIndex(id);
        if (idx == -1){
            cout << "\n[GAGAL] Barang tidak ditemukan!\n";
            return;
        }

		if(daftarBarang[idx].stok > 0){
			cout << "\n[GAGAL] Barang masih memiliki stok!\n";
			return;
		}
		
        string nama = daftarBarang[idx].nama;
        
        for (int i = idx; i < jumlahBarang - 1; i++){
            daftarBarang[i] = daftarBarang[i + 1];
        }
        jumlahBarang--;
        
        simpanKeFile();

        cout << "\n[ SUKSES ] Barang " << nama << " berhasil dihapus dari gudang.\n";
    }


    void cariBarang(){
        cout << "\n========================================";
        cout << "\n              CARI BARANG";
        cout << "\n========================================\n";

        bersihkanInput();
        string kataKunci;
        cout << "Masukkan ID atau Nama Barang : ";
        getline(cin, kataKunci);

        bool ditemukan = false;
        for (int i = 0; i < jumlahBarang; i++){
            if (daftarBarang[i].id == kataKunci || daftarBarang[i].nama == kataKunci){
            	cout << left
            		 << setw(10) << daftarBarang[i].id
            		 << setw(10) << daftarBarang[i].nama
            		 << setw(10) <<  formatRupiah(daftarBarang[i].harga)
            		 << setw(10) << daftarBarang[i].stok
            		 << daftarBarang[i].terjual
            		 << endl;

                cout << "\n----------------------------------------\n";
                ditemukan = true;
            }
        }

        if (!ditemukan) cout << "\n[INFO] Barang tidak ditemukan.\n";
        cout << "========================================\n";
    }
	
	
	// Fungsi Input Restock (Menambahkan stok barang ())
    void inputRestock(){
        cout << "\n========================================";
        cout << "\n           INPUT RESTOCK";
        cout << "\n========================================\n";

        string id;
        cout << "ID Barang       : ";
        cin >> id;

        int idx = cariIndex(id);
        if (idx == -1){
            cout << "\n[GAGAL] Barang tidak ditemukan!\n";
            return;
        }

        int tambah;
        cout << "Nama Barang     : " << daftarBarang[idx].nama;
        cout << "\nStok Sekarang   : " << daftarBarang[idx].stok;
        cout << "\nJumlah Tambahan : ";
        cin >> tambah;
        
        if(tambah <= 0){
        	cout << "\n[GAGAL] Jumlah restock tidak valid\n";
        	return;
		}

        daftarBarang[idx].stok += tambah;
        
        simpanKeFile();

        cout << "\n[ SIMPAN ] Stok baru " << daftarBarang[idx].nama
             << " : " << daftarBarang[idx].stok;
        cout << "\n========================================\n";
    }

	// Fungsi saveSupplier()
	void saveSupplier(){

    ofstream file("supplier.txt");

    if(!file.is_open()){
        return;
    }

    for(int i = 0; i < jumlahSupplier; i++){

        file
        << daftarSupplier[i].nama << ";"
        << daftarSupplier[i].alamat << ";"
        << daftarSupplier[i].telepon
        << endl;
    }

    file.close();
}

	// Fungsi Load Supplier()
	void loadSupplier(){
		 ifstream file("supplier.txt");

    if(!file.is_open()){
        return;
    }

    jumlahSupplier = 0;

    string line;

    while(getline(file, line)){

        stringstream ss(line);

        Supplier s;

        getline(ss, s.nama, ';');
        getline(ss, s.alamat, ';');
        getline(ss, s.telepon, ';');

        daftarSupplier[jumlahSupplier] = s;
        jumlahSupplier++;
    }

    file.close();
}
	

	// Fungsi Input Supplier (Menambahkan data supplier)
    void inputSupplier(){
        cout << "\n========================================";
        cout << "\n          INPUT SUPPLIER";
        cout << "\n========================================\n";

        Supplier s;
        bersihkanInput();
        cout << "Nama Supplier : ";
        getline(cin, s.nama);

        cout << "Alamat        : ";
        getline(cin, s.alamat);

        cout << "Telepon       : ";
        getline(cin, s.telepon);

        if (jumlahSupplier >= MAX_SUPPLIER){
            cout << "\n[GAGAL] Data supplier penuh!\n";
            return;
        }

        daftarSupplier[jumlahSupplier] = s;
        jumlahSupplier++;
        
        saveSuplier();

        cout << "\n[ SIMPAN ] Data supplier berhasil disimpan.";
        cout << "\n========================================\n";
    }


	// Fungsi Tampil Supplier (Menampilkan data supplier)
	void tampilSupplier(){

    cout << "\n========================================";
    cout << "\n           DATA SUPPLIER";
    cout << "\n========================================\n";

    if(jumlahSupplier == 0){

        cout << "Belum ada data supplier.\n";
        cout << "========================================\n";
        return;
    }

    cout << left
         << setw(5)  << "No"
         << setw(20) << "Nama"
         << setw(30) << "Alamat"
         << setw(15) << "Telepon" << endl;

    cout << "-------------------------------------------------------------\n";

    for(int i = 0; i < jumlahSupplier; i++){

        cout << left
             << setw(5)  << i + 1
             << setw(20) << daftarSupplier[i].nama
             << setw(30) << daftarSupplier[i].alamat
             << setw(15) << daftarSupplier[i].telepon
             << endl;
    }

    cout << "========================================\n";
}

	// Fungsi Penyesuaian Harga (Mengubah harga barang)
    void inputPenyesuaianHarga(){
        cout << "\n========================================";
        cout << "\n        PENYESUAIAN HARGA";
        cout << "\n========================================\n";

        string id;
        cout << "ID Barang  : ";
        cin >> id;
        
        int idx = cariIndex(id);
        if (idx == -1){
            cout << "\n[GAGAL] Barang tidak ditemukan!\n";
            return;
        }

        long long hargaBaru;
        cout << "Nama Barang : " << daftarBarang[idx].nama;
        cout << "\nHarga Lama  : Rp" << formatRupiah(daftarBarang[idx].harga);
        cout << "\nHarga Baru  : Rp";
        cin >> hargaBaru;

		if(hargaBaru <= 0){
        	cout << "\n[GAGAL] Harga tidak valid!\n";
        	return;
		}
		
        daftarBarang[idx].harga = hargaBaru;
        
        simpanKeFile();

        cout << "\n[ UPDATE ] Harga berhasil diperbarui.";
        cout << "\n========================================\n";
    }

	
	// Fungsi Urutkan Barang (Mengurutkan data barang sementara)
    void urutkanBarang(){
        cout << "\n========================================";
    cout << "\n          URUTKAN DATA BARANG";
    cout << "\n========================================\n";

    if(jumlahBarang < 2){

        cout << "Data barang belum cukup untuk diurutkan.\n";
        cout << "========================================\n";
        return;
    }

    cout << "Urutkan berdasarkan:";
    cout << "\n1. Nama (A-Z)";
    cout << "\n2. Harga (Termurah)";
    cout << "\n3. Stok (Terbanyak)";
    cout << "\nPilih : ";

    int pilih;
    cin >> pilih;

    // Validasi pilihan
    if(pilih < 1 || pilih > 3){

        cout << "\n[GAGAL] Pilihan tidak valid!\n";
        return;
    }

    // Array sementara agar data asli tidak berubah
    Barang temp[MAX_BARANG];

    for(int i = 0; i < jumlahBarang; i++){

        temp[i] = daftarBarang[i];
    }

    // Bubble Sort
    for(int i = 0; i < jumlahBarang - 1; i++){

        for(int j = 0; j < jumlahBarang - 1 - i; j++){

            bool tukar = false;

            if(pilih == 1 &&
               temp[j].nama > temp[j + 1].nama){

                tukar = true;
            }

            else if(pilih == 2 &&
                    temp[j].harga > temp[j + 1].harga){

                tukar = true;
            }

            else if(pilih == 3 &&
                    temp[j].stok < temp[j + 1].stok){

                tukar = true;
            }

            if(tukar){

                Barang swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }

    cout << "\n========================================";
    cout << "\n         HASIL DATA TERURUT";
    cout << "\n========================================\n";

    cout << left
         << setw(8)  << "ID"
         << setw(20) << "Nama"
         << setw(15) << "Harga"
         << setw(10) << "Stok"
         << "Terjual" << endl;

    cout << "----------------------------------------------------------\n";

    for(int i = 0; i < jumlahBarang; i++){

        cout << left
             << setw(8)  << temp[i].id
             << setw(20) << temp[i].nama
             << setw(15) << formatRupiah(temp[i].harga)
             << setw(10) << temp[i].stok
             << temp[i].terjual
             << endl;
    }

    cout << "==========================================================\n";
}


    void barangTerlaris(){
        cout << "\n========================================";
        cout << "\n             BARANG TERLARIS";
        cout << "\n========================================\n";

        if (jumlahBarang == 0){
            cout << "Belum ada data barang.\n";
            cout << "========================================\n";
            return;
        }
        
        // Cek apakah ada barang yang pernah terjual
    	bool adaPenjualan = false;
    	
    	
    	for(int i = 0; i < jumlahBarang; i++){

        	if(daftarBarang[i].terjual > 0){

            	adaPenjualan = true;
            	break;
        	}
    	}
    	
    	 if(!adaPenjualan){

        	cout << "Belum ada transaksi penjualan.\n";
        	cout << "========================================\n";
        	return;
    	}

        // salin data supaya urutan asli daftarBarang tidak berubah
        Barang temp[MAX_BARANG];
        
        for (int i = 0; i < jumlahBarang; i++) {
        	temp[i] = daftarBarang[i];
		}
		
		
		// Bubble sort Berdasarkan jumlah yang terjual
        for (int i = 0; i < jumlahBarang - 1; i++){
            for (int j = 0; j < jumlahBarang - 1 - i; j++){
                if (temp[j].terjual < temp[j + 1].terjual){
                    Barang t   = temp[j];
                    temp[j]    = temp[j + 1];
                    temp[j+1]  = t;
                }
            }
        }

        int batas;
		if(jumlahBarang < 5){
			batas = jumlahBarang;
			
		} else{
			batas = 5;
		}
		
		cout << left
			 << setw(8) << "Rank"
			 << setw(10) << "ID"
			 << setw(25) << "Nama Barang"
			 << "Terjual" << endl;
			 
        cout << "----------------------------------------\n";
        
        for (int i = 0; i < batas; i++){
        	
        	// script barang yang belum pernah terjual
        	if(temp[i].terjual == 0){
        		continue;
			}
			
			cout << left
            	<< setw(8) << (i + 1) 
				<< setw(10) << temp[i].id
				<< setw(25) << temp[i].nama
                << temp[i].terjual << " unit\n";
        }
        cout << "========================================\n";
    }


	
    void tampilStok(){
        cout << "\n========================================";
        cout << "\n            DETAIL STOK";
        cout << "\n========================================\n";

        if (jumlahBarang == 0){
            cout << "Belum ada data barang.\n";
        }else{
            cout << left << setw(8) << "ID" << setw(20) << "Nama"
                 << setw(12) << "Harga" << setw(8) << "Stok" << "Terjual\n";
            cout << "----------------------------------------\n";
            for (int i = 0; i < jumlahBarang; i++){
                Barang &b = daftarBarang[i];
                cout << left << setw(8) << b.id << setw(20) << b.nama
                     << setw(12) << formatRupiah(b.harga) << setw(8) << b.stok
                     << b.terjual << "\n";
            }
        }
        cout << "========================================\n";
    }


	//Fungsi Notifikasi Stok (Menampilkan peringatan stok)
    void tampilNotifikasiStok(int batas = 10){
        cout << "\n========================================";
        cout << "\n           STOK MENIPIS";
        cout << "\n========================================\n";

        bool ada = false;
        for (int i = 0; i < jumlahBarang; i++){
            Barang &b = daftarBarang[i];
            if (b.stok <= batas){
                cout << b.nama << " : " << b.stok << "\n";
                ada = true;
            }
        }
        if (!ada) cout << "Semua stok dalam jumlah aman (>" << batas << ").\n";
        cout << "========================================\n";
    }

	
	// Fungsi Inventaris (Menampilkan data inventaris)
    void tampilInventaris(){
        cout << "\n========================================";
        cout << "\n         LAPORAN INVENTARIS";
        cout << "\n========================================\n";

        int totalJenis       = jumlahBarang;
        long long totalStok  = 0;
        long long totalNilai = 0;
        int barangHabis      = 0;

        for (int i = 0; i < jumlahBarang; i++){
            Barang &b = daftarBarang[i];
            totalStok  += b.stok;
            totalNilai += (long long)b.stok * b.harga;
            if (b.stok == 0) barangHabis++;
        }

        cout << "Total Jenis Barang : " << totalJenis << "\n";
        cout << "Total Stok Barang  : " << totalStok << "\n";
        cout << "Barang Habis       : " << barangHabis << "\n";
        cout << "Total Nilai Stok   : Rp" << formatRupiah(totalNilai) << "\n";
        cout << "========================================\n";
    }


	// Fungsi Daftar Harga (Menampilkan daftar harga barang)
    void tampilDaftarHarga(){
        cout << "\n========================================";
        cout << "\n           DAFTAR HARGA";
        cout << "\n========================================\n";

        if (jumlahBarang == 0){
            cout << "Belum ada data barang.\n";
        }
        else{
            for (int i = 0; i < jumlahBarang; i++){
                Barang &b = daftarBarang[i];
                cout << b.nama << " : Rp" << formatRupiah(b.harga) << "\n";
            }
        }
        cout << "========================================\n";
    }
};


// ======================================================
// CLASS MENU STAFF (Kasir dan Gudang)
// ======================================================

class Staff{

private:

    Kasir kasir;

    Gudang gudang;

public:
	
	void menuKasir(){
    int pilih;
    do{
        cout << "\n========================================";
		cout << "\n              MENU Kasir";
        cout << "\n========================================";
        cout << "\n1. Input Transaksi";
        cout << "\n2. Lihat / Batalkan Item Keranjang";
        cout << "\n3. Input Diskon";
        cout << "\n4. Input Pembayaran (+Cetak Struk)";
        cout << "\n5. Split Bill";
        cout << "\n6. Retur Barang";
        cout << "\n7. Riwayat Transaksi";
        cout << "\n8. Cari Transaksi (No Struk)";
        cout << "\n9. Statistik Penjualan";
        cout << "\n0. Kembali ke Menu Utama";
        cout << "\n=================================";
        cout << "\nPilih menu : ";
        cin >> pilih;

        switch (pilih){
            case 1: 
				kasir.inputTransaksi(); 
				break;
            case 2: 
				kasir.voidItemKeranjang(); 
				break;
            case 3: 
				kasir.inputDiskon(); 
				break;
            case 4: 
				kasir.inputPembayaran(); 
				break;
            case 5: 
				kasir.inputSplitBill(); 
				break;
            case 6: 
				kasir.inputRetur(); 
				break;
            case 7: 
				kasir.riwayatTransaksi(); 
				break;
            case 8: 
				kasir.cariTransaksi(); 
				break;
            case 9: 
				kasir.statistikPenjualan(); 
				break;
            case 0: 
				cout << "\nKembali ke menu utama...\n"; 
				break;
            default: 
			cout << "\nPilihan tidak tersedia!\n";
        }
    } while (pilih != 0);
}
	

    void menuGudang(){
    int pilih;
    do{
        cout << "\n========================================";
		cout << "\n              MENU Gudang";
        cout << "\n========================================";
        cout << "\n1. Input Barang Baru";
        cout << "\n2. Hapus Barang";
        cout << "\n3. Cari Barang";
        cout << "\n4. Input Restock";
        
        cout << "\n5. Input Supplier";
        cout << "\n6. Tampil Supplier\n";
        
        cout << "\n7. Penyesuaian Harga";
        cout << "\n8. Urutkan Data Barang";
        
        cout << "\n9. Barang Terlaris";
        cout << "\n10. Tampil Stok";
        cout << "\n11. Notifikasi Stok Menipis";
        
        cout << "\n12. Laporan Inventaris";
        cout << "\n13. Daftar Harga";
        
        cout << "\n0. Kembali ke Menu Utama";
        cout << "\n==================================";
        cout << "\nPilih menu : ";
        cin >> pilih;

        switch (pilih){
            case 1:  
				gudang.inputBarangBaru(); 
				break;
            case 2:  
				gudang.hapusBarang(); 
				break;
            case 3: 
				gudang.cariBarang(); 
				break;
            case 4:  	
				gudang.inputRestock(); 
				break;
            case 5:  
				gudang.inputSupplier(); 
				break;
			case 6;
				gudang.tampilSupplier();
				break;
            case 7:  
				gudang.inputPenyesuaianHarga(); 
				break;
            case 8:  
				gudang.urutkanBarang(); 
				break;
            case 9:  
				gudang.barangTerlaris(); 
				break;
			case 10;
				gudang.tampilStok();
            case 11: 
				gudang.tampilNotifikasiStok(); 
				break;
            case 12: 
				gudang.tampilInventaris(); 
				break;
            case 13: 
				gudang.tampilDaftarHarga();
				break;
            case 0:  
				cout << "\nKembali ke menu utama...\n"; 
				break;
            default: 
				cout << "\nPilihan tidak tersedia!\n";
        }
	}while (pilih != 0);
}


    void menuUtama(){

        int pilih;

        do{

            cout << "\n========================================";
            cout << "\n      APLIKASI SUPERMARKET";
            cout << "\n========================================";
            cout << "\n1. Menu Kasir";
            cout << "\n2. Menu Gudang";
            cout << "\n0. Keluar";
            cout << "\n========================================";
            cout << "\nPilih menu : ";

            cin >> pilih;

            switch(pilih){

                case 1:
                    menuKasir();
                    break;

                case 2:
                    menuGudang();
                    break;

                case 0:
                    cout << "\nProgram selesai.\n";
                    break;

                default:
                    cout << "\nMenu tidak tersedia!\n";
            }

        }while(pilih != 0);
    }
};


// ======================================================
// CLASS CUSTOMER : Johan Hanggara Hasiholan
// ======================================================

class Customer {
private:
    DataUtama databaseUser[100];
    ItemTransaksi keranjang[50];
	int jumlahKeranjang = 0;

public:
	// Fungsi Registrasi Customer (Menyimpan data customer)
	// Banyak baris kode : 33 baris
    void registrasi() {
        DataUtama data;

        cout << "\n=== REGISTRASI CUSTOMER ===";
        cout << "\nNama     : ";
        getline(cin, data.nama);   // gunakan getline
        cout << "\nEmail    : ";
        getline(cin, data.email);
        cout << "\nNo HP    : ";
        getline(cin, data.noHp);
        cout << "\nUsername : ";
        getline(cin, data.username);

        do {
            cout << "\nPassword : ";
            getline(cin, data.password);
            cout << "\nUlangi Password: ";
            getline(cin, data.ulang);

            if (data.password != data.ulang)
                cout << "Maaf, password tidak cocok!\n";
        } while (data.password != data.ulang);

        ofstream outFile("dataCustomer.txt", ios::app);
        if (outFile.is_open()) {
            outFile << data.nama << ";" << data.email << ";" << data.noHp << ";" << data.username << ";" << data.password << endl;
            outFile.close();
            cout << "Data berhasil disimpan!" << endl;
        } else {
            cout << "Maaf, Data tidak tersimpan, silahkan coba lagi!." << endl;
        }
    }

	// Fungsi Read Customer (Membaca data customer dari file)
	// Banyak baris kode : 16 baris
    void readCustomers() {
        ifstream inFile("dataCustomer.txt");
        string line;

        if (inFile.is_open()) {
            cout << "\nData Customer:" << endl;
            while (getline(inFile, line)) {
                cout << line << endl;
            }
            inFile.close();
        } else {
            cout << "Tidak bisa membuka file untuk membaca." << endl;
        }
    }
    
    // Fungsi Login Customer (Memverifikasi login customer)
	// Banyak baris kode : 49 baris
    void Login(){
    	string usernameInput, passwordInput;
    	cout << "\n========================================";
        cout << "\n          LOGIN CUSTOMER";
        cout << "\n========================================";

        cout << "\nUsername : ";
        getline(cin, usernameInput);
        cout << "\nPassword : ";
        getline(cin, passwordInput);
        cout << "\n========================================\n";
        
        ifstream inFile("dataCustomer.txt");
        string line;
        bool found = false;
        
        if(inFile.is_open()){
        	while (getline(inFile, line)){
        		stringstream ss(line);
    			DataUtama data;
        	
        		getline(ss,data.nama, ';');
        		getline(ss,data.email, ';');
        		getline(ss,data.noHp, ';');
        		getline(ss,data.username, ';');
        		getline(ss,data.password, ';');
        	
        	if(data.username == usernameInput &&
   				data.password == passwordInput){

    			cout << "\nLogin berhasil! Selamat datang, "
         		<< data.nama << endl;

    			found = true;

    			// MASUK KE MENU CUSTOMER
    			menuCustomer();
    			break;
			}
		}
			inFile.close();
			
			if(!found){
				cout << "\nLogin gagal! Username atau password salah." << endl;
			} 
		}
	}
	
	// Fungsi Katalog Produk (Menampilkan daftar produk)
	// Banyak baris kode : 49 baris
	void KatalogProduk(){
		
		cout << "\n========================================";
    	cout << "\n          KATALOG PRODUK";
    	cout << "\n========================================\n";

    	cout << left << setw(5) << "ID" 
     		 << setw(20) << "Nama Barang"
			  << setw(15) << "Jenis"  
     		 << setw(20) << "Harga" 
     		 << setw(10) << "Stok" 
     		 << endl;
    	cout <<"---------------------------------------------------------\n";
    	
    	ifstream inFile("katalogProduk.txt");
    	
    	if (!inFile.is_open()){
    		cout << "File gagal dibuka!" << endl;
    		return;
		}
		
		string line;
		
		while (getline(inFile, line)) {
			stringstream ss(line);
			
			Barang item;
			
			string hargaStr;
			string stokStr;
			
			// Parsing sesuai format id, nama, jenis, harga, stok
			getline(ss, item.id, ';');
			getline(ss, item.nama, ';');
			getline(ss, item.jenis, ';');
			
			getline(ss, hargaStr, ';');
			stringstream(hargaStr) >> item.harga;
			
			getline(ss, stokStr, ';');
			stringstream(stokStr) >> item.stok;
			
			 cout << left 
			 << setw(5)  << item.id 
         	 << setw(20) << item.nama
         	 << setw(15) << item.jenis 
         	 << setw(10) << item.harga
        	 << setw(10) << item.stok 
         	 
			 
			 << endl;
		
		}
		
	inFile.close();
	
	}	
	
	// Fungsi Pencarian Produk (Mencari produk berdasarkan ID)
	// Banyak baris kode : 55 baris
	void pencarianProduk(){
		string keyword;
		
		cout << "\n========================================";
    	cout << "\n           CARI PRODUK";
    	cout << "\n========================================";
    	
        cout << "\nNama Produk / ID : ";
        getline(cin, keyword);
        
        cout << "\n========================================";
        
        ifstream inFile("katalogProduk.txt");
        
		string line;
		bool found = false;
		
		if(!inFile.is_open()){
			cout << "File gagal dibuka!" << endl;
			return;
		}
		
		while(getline(inFile, line)){
			
			stringstream ss(line);
			
			Barang item;
			
			string hargaStr;
			string stokStr;
			
		
			// Parsing sesuai format id, nama, jenis, harga, stok
			getline(ss, item.id, ';');
			getline(ss, item.nama, ';');
			getline(ss, item.jenis, ';');
			
			getline(ss, hargaStr, ';');
			stringstream(hargaStr) >> item.harga;
			
			getline(ss, stokStr, ';');
			stringstream(stokStr) >> item.stok;
			
			// Pencarian Berdasarkan ID atau Nama
			
			if(item.id == keyword || item.nama == keyword ){
				
				cout << "\nProduk ditemukan:\n";
				
				cout << left 
					 << setw(5) << item.id
					 << setw(20) << item.nama
					 << setw(15) << item.jenis
					 << setw(20) << "Rp" << item.harga
					 << setw(10) << item.stok
					 << endl;
				found = true;
			}
		}
		
		if(!found){
			cout << "\nProduk tidak ditemukan!" << endl;
		}
        
        
	}
	
	//==================================
	// Fungsi Tambah Keranjang
	//==================================
	
	void tambahKeranjang(){

    string id;
    int qty;

    cout << "\n========================================";
    cout << "\n         TAMBAH KERANJANG";
    cout << "\n========================================";

    cout << "\nID Barang : ";
    cin >> id;

    bool ditemukan = false;
    
    if(jumlahKeranjang >= 50){
    	
    	cout << "\nKeranjang Penuh!\n";
    	return;
	}

    for(int i = 0; i < jumlahBarang; i++){

        if(daftarBarang[i].id == id){

            ditemukan = true;

            cout << "Nama Barang : "
                 << daftarBarang[i].nama;
                 
            cout << "\nHarga : Rp. " 
            	 << daftarBarang[i].harga;

            cout << "\nStok Tersedia : "
                 << daftarBarang[i].stok;

            cout << "\nJumlah Beli : ";
            cin >> qty;

            if(qty <= 0 || qty > daftarBarang[i].stok){

                cout << "\nStok tidak cukup!\n";
                return;
            }

            ItemTransaksi item;

            item.nama = daftarBarang[i].nama;
            item.harga = daftarBarang[i].harga;
            item.qty = qty;
            item.subtotal = item.harga * qty;

            keranjang[jumlahKeranjang] = item;
            jumlahKeranjang++;

            cout << "\nBarang berhasil ditambahkan!\n";

            return;
        }
    }

    if(!ditemukan){
        cout << "\nBarang tidak ditemukan!\n";
    }
}


	// =========================
	// Fungsi Lihat Keranjang
	// =========================
	void lihatKeranjang(){

    cout << "\n========================================";
    cout << "\n         KERANJANG BELANJA";
    cout << "\n========================================";

    if(jumlahKeranjang == 0){

        cout << "\nKeranjang kosong.\n";
        return;
    }

    long long total = 0;

    cout << left	// HEADER TABEL
         << setw(5)  << "No"
         << setw(20) << "Nama Barang"
         << setw(10) << "Qty"
         << setw(15) << "Harga"
         << setw(15) << "Subtotal"
         << endl;

    cout << "-------------------------------------------------------------\n";

    
    // DATA KERANJANG

    for(int i = 0; i < jumlahKeranjang; i++){

        cout << left
             << setw(5)  << i + 1
             << setw(20) << keranjang[i].nama
             << setw(10) << keranjang[i].qty
             << setw(15) << formatRupiah(keranjang[i].harga)
             << setw(15) << formatRupiah(keranjang[i].subtotal)
             << endl;

        total += keranjang[i].subtotal;
    }

    cout << "-------------------------------------------------------------";

    cout << "\nTotal : Rp"
         << formatRupiah(total);

    cout << "\n========================================\n";
}
	
	
	
	// ===========================
	// Fungsi Chekout Produk
	//============================
	
void checkoutCustomer(){

    if(jumlahKeranjang == 0){

        cout << "\nKeranjang kosong!\n";
        return;
    }

    long long total = 0;

    for(int i = 0; i < jumlahKeranjang; i++){

        total += keranjang[i].subtotal;
    }

    cout << "\n========================================";
    cout << "\n            CHECKOUT";
    cout << "\n========================================";

    cout << "\nTotal Belanja : Rp"
         << formatRupiah(total);

    for(int i = 0; i < jumlahKeranjang; i++){	// Kurangi Stok Produk Gudang
    	for(int j=0; j < jumlahBarang; j++){
    		if(keranjang[i].nama == daftarBarang[j].nama){
    			
    			daftarBarang[j].stok -= keranjang[i].qty;
    			daftarBarang[j].terjual += keranjang[i].qty;
    			
			}
		}
	}
	
	cout << "\nCheckout Berhasil!\n";
	
	jumlahKeranjang = 0;
	
    cout << "========================================\n";
}


	// Riwayat Transaksi
	void riwayatTransaksi(){
		
		ifstream inFile("riwayatCustomer.txt");
		
		string line;
		
		cout << "\n========================================";
        cout << "\n             Riwayat Transaksi";
        cout << "\n========================================";
        
        if(!inFile.is_open()){
        	
        	cout << "Belum ada riwayat.\n";
        	return;
		}
		
		while(getline(inFile, line)){
			
			cout << line << endl;
		}
		
		inFile.close();
		
	}
	
	 // Fungsi Menu Customer (Menampilkan menu customer)
	// Banyak baris kode : 56 baris
    void menuCustomer() {

        int pilih;

        do {

            cout << "\n========================================";
            cout << "\n             MENU CUSTOMER";
            cout << "\n========================================";

            cout << "\n1. Lihat Katalog";
            cout << "\n2. Cari Produk";
            cout << "\n3. Tambah Belanja";
            cout << "\n4. Lihat Keranjang";
            cout << "\n5. Checkout";
            cout << "\n6. Riwayat Pembelian";
            cout << "\n7. Retur Barang";
            cout << "\n0. Logout";

            cout << "\n========================================";
            cout << "\nPilih Menu : ";
            cin >> pilih;
            cin.ignore();

            switch (pilih) {

                case 1:
    				KatalogProduk();
   					break;

				case 2:
    				pencarianProduk();
    				break;

                case 3:
                    tambahKeranjang();
                    break;
                 
                case 4:
                	lihatKeranjang();

                case 5:
                    checkoutCustomer();
                    break;

                case 6:
                    riwayatTransaksi();
                    break;
                    
                case 7:
                    riwayatTransaksi();
                    break;

                case 0:
                    cout << "\nLogout berhasil!\n";
                    break;

                default:
                    cout << "\nMenu tidak tersedia!\n";
            }

        } while (pilih != 0);
    }
    

   // Fungsi Menu Utama Customer (Navigasi login dan registrasi)
  // Banyak baris kode : 36 baris
  void menuUtama(){  
  int pilih;

    do{

        cout << "\n===== MENU UTAMA =====";
        cout << "\n1. Registrasi";
        cout << "\n2. Login";
        cout << "\n3. Keluar";

        cout << "\nPilih : ";
        cin >> pilih;
        cin.ignore();

        switch(pilih){

            case 1:
                registrasi();
                break;

            case 2:
               Login();
                break;

            case 3:
                cout << "Program selesai." << endl;
                break;

            default:
                cout << "Menu tidak tersedia!" << endl;
        }

    } while(pilih != 3);
}
};


class MenuUtama {
public:
	void menuSistem(
		MenuAdmin &adminSistem, 
		Customer &customer,
		Staff &staff
	){

    int pilih;

    do{

        cout << "\n========================================";
        cout << "\n         SISTEM APLIKASI SUPERMARKET";
        cout << "\n========================================";

        cout << "\n1. Menu Admin";
        cout << "\n2. Menu Staff";
        cout << "\n3. Menu Customer";
        cout << "\n4. Keluar";

        cout << "\n========================================";
        cout << "\nPilih Menu : ";
        cin >> pilih;
        cin.ignore();

        switch(pilih){

            case 1:
                adminSistem.tampilkanMenu();
                break;

            case 2:
                staff.menuUtama();
                break;

            case 3:
            	customer.menuUtama();
                break;

			case 4:
				cout << "\nProgram Selesai.\n";
				break;
            default:
                cout << "\nMenu tidak tersedia!\n";
        }

    } while(pilih != 4);
}
};


int main() {

    MenuAdmin adminSistem;

    Customer customer;

    Staff staff;
    
    MenuUtama menu;

    menu.menuSistem(
        adminSistem,
        customer,
        staff
    );

    return 0;
}
