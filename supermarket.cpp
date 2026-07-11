//=====================================================
// 1. LIBRARY
//=====================================================
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

//=====================================================
// 3. KONSTANTA SISTEM
//=====================================================
const int MAX_BARANG    = 100;

const int MAX_SUPPLIER  = 100;

const int MAX_TRANSAKSI = 100;

const int MAX_RETUR     = 100;

//=====================================================
// 4. NAMA FILE DATABASE
//=====================================================
const string FILE_BARANG    = "barang.txt";
const string FILE_SUPPLIER  = "supplier.txt";
const string FILE_TRANSAKSI = "transaksi.txt";
const string FILE_RETUR     = "dataRetur.txt";
const string FILE_DETAIL_TRANSAKSI = "detaiTransaksi.txt";


//=====================================================
// 5. FUNGSI UTILITAS GLOBAL
//=====================================================

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


//=====================================================
// 6. STRUCT DATABASE
//=====================================================
// Data barang di gudang / inventaris
struct Barang{
    string id;
    string nama;
    long long harga;
    int stok;
    string jenis;
    int terjual; // total unit yang sudah pernah terjual
};

//Data transaksi

// item keranjang belaja sementara (Customer)
struct ItemTransaksi{
    string nama;
    long long harga;
    int qty;
    long long subtotal;
};

// data vendor / supplier pasokan
struct Supplier{
    string nama;
    string alamat;
    string telepon;
};


// Data riwayat transaksi induk (struk pembayaran)
struct RiwayatTransaksi{
    string noStruk;
    string idCustomer;
    
    long long subtotal;
    long long diskon;
    
    double ongkosKirim;

    string metode;
    int jumlahItem;
    
    // Tambahan
    string idTransaksi;
    string tanggal;
    double totalBelanja;
};

// Data retur relasional
struct DataRetur{
    string noStruk;	// foreign key (riwayatTransaksi)
	string idCustomer; // foreign Key ke dataUtama.iduser
	
	int jumlah; // kuantitas unit
    string namaBarang;
    string alasan;
    string status;
    string idRetur;	// primary key (ex: RTR00q)
    string idTransaksi;
    string skuBarang;	// foreign key ke barang.id
    string catatan_admin; // alasan keputusan dari admin/kasir
    string tanggal;
};

// Data profil pengguna (Admin, staff, Customer)
struct DataUtama {
	string idUser;
    string nama;
    string email;
    string noHp;      
    string username;
    string password;
    string ulang;
    string role; // ADMIN, KASIR, GUDANG, CUSTOMER
};


// Data operasional internal karyawan
struct Karyawan {
    string id;
    string nama;
    string role; // kasir / gudang
    string shift; // pagi, siang, malam
};

// sistem promo & voucher Toko
struct Promo {
    string kodePromo;
    double diskonPersen;
    string status;
};

// sistem delivery antar rumah
struct Delivery {
	string idTransaksi;
    string idResi; // RESI001, dst
    string namaPenerima;
    string kotaTujuan;
    double ongkosKirim;
    string statusKirim;
};

// Log logistik sistem
struct LogAktivitas {
	string idUser;	// siapa pelaku aktivitasnya
    string waktu;
    string deskripsi;
};


//=====================================================
// 7. DATABASE GLOBAL (ARRAY)
//=====================================================
Barang           daftarBarang[MAX_BARANG];

Supplier         daftarSupplier[MAX_SUPPLIER];

RiwayatTransaksi daftarTransaksi[MAX_TRANSAKSI];

DataRetur        daftarRetur[MAX_RETUR];

//=====================================================
// JUMLAH DATA (DATABASE COUNTER)
//=====================================================
int jumlahBarang    = 0;
int jumlahSupplier  = 0;
int jumlahTransaksi = 0;
int jumlahRetur     = 0;

//=====================================================
// 8. VARIABEL GLOBAL SISTEM
//=====================================================

//=====================================================
// 8. CLASS ENTITY
//=====================================================



//=====================================================
// FUNGSI VALIDASI (Customer Helper)
//=====================================================
class ValidasiDanGenerate{
	public:
		// validasi input kosong
	static bool validasiKosong(string data){
	    return !data.empty();
	}
	
	// validasi panjang username
	static bool validasiUsername(string username){
	    return username.length() >= 5;
	}
	
	// validasi password
	static bool validasiPassword(string password){
	    return password.length() >= 8;
	}
	
	// konfirmasi password
	static bool konfirmasiPassword(string password,
	                        string ulangPassword){
	    return password == ulangPassword;
	}
	
	// validasi noHP
	static bool validasiNoHP(string noHP){
	
	    if(noHP.length() < 12)
	        return false;
	
	    for(int i = 0; i < noHP.length(); i++){
	
	        if(!isdigit(noHP[i]))
	            return false;
	    }
	
	    return true;
	}
	
	// validasi email
	static bool emailSudahAda(string email)
	{
	
	    ifstream inFile("dataCustomer.txt");
	
	    if(!inFile.is_open())
	        return false;
	
	    string line;
	
	    while(getline(inFile, line)){
	
	        stringstream ss(line);
	
	        DataUtama data;
	
				getline(ss, data.idUser, 	';');
		       	getline(ss, data.nama, 		';');
				getline(ss, data.username, 	';');
				getline(ss, data.email, 	';');
				getline(ss, data.noHp, 		';');
				getline(ss, data.password, 	';');
				
	        if(data.email == email){
	
	            inFile.close();
	            return true;
	        }
	    }
	
	    inFile.close();
	    return false;
	}
	
	// validasi username yang sudah ada
	static bool usernameSudahAda(string username){
    if(username.empty())
        return false;
    
    ifstream inFile("dataCustomer.txt");
    
    // 1. Jika file gagal dibuka, langsung keluar fungsi di sini
    if(!inFile.is_open()){
        return false;
    } // <--- KURUNG KURAWAL SUDAH DIPINDAHKAN KE SINI!
    
    string line;
    
    // 2. Sekarang loop pembacaan berjalan ketika file BERHASIL dibuka
    while(getline(inFile, line)){
        if(line.empty() || line == "\n" || line == "\r") 
            continue;
        
        stringstream ss(line);
        DataUtama data;
        
        // Membaca file menggunakan format pembatas ';' sesuai keinginan Anda
        getline(ss, data.idUser,   ';');
        getline(ss, data.nama,     ';');
        getline(ss, data.username, ';');
        getline(ss, data.email,    ';');
        getline(ss, data.noHp,     ';');
        getline(ss, data.password, ';');
        getline(ss, data.ulang,    ';');
        
        // Lakukan pencocokan username
        if(data.username == username){
            inFile.close();
            return true; // Username ditemukan (sudah terpakai)
        }
    }
    
    inFile.close();
    return false; // Username aman digunakan (belum ada)
}

	
	// generate id customer otomatis
	static string generateIDCustomer(){
		 ifstream inFile("dataCustomer.txt");
	
	    if(!inFile.is_open())
	        return "CUS001";
	
	    string line;
	
	    int terbesar = 0;
	
	    while(getline(inFile, line)){
	
	        stringstream ss(line);
	
	        DataUtama data;
	
	        getline(ss, data.idUser, ';');
	
	        if(data.idUser.substr(0,3) == "CUS"){
	
	            int nomor = atoi(data.idUser.substr(3).c_str());
	
	            if(nomor > terbesar)
	                terbesar = nomor;
	        }
	    }
	
	    inFile.close();
	
	    terbesar++;
	
	    char buffer[20];
	
	    sprintf(buffer,"CUS%03d",terbesar);
	
	    return string(buffer);
	}
	
	// generate tanggal otomatis
	static string generateTanggal()
	{
		time_t sekarang = time(0);
	
	    tm *waktu = localtime(&sekarang);
	
	    char buffer[20];
	
	    sprintf(buffer,"%02d-%02d-%04d",
	            waktu->tm_mday,
	            waktu->tm_mon + 1,
	            waktu->tm_year + 1900);
	
	    return string(buffer);
	}
	
	// --- ?? TAMBAHKAN FUNGSI INI DI DALAM KELAS YANG SAMA ?? ---
    // Fungsi umum untuk membuat ID otomatis (bisa untuk CUS, BRG, TRX, maupun RTR)
    static string generateIDOtomatis(string prefix, string namaFile) {
    // Membuka file berdasarkan variabel namaFile yang dikirim
    ifstream inFile(namaFile.c_str()); 
    if (!inFile.is_open()) 
        return prefix + "001";

    string line;
    int terbesar = 0;
    
    // PERBAIKAN: Ganti dari 'file' menjadi 'inFile' agar sinkron dengan baris atas
    while (getline(inFile, line)) { 
        if (line.empty()) continue;
        stringstream ss(line);
        string idTerbaca;
        getline(ss, idTerbaca, ';'); // Ambil kolom ID pertama

        if (idTerbaca.length() >= 4 && idTerbaca.substr(0, 3) == prefix) {
            int nomor = atoi(idTerbaca.substr(3).c_str());
            if (nomor > terbesar) terbesar = nomor;
        }
    }
    inFile.close();
    terbesar++;
    char buffer[20];
    sprintf(buffer, "%s%03d", prefix.c_str(), terbesar);
    return string(buffer);
}
};





//=====================================================
// 8. CLASS HAK AKSES
//=====================================================

//=====================================================
// MENU UTAMA
//=====================================================
// ======================================================
// CLASS ADMIN : Ahmad Faeruz Salim
// ======================================================

class Admin {
private:
    // Kredensial Sistem
    string usernameAdmin;
    string pinAdmin;
    
    // Penyimpanan Array Statis
    Karyawan daftarKaryawan[100];
    int jumlahKaryawan;
    
    Promo daftarPromo[50];
    int jumlahPromo;

    Delivery daftarDelivery[100];
    int jumlahDelivery;
    
    RiwayatTransaksi daftarTransaksi[200];
    int jumlahTransaksi;

    DataRetur daftarRetur[100];
    int jumlahRetur;

    LogAktivitas daftarLog[200];
    int jumlahLog;

    // ==========================================
    // UTILITAS & VALIDASI
    // ==========================================

    void bersihkanInput() {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    void pauseLayar() {
        cout << "\nTekan Enter untuk kembali...";
        cin.get();
    }

    bool konfirmasiAksi(string pesan) {
        char pilihan;
        while (true) {
            cout << pesan << " (Y/N): ";
            cin >> pilihan;
            bersihkanInput();
            if (pilihan == 'Y' || pilihan == 'y') return true;
            if (pilihan == 'N' || pilihan == 'n') return false;
            cout << "[!] Input tidak valid. Masukkan Y atau N.\n";
        }
    }

    int inputIntValid(string prompt) {
        int nilai;
        while (true) {
            cout << prompt;
            if (cin >> nilai) {
                bersihkanInput();
                return nilai;
            }
            cout << "[!] Error: Input harus berupa angka bulat!\n";
            bersihkanInput();
        }
    }

    double inputDoubleValid(string prompt) {
        double nilai;
        while (true) {
            cout << prompt;
            if (cin >> nilai) {
                bersihkanInput();
                return nilai;
            }
            cout << "[!] Error: Input harus berupa angka desimal/bulat!\n";
            bersihkanInput();
        }
    }

    string dapatkanWaktuSekarang() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[80];
        // Format: YYYY-MM-DD HH:MM:SS
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", ltm);
        return string(buffer);
    }

    void tambahLog(string aktivitas) {
        if (jumlahLog >= 200) {
            // Geser log jika penuh (FIFO)
            for (int i = 0; i < 199; i++) {
                daftarLog[i] = daftarLog[i + 1];
            }
            jumlahLog = 199;
        }
        daftarLog[jumlahLog].waktu = dapatkanWaktuSekarang();
        daftarLog[jumlahLog].deskripsi = aktivitas;
        jumlahLog++;
        saveDataLog();
    }

    // ==========================================
    // SISTEM KEAMANAN
    // ==========================================

    void validasiKeamanan() {
        string inputUser, inputPin;
        bool aksesDiberikan = false;
        int percobaan = 0;
        
        while (!aksesDiberikan && percobaan < 3) {
            cout << "\n======================================" << endl;
            cout << "          LOGIN ADMIN SISTEM          " << endl;
            cout << "======================================" << endl;
            cout << "Username : ";
            cin >> inputUser;
            cout << "PIN      : ";
            cin >> inputPin;
            bersihkanInput();
            
            if (inputUser == usernameAdmin && inputPin == pinAdmin) {
                aksesDiberikan = true;
                cout << "\n[+] Login Berhasil! Selamat datang, " << usernameAdmin << "." << endl;
                tambahLog("Login ke dalam sistem.");
            } else {
                percobaan++;
                cout << "[-] Akses Ditolak! Kredensial salah. Sisa percobaan: " << (3 - percobaan) << endl;
            }
        }

        if (!aksesDiberikan) {
            cout << "\n[!] Anda telah gagal login 3 kali. Program dihentikan demi keamanan." << endl;
            exit(0);
        }
    }

    void ubahPasswordAdmin() {
        cout << "\n--- UBAH PASSWORD ADMIN ---" << endl;
        string pinLama, pinBaru1, pinBaru2;
        
        cout << "Masukkan PIN Lama: ";
        cin >> pinLama;
        
        if (pinLama == pinAdmin) {
            cout << "Masukkan PIN Baru: ";
            cin >> pinBaru1;
            cout << "Konfirmasi PIN Baru: ";
            cin >> pinBaru2;
            
            if (pinBaru1 == pinBaru2) {
                pinAdmin = pinBaru1;
                saveKonfigurasiSistem();
                tambahLog("Mengubah PIN Admin sistem.");
                cout << "[+] PIN berhasil diubah!" << endl;
            } else {
                cout << "[-] Konfirmasi PIN tidak cocok. Gagal mengubah PIN." << endl;
            }
        } else {
            cout << "[-] PIN Lama salah!" << endl;
        }
        bersihkanInput();
    }

    // ==========================================
    // FILE HANDLING (LOAD & SAVE)
    // ==========================================

    void loadKonfigurasiSistem() {
        ifstream file("config.txt");
        if (file.is_open()) {
            file >> usernameAdmin >> pinAdmin;
            file.close();
        } else {
            usernameAdmin = "salim";
            pinAdmin = "1207";
        }
    }

    void saveKonfigurasiSistem() {
        ofstream file("config.txt", ios::trunc);
        if (file.is_open()) {
            file << usernameAdmin << "\n" << pinAdmin << "\n";
            file.close();
        }
    }

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

    void saveDataKaryawan() {
        ofstream file("dataKaryawan.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahKaryawan; i++) {
                file << daftarKaryawan[i].id << " " 
                     << daftarKaryawan[i].nama << " " 
                     << daftarKaryawan[i].role << " " 
                     << daftarKaryawan[i].shift << "\n";
            }
            file.close();
        }
    }

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

    void saveDataPromo() {
        ofstream file("data_promo.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahPromo; i++) {
                file << daftarPromo[i].kodePromo << " " 
                     << daftarPromo[i].diskonPersen << " " 
                     << daftarPromo[i].status << "\n";
            }
            file.close();
        }
    }

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

    void saveDataDelivery() {
        ofstream file("data_delivery.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahDelivery; i++) {
                file << daftarDelivery[i].idResi << " " 
                     << daftarDelivery[i].namaPenerima << " " 
                     << daftarDelivery[i].kotaTujuan << " " 
                     << daftarDelivery[i].ongkosKirim << "\n";
            }
            file.close();
        }
    }

    void loadDataTransaksi() {
        ifstream file("transaksi.txt");
        jumlahTransaksi = 0;
        if (file.is_open()) {
            string id, tgl;
            double total;
            while (file >> id >> tgl >> total) {
                if (jumlahTransaksi < 200) {
                    daftarTransaksi[jumlahTransaksi].idTransaksi = id;
                    daftarTransaksi[jumlahTransaksi].tanggal = tgl;
                    daftarTransaksi[jumlahTransaksi].totalBelanja = total;
                    jumlahTransaksi++;
                }
            }
            file.close();
        }
    }

    void saveDataTransaksi() {
        ofstream file("transaksi.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahTransaksi; i++) {
                file << daftarTransaksi[i].idTransaksi << " " 
                     << daftarTransaksi[i].tanggal << " " 
                     << daftarTransaksi[i].totalBelanja << "\n";
            }
            file.close();
        }
    }

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

    void saveDataRetur() {
        ofstream file("retur.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahRetur; i++) {
                file << daftarRetur[i].idRetur << " " 
                     << daftarRetur[i].idTransaksi << " " 
                     << daftarRetur[i].skuBarang << " " 
                     << daftarRetur[i].alasan << "\n";
            }
            file.close();
        }
    }

    void loadDataLog() {
        ifstream file("log_aktivitas.txt");
        jumlahLog = 0;
        if (file.is_open()) {
            string tgl, jam, desk;
            while (file >> tgl >> jam) {
                getline(file, desk);
                if (jumlahLog < 200) {
                    daftarLog[jumlahLog].waktu = tgl + " " + jam;
                    // Menghapus spasi di awal deskripsi yang terbawa oleh getline
                    if(!desk.empty() && desk[0] == ' ') {
                        desk = desk.substr(1);
                    }
                    daftarLog[jumlahLog].deskripsi = desk;
                    jumlahLog++;
                }
            }
            file.close();
        }
    }

    void saveDataLog() {
        ofstream file("log_aktivitas.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < jumlahLog; i++) {
                file << daftarLog[i].waktu << " " 
                     << daftarLog[i].deskripsi << "\n";
            }
            file.close();
        }
    }

    // ==========================================
    // MENU 1: MANAJEMEN KARYAWAN
    // ==========================================

    bool cekDuplikasiIdKaryawan(string id) {
        for (int i = 0; i < jumlahKaryawan; i++) {
            if (daftarKaryawan[i].id == id) return true;
        }
        return false;
    }

    void tampilkanDaftarKaryawan() {
        if (jumlahKaryawan == 0) {
            cout << "[-] Data karyawan kosong." << endl;
            return;
        }
        cout << "\n====================== DAFTAR KARYAWAN ======================\n";
        cout << left << setw(5) << "No" << setw(15) << "ID" << setw(20) << "Nama" << setw(15) << "Role" << setw(15) << "Shift" << endl;
        cout << "-------------------------------------------------------------\n";
        for (int i = 0; i < jumlahKaryawan; i++) {
            cout << left << setw(5) << i + 1 
                 << setw(15) << daftarKaryawan[i].id 
                 << setw(20) << daftarKaryawan[i].nama 
                 << setw(15) << daftarKaryawan[i].role 
                 << setw(15) << daftarKaryawan[i].shift << endl;
        }
        cout << "-------------------------------------------------------------\n";
    }

    void registrasiKaryawan() {
        cout << "\n=== [TAMBAH KARYAWAN] ===" << endl;
        if (jumlahKaryawan >= 100) {
            cout << "[-] Kapasitas data karyawan penuh!" << endl;
            return;
        }

        string idBaru;
        cout << "ID Karyawan      : ";
        cin >> idBaru;
        
        if (cekDuplikasiIdKaryawan(idBaru)) {
            cout << "[-] ID Karyawan sudah terdaftar! Gunakan ID lain.\n";
            bersihkanInput();
            return;
        }

        daftarKaryawan[jumlahKaryawan].id = idBaru;
        cout << "Nama (1 Kata)    : ";
        cin >> daftarKaryawan[jumlahKaryawan].nama;
        cout << "Posisi/Role      : ";
        cin >> daftarKaryawan[jumlahKaryawan].role;
        daftarKaryawan[jumlahKaryawan].shift = "Belum_Diatur";
        
        jumlahKaryawan++;
        saveDataKaryawan();
        tambahLog("Menambah data karyawan baru dgn ID: " + idBaru);
        
        cout << "[+] Data berhasil disimpan ke sistem!" << endl;
        bersihkanInput();
    }

    void cariKaryawan() {
        cout << "\n=== [CARI KARYAWAN] ===" << endl;
        if (jumlahKaryawan == 0) { cout << "Data kosong.\n"; return; }
        
        string keyword;
        cout << "Masukkan ID atau Nama Karyawan: ";
        cin >> keyword;
        bersihkanInput();

        bool ditemukan = false;
        cout << "\nHasil Pencarian:\n";
        cout << "-------------------------------------------------------------\n";
        for (int i = 0; i < jumlahKaryawan; i++) {
            if (daftarKaryawan[i].id == keyword || daftarKaryawan[i].nama == keyword) {
                cout << "ID      : " << daftarKaryawan[i].id << "\n"
                     << "Nama    : " << daftarKaryawan[i].nama << "\n"
                     << "Role    : " << daftarKaryawan[i].role << "\n"
                     << "Shift   : " << daftarKaryawan[i].shift << "\n";
                cout << "-------------------------------------------------------------\n";
                ditemukan = true;
            }
        }
        if (!ditemukan) cout << "[-] Karyawan dengan kata kunci '" << keyword << "' tidak ditemukan.\n";
        tambahLog("Melakukan pencarian karyawan: " + keyword);
    }

    void editKaryawan() {
        cout << "\n=== [EDIT KARYAWAN] ===" << endl;
        tampilkanDaftarKaryawan();
        if (jumlahKaryawan == 0) return;

        string idCari;
        cout << "\nMasukkan ID Karyawan yang akan diedit: ";
        cin >> idCari;
        bersihkanInput();

        int index = -1;
        for (int i = 0; i < jumlahKaryawan; i++) {
            if (daftarKaryawan[i].id == idCari) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            cout << "Data Saat Ini -> Nama: " << daftarKaryawan[index].nama << ", Role: " << daftarKaryawan[index].role << "\n";
            if (konfirmasiAksi("Apakah Anda yakin ingin mengubah data ini?")) {
                cout << "Masukkan Nama Baru : ";
                cin >> daftarKaryawan[index].nama;
                cout << "Masukkan Role Baru : ";
                cin >> daftarKaryawan[index].role;
                saveDataKaryawan();
                tambahLog("Mengubah data karyawan ID: " + idCari);
                cout << "[+] Data karyawan berhasil diperbarui!\n";
            }
        } else {
            cout << "[-] ID Karyawan tidak ditemukan.\n";
        }
    }

    void hapusKaryawan() {
        cout << "\n=== [HAPUS KARYAWAN] ===" << endl;
        tampilkanDaftarKaryawan();
        if (jumlahKaryawan == 0) return;

        string idCari;
        cout << "\nMasukkan ID Karyawan yang akan dihapus: ";
        cin >> idCari;
        bersihkanInput();

        int index = -1;
        for (int i = 0; i < jumlahKaryawan; i++) {
            if (daftarKaryawan[i].id == idCari) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            cout << "Data yang akan dihapus: " << daftarKaryawan[index].nama << " (" << daftarKaryawan[index].role << ")\n";
            if (konfirmasiAksi("PERINGATAN: Yakin ingin menghapus data ini?")) {
                for (int i = index; i < jumlahKaryawan - 1; i++) {
                    daftarKaryawan[i] = daftarKaryawan[i + 1];
                }
                jumlahKaryawan--;
                saveDataKaryawan();
                tambahLog("Menghapus data karyawan ID: " + idCari);
                cout << "[+] Data karyawan berhasil dihapus dari sistem.\n";
            } else {
                cout << "[!] Penghapusan dibatalkan.\n";
            }
        } else {
            cout << "[-] ID Karyawan tidak ditemukan.\n";
        }
    }

    void urutkanKaryawanBubbleSort() {
        // Menggunakan Bubble Sort untuk mengurutkan karyawan berdasarkan Nama (Alfabetis)
        if (jumlahKaryawan <= 1) {
            cout << "[!] Data terlalu sedikit untuk diurutkan.\n";
            return;
        }

        for (int i = 0; i < jumlahKaryawan - 1; i++) {
            for (int j = 0; j < jumlahKaryawan - i - 1; j++) {
                if (daftarKaryawan[j].nama > daftarKaryawan[j + 1].nama) {
                    // Swap
                    Karyawan temp = daftarKaryawan[j];
                    daftarKaryawan[j] = daftarKaryawan[j + 1];
                    daftarKaryawan[j + 1] = temp;
                }
            }
        }
        saveDataKaryawan();
        tambahLog("Mengurutkan data karyawan berdasarkan nama.");
        cout << "[+] Data karyawan berhasil diurutkan berdasarkan Nama (A-Z).\n";
        tampilkanDaftarKaryawan();
    }

    void menuManajemenKaryawan() {
        bool stay = true;
        while (stay) {
            cout << "\n======================================" << endl;
            cout << "       MANAJEMEN DATA KARYAWAN        " << endl;
            cout << "======================================" << endl;
            cout << "1. Registrasi Karyawan Baru\n";
            cout << "2. Tampilkan Semua Karyawan\n";
            cout << "3. Cari Data Karyawan\n";
            cout << "4. Edit Data Karyawan\n";
            cout << "5. Hapus Data Karyawan\n";
            cout << "6. Urutkan Karyawan (A-Z)\n";
            cout << "7. Kembali ke Menu Utama\n";
            cout << "Pilih Menu: ";
            
            int pilihan = inputIntValid("");
            switch(pilihan) {
                case 1: registrasiKaryawan(); pauseLayar(); break;
                case 2: tampilkanDaftarKaryawan(); pauseLayar(); break;
                case 3: cariKaryawan(); pauseLayar(); break;
                case 4: editKaryawan(); pauseLayar(); break;
                case 5: hapusKaryawan(); pauseLayar(); break;
                case 6: urutkanKaryawanBubbleSort(); pauseLayar(); break;
                case 7: stay = false; break;
                default: cout << "[-] Pilihan tidak valid.\n"; pauseLayar(); break;
            }
        }
    }

    // ==========================================
    // MENU 2: ATUR SHIFT KARYAWAN (FUNGSI LAMA DIPERTAHANKAN & DIPERLUAS)
    // ==========================================

    void aturShiftKaryawan() {
        bool menuShift = true;
        while (menuShift) {
            cout << "\n=== [ATUR SHIFT KARYAWAN] ===" << endl;
            cout << "1. Lihat Jadwal Shift" << endl;
            cout << "2. Ubah Shift Karyawan" << endl;
            cout << "3. Kembali ke Menu Utama" << endl;
            
            int pilihan = inputIntValid("Pilih : ");

            if (pilihan == 1) {
                tampilkanDaftarKaryawan();
                tambahLog("Melihat jadwal shift karyawan.");
                pauseLayar();
            } 
            else if (pilihan == 2) {
                if (jumlahKaryawan == 0) {
                    cout << "[-] Data karyawan kosong." << endl;
                    pauseLayar();
                    continue;
                }
                tampilkanDaftarKaryawan();
                
                int noKar = inputIntValid("\nMasukkan Nomor Urut Karyawan : ");
                
                if (noKar > 0 && noKar <= jumlahKaryawan) {
                    int index = noKar - 1;
                    cout << "Opsi Shift:\n1. Pagi (07:00-15:00)\n2. Siang (15:00-23:00)\n3. Malam (23:00-07:00)\n";
                    int shiftPil = inputIntValid("Pilih Shift : ");

                    if (shiftPil == 1) daftarKaryawan[index].shift = "Pagi";
                    else if (shiftPil == 2) daftarKaryawan[index].shift = "Siang";
                    else if (shiftPil == 3) daftarKaryawan[index].shift = "Malam";
                    else {
                        cout << "[-] Pilihan shift tidak valid." << endl;
                        pauseLayar();
                        continue;
                    }
                    
                    saveDataKaryawan();
                    tambahLog("Mengubah shift karyawan: " + daftarKaryawan[index].nama);
                    cout << "[+] Shift " << daftarKaryawan[index].nama << " berhasil diubah!" << endl;
                } else {
                    cout << "[-] Nomor urut tidak valid!" << endl;
                }
                pauseLayar();
            } 
            else if (pilihan == 3) {
                menuShift = false;
            }
        }
    }

    // ==========================================
    // MENU 3: KELOLA PROMO & DISKON
    // ==========================================

    void tampilkanDaftarPromo() {
        if (jumlahPromo == 0) {
            cout << "[-] Belum ada promo yang terdaftar." << endl;
            return;
        }
        cout << "\n================= DAFTAR PROMO AKTIF =================\n";
        cout << left << setw(20) << "Kode Promo" << setw(15) << "Diskon (%)" << setw(15) << "Status" << endl;
        cout << "------------------------------------------------------\n";
        for (int i = 0; i < jumlahPromo; i++) {
            cout << left << setw(20) << daftarPromo[i].kodePromo 
                 << setw(15) << daftarPromo[i].diskonPersen 
                 << setw(15) << daftarPromo[i].status << endl;
        }
        cout << "------------------------------------------------------\n";
    }

    void kelolaDiskonPromo() {
        bool menuPromo = true;
        while (menuPromo) {
            cout << "\n=== [KELOLA DISKON & PROMO] ===" << endl;
            cout << "1. Tambah Promo Baru\n";
            cout << "2. Lihat Daftar Promo\n";
            cout << "3. Ubah Status Promo (Aktif/Nonaktif)\n";
            cout << "4. Hapus Promo\n";
            cout << "5. Kembali ke Menu Utama\n";
            
            int pilihan = inputIntValid("Pilih : ");

            if (pilihan == 1) {
                if (jumlahPromo >= 50) {
                    cout << "[-] Kapasitas data promo penuh!" << endl;
                } else {
                    cout << "Kode Promo (Tanpa Spasi) : ";
                    cin >> daftarPromo[jumlahPromo].kodePromo;
                    
                    // Cek duplikasi kode promo
                    bool duplikat = false;
                    for (int i = 0; i < jumlahPromo; i++) {
                        if (daftarPromo[i].kodePromo == daftarPromo[jumlahPromo].kodePromo) {
                            duplikat = true;
                            break;
                        }
                    }
                    
                    if(duplikat) {
                        cout << "[-] Kode promo sudah ada di database!\n";
                        bersihkanInput();
                    } else {
                        daftarPromo[jumlahPromo].diskonPersen = inputDoubleValid("Besaran Diskon (%)       : ");
                        daftarPromo[jumlahPromo].status = "Aktif";
                        
                        jumlahPromo++;
                        saveDataPromo();
                        tambahLog("Menambah promo baru: " + daftarPromo[jumlahPromo-1].kodePromo);
                        cout << "[+] Promo berhasil ditambahkan dan diaktifkan!" << endl;
                    }
                }
                pauseLayar();
            } 
            else if (pilihan == 2) {
                tampilkanDaftarPromo();
                pauseLayar();
            }
            else if (pilihan == 3) {
                tampilkanDaftarPromo();
                if (jumlahPromo > 0) {
                    string kodeCari;
                    cout << "Masukkan Kode Promo yang ingin diubah statusnya: ";
                    cin >> kodeCari;
                    bersihkanInput();
                    
                    bool ketemu = false;
                    for (int i = 0; i < jumlahPromo; i++) {
                        if (daftarPromo[i].kodePromo == kodeCari) {
                            ketemu = true;
                            if (daftarPromo[i].status == "Aktif") {
                                daftarPromo[i].status = "Nonaktif";
                            } else {
                                daftarPromo[i].status = "Aktif";
                            }
                            saveDataPromo();
                            tambahLog("Mengubah status promo: " + kodeCari);
                            cout << "[+] Status promo berhasil diubah menjadi: " << daftarPromo[i].status << "\n";
                            break;
                        }
                    }
                    if (!ketemu) cout << "[-] Kode promo tidak ditemukan.\n";
                }
                pauseLayar();
            }
            else if (pilihan == 4) {
                tampilkanDaftarPromo();
                if (jumlahPromo > 0) {
                    string kodeCari;
                    cout << "Masukkan Kode Promo yang ingin dihapus: ";
                    cin >> kodeCari;
                    bersihkanInput();
                    
                    int idx = -1;
                    for (int i = 0; i < jumlahPromo; i++) {
                        if (daftarPromo[i].kodePromo == kodeCari) {
                            idx = i; break;
                        }
                    }
                    
                    if (idx != -1) {
                        if (konfirmasiAksi("Yakin ingin menghapus promo ini?")) {
                            for (int i = idx; i < jumlahPromo - 1; i++) {
                                daftarPromo[i] = daftarPromo[i+1];
                            }
                            jumlahPromo--;
                            saveDataPromo();
                            tambahLog("Menghapus promo: " + kodeCari);
                            cout << "[+] Promo berhasil dihapus.\n";
                        }
                    } else {
                        cout << "[-] Kode promo tidak ditemukan.\n";
                    }
                }
                pauseLayar();
            }
            else if (pilihan == 5) {
                menuPromo = false;
            }
        }
    }

    // ==========================================
    // MENU 4: KELOLA DELIVERY
    // ==========================================

    void tampilkanDaftarDelivery() {
        if (jumlahDelivery == 0) {
            cout << "[-] Belum ada data pengiriman." << endl;
            return;
        }
        cout << "\n======================== DAFTAR PENGIRIMAN ========================\n";
        cout << left << setw(15) << "Resi" << setw(20) << "Penerima" << setw(15) << "Kota" << setw(15) << "Ongkir (Rp)" << endl;
        cout << "-------------------------------------------------------------------\n";
        for (int i = 0; i < jumlahDelivery; i++) {
            cout << left << setw(15) << daftarDelivery[i].idResi 
                 << setw(20) << daftarDelivery[i].namaPenerima 
                 << setw(15) << daftarDelivery[i].kotaTujuan 
                 << setw(15) << fixed << setprecision(0) << daftarDelivery[i].ongkosKirim << endl;
        }
        cout << "-------------------------------------------------------------------\n";
    }

    void urutkanDeliverySelectionSort() {
        // Selection sort berdasarkan Ongkos Kirim (Tertinggi ke Terendah)
        if (jumlahDelivery <= 1) return;
        
        for (int i = 0; i < jumlahDelivery - 1; i++) {
            int maxIdx = i;
            for (int j = i + 1; j < jumlahDelivery; j++) {
                if (daftarDelivery[j].ongkosKirim > daftarDelivery[maxIdx].ongkosKirim) {
                    maxIdx = j;
                }
            }
            if (maxIdx != i) {
                Delivery temp = daftarDelivery[i];
                daftarDelivery[i] = daftarDelivery[maxIdx];
                daftarDelivery[maxIdx] = temp;
            }
        }
        saveDataDelivery();
        tambahLog("Mengurutkan data delivery berdasarkan ongkos kirim.");
        cout << "[+] Data delivery berhasil diurutkan berdasarkan ongkos kirim (Tertinggi - Terendah).\n";
    }

    void kelolaDelivery() {
        bool menuDelivery = true;
        while (menuDelivery) {
            cout << "\n=== [KELOLA DELIVERY] ===" << endl;
            cout << "1. Input Resi Pengiriman Baru\n";
            cout << "2. Lihat Daftar Pengiriman\n";
            cout << "3. Cari Resi Pengiriman\n";
            cout << "4. Urutkan Berdasarkan Ongkir\n";
            cout << "5. Kembali ke Menu Utama\n";
            
            int pilihan = inputIntValid("Pilih : ");

            if (pilihan == 1) {
                if (jumlahDelivery >= 100) {
                    cout << "[-] Kapasitas data pengiriman penuh!" << endl;
                } else {
                    cout << "Nomor Resi             : ";
                    cin >> daftarDelivery[jumlahDelivery].idResi;
                    
                    // Cek duplikasi
                    bool duplikat = false;
                    for (int i=0; i<jumlahDelivery; i++) {
                        if(daftarDelivery[i].idResi == daftarDelivery[jumlahDelivery].idResi) {
                            duplikat = true; break;
                        }
                    }
                    
                    if (duplikat) {
                        cout << "[-] Nomor resi sudah digunakan!\n";
                        bersihkanInput();
                    } else {
                        cout << "Nama Penerima (1 Kata) : ";
                        cin >> daftarDelivery[jumlahDelivery].namaPenerima;
                        cout << "Kota Tujuan (1 Kata)   : ";
                        cin >> daftarDelivery[jumlahDelivery].kotaTujuan;
                        
                        double jarak = inputDoubleValid("Estimasi Jarak (Km)    : ");
                        daftarDelivery[jumlahDelivery].ongkosKirim = jarak * 2500;
                        
                        jumlahDelivery++;
                        saveDataDelivery();
                        tambahLog("Menginput resi delivery baru: " + daftarDelivery[jumlahDelivery-1].idResi);
                        cout << "[+] Data disimpan. Ongkos kirim: Rp " << fixed << setprecision(0) << daftarDelivery[jumlahDelivery-1].ongkosKirim << endl;
                    }
                }
                pauseLayar();
            } 
            else if (pilihan == 2) {
                tampilkanDaftarDelivery();
                pauseLayar();
            }
            else if (pilihan == 3) {
                if (jumlahDelivery == 0) { cout << "Data kosong.\n"; continue; }
                string resiCari;
                cout << "Masukkan Nomor Resi: ";
                cin >> resiCari;
                bersihkanInput();
                
                bool ketemu = false;
                for (int i = 0; i < jumlahDelivery; i++) {
                    if (daftarDelivery[i].idResi == resiCari) {
                        cout << "\nData Ditemukan:\n"
                             << "Resi     : " << daftarDelivery[i].idResi << "\n"
                             << "Penerima : " << daftarDelivery[i].namaPenerima << "\n"
                             << "Kota     : " << daftarDelivery[i].kotaTujuan << "\n"
                             << "Ongkir   : Rp " << fixed << setprecision(0) << daftarDelivery[i].ongkosKirim << "\n";
                        ketemu = true; break;
                    }
                }
                if (!ketemu) cout << "[-] Resi tidak ditemukan.\n";
                pauseLayar();
            }
            else if (pilihan == 4) {
                urutkanDeliverySelectionSort();
                tampilkanDaftarDelivery();
                pauseLayar();
            }
            else if (pilihan == 5) {
                menuDelivery = false;
            }
        }
    }

    // ==========================================
    // MENU 5 & 6: LAPORAN PENJUALAN & KEUANGAN
    // ==========================================

    void lihatLaporanPenjualan() {
        cout << "\n=== [LAPORAN PENJUALAN] ===" << endl;
        if (jumlahTransaksi == 0) {
            cout << "[-] Belum ada data transaksi penjualan." << endl;
        } else {
            cout << left << setw(5) << "No" << setw(20) << "ID Transaksi" << setw(15) << "Tanggal" << setw(20) << "Total Belanja (Rp)" << endl;
            cout << "------------------------------------------------------------" << endl;
            double totalKeseluruhan = 0;
            double penjualanTertinggi = 0;
            double penjualanTerendah = daftarTransaksi[0].totalBelanja;
            
            for (int i = 0; i < jumlahTransaksi; i++) {
                cout << left << setw(5) << i + 1 
                     << setw(20) << daftarTransaksi[i].idTransaksi 
                     << setw(15) << daftarTransaksi[i].tanggal 
                     << setw(20) << fixed << setprecision(0) << daftarTransaksi[i].totalBelanja << endl;
                
                totalKeseluruhan += daftarTransaksi[i].totalBelanja;
                
                if (daftarTransaksi[i].totalBelanja > penjualanTertinggi) 
                    penjualanTertinggi = daftarTransaksi[i].totalBelanja;
                if (daftarTransaksi[i].totalBelanja < penjualanTerendah) 
                    penjualanTerendah = daftarTransaksi[i].totalBelanja;
            }
            cout << "------------------------------------------------------------" << endl;
            cout << "TOTAL PENDAPATAN KESELURUHAN : Rp " << fixed << setprecision(0) << totalKeseluruhan << endl;
            
            // Statistik tambahan
            cout << "\n[STATISTIK PENJUALAN]\n";
            cout << "Total Transaksi      : " << jumlahTransaksi << " struk\n";
            cout << "Rata-rata Transaksi  : Rp " << fixed << setprecision(0) << (totalKeseluruhan / jumlahTransaksi) << "\n";
            cout << "Penjualan Tertinggi  : Rp " << fixed << setprecision(0) << penjualanTertinggi << "\n";
            cout << "Penjualan Terendah   : Rp " << fixed << setprecision(0) << penjualanTerendah << "\n";
            
            tambahLog("Melihat laporan penjualan dan statistik.");
        }
        pauseLayar();
    }

    void lihatLaporanKeuangan() {
        cout << "\n============================================================" << endl;
        cout << "              LAPORAN KEUANGAN SISTEM KASIR                 " << endl;
        cout << "============================================================" << endl;

        double totalPendapatan = 0;
        for (int i = 0; i < jumlahTransaksi; i++) {
            totalPendapatan += daftarTransaksi[i].totalBelanja;
        }

        double bebanDelivery = 0;
        for (int i = 0; i < jumlahDelivery; i++) {
            bebanDelivery += daftarDelivery[i].ongkosKirim;
        }

        double bebanRetur = jumlahRetur * 50000; // Asumsi biaya logistik retur
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
        if (labaBersih > 0) {
            cout << "STATUS                        : UNTUNG (PROFIT)" << endl;
        } else if (labaBersih == 0) {
            cout << "STATUS                        : BREAK EVEN POINT (BEP)" << endl;
        } else {
            cout << "STATUS                        : RUGI (DEFISIT)" << endl;
        }
        cout << "============================================================" << endl;
        
        tambahLog("Melihat ringkasan laporan keuangan (Laba/Rugi).");
        pauseLayar();
    }

    // ==========================================
    // MENU 7: KELOLA RETUR
    // ==========================================

    void tampilkanDaftarRetur() {
        if (jumlahRetur == 0) {
            cout << "[-] Belum ada data retur." << endl;
            return;
        }
        cout << "\n========================== DAFTAR BARANG RETUR ==========================\n";
        cout << left << setw(15) << "ID Retur" << setw(15) << "ID Transaksi" << setw(15) << "SKU Barang" << setw(20) << "Alasan" << endl;
        cout << "-------------------------------------------------------------------------\n";
        for (int i = 0; i < jumlahRetur; i++) {
            cout << left << setw(15) << daftarRetur[i].idRetur 
                 << setw(15) << daftarRetur[i].idTransaksi 
                 << setw(15) << daftarRetur[i].skuBarang 
                 << setw(20) << daftarRetur[i].alasan << endl;
        }
        cout << "-------------------------------------------------------------------------\n";
    }

    void kelolaRetur() {
        bool menuRetur = true;
        while (menuRetur) {
            cout << "\n=== [KELOLA RETUR BARANG] ===" << endl;
            cout << "1. Input Data Retur Baru\n";
            cout << "2. Lihat Daftar Retur\n";
            cout << "3. Cari Data Retur\n";
            cout << "4. Hapus Data Retur (Selesai Diproses)\n";
            cout << "5. Kembali ke Menu Utama\n";
            
            int pilihan = inputIntValid("Pilih : ");

            if (pilihan == 1) {
                if (jumlahRetur >= 100) {
                    cout << "[-] Kapasitas data retur penuh!" << endl;
                } else {
                    cout << "ID Retur               : ";
                    cin >> daftarRetur[jumlahRetur].idRetur;
                    
                    // Cek duplikasi
                    bool duplikat = false;
                    for (int i=0; i<jumlahRetur; i++) {
                        if(daftarRetur[i].idRetur == daftarRetur[jumlahRetur].idRetur) {
                            duplikat = true; break;
                        }
                    }
                    
                    if (duplikat) {
                        cout << "[-] ID Retur sudah digunakan!\n";
                        bersihkanInput();
                    } else {
                        cout << "ID Transaksi Terkait   : ";
                        cin >> daftarRetur[jumlahRetur].idTransaksi;
                        cout << "SKU Barang             : ";
                        cin >> daftarRetur[jumlahRetur].skuBarang;
                        cout << "Alasan (1 Kata)        : ";
                        cin >> daftarRetur[jumlahRetur].alasan;
                        
                        jumlahRetur++;
                        saveDataRetur();
                        tambahLog("Menginput retur baru: " + daftarRetur[jumlahRetur-1].idRetur);
                        cout << "[+] Data retur berhasil dicatat ke sistem!" << endl;
                    }
                }
                pauseLayar();
            } 
            else if (pilihan == 2) {
                tampilkanDaftarRetur();
                pauseLayar();
            }
            else if (pilihan == 3) {
                if (jumlahRetur == 0) { cout << "Data kosong.\n"; continue; }
                string cariID;
                cout << "Masukkan ID Retur atau ID Transaksi: ";
                cin >> cariID;
                bersihkanInput();
                
                bool ketemu = false;
                cout << "\nHasil Pencarian:\n";
                for (int i = 0; i < jumlahRetur; i++) {
                    if (daftarRetur[i].idRetur == cariID || daftarRetur[i].idTransaksi == cariID) {
                        cout << "ID Retur     : " << daftarRetur[i].idRetur << "\n"
                             << "ID Transaksi : " << daftarRetur[i].idTransaksi << "\n"
                             << "SKU          : " << daftarRetur[i].skuBarang << "\n"
                             << "Alasan       : " << daftarRetur[i].alasan << "\n"
                             << "----------------------------------\n";
                        ketemu = true;
                    }
                }
                if (!ketemu) cout << "[-] Data retur tidak ditemukan.\n";
                pauseLayar();
            }
            else if (pilihan == 4) {
                tampilkanDaftarRetur();
                if (jumlahRetur > 0) {
                    string idHapus;
                    cout << "Masukkan ID Retur yang sudah selesai & ingin dihapus: ";
                    cin >> idHapus;
                    bersihkanInput();
                    
                    int idx = -1;
                    for (int i=0; i<jumlahRetur; i++) {
                        if (daftarRetur[i].idRetur == idHapus) { idx = i; break; }
                    }
                    
                    if (idx != -1) {
                        if(konfirmasiAksi("Hapus data retur ini?")) {
                            for(int i = idx; i < jumlahRetur - 1; i++) {
                                daftarRetur[i] = daftarRetur[i+1];
                            }
                            jumlahRetur--;
                            saveDataRetur();
                            tambahLog("Menyelesaikan/menghapus retur: " + idHapus);
                            cout << "[+] Data retur berhasil dihapus.\n";
                        }
                    } else {
                        cout << "[-] ID Retur tidak ditemukan.\n";
                    }
                }
                pauseLayar();
            }
            else if (pilihan == 5) {
                menuRetur = false;
            }
        }
    }

    // ==========================================
    // MENU 8: PENGATURAN SISTEM & DATABASE
    // ==========================================

    void lihatLogAktivitas() {
        cout << "\n=== [RIWAYAT AKTIVITAS ADMIN] ===" << endl;
        if (jumlahLog == 0) {
            cout << "[-] Belum ada riwayat aktivitas." << endl;
        } else {
            cout << left << setw(22) << "Waktu" << "Deskripsi Aktivitas" << endl;
            cout << "----------------------------------------------------------------------" << endl;
            // Tampilkan log dari yang terbaru (reverse loop)
            for (int i = jumlahLog - 1; i >= 0; i--) {
                cout << left << setw(22) << daftarLog[i].waktu << daftarLog[i].deskripsi << endl;
            }
            cout << "----------------------------------------------------------------------" << endl;
        }
    }

    void backupDataSistem() {
        if(konfirmasiAksi("Apakah Anda yakin ingin melakukan Backup semua data ke file 'Backup_'?")) {
            // Backup Karyawan
            ofstream bkKar("Backup_dataKaryawan.txt");
            for (int i = 0; i < jumlahKaryawan; i++) 
                bkKar << daftarKaryawan[i].id << " " << daftarKaryawan[i].nama << " " << daftarKaryawan[i].role << " " << daftarKaryawan[i].shift << "\n";
            
            // Backup Promo
            ofstream bkPro("Backup_data_promo.txt");
            for (int i = 0; i < jumlahPromo; i++) 
                bkPro << daftarPromo[i].kodePromo << " " << daftarPromo[i].diskonPersen << " " << daftarPromo[i].status << "\n";
            
            // Backup Delivery
            ofstream bkDel("Backup_data_delivery.txt");
            for (int i = 0; i < jumlahDelivery; i++) 
                bkDel << daftarDelivery[i].idResi << " " << daftarDelivery[i].namaPenerima << " " << daftarDelivery[i].kotaTujuan << " " << daftarDelivery[i].ongkosKirim << "\n";
            
            // Backup Transaksi & Retur
            ofstream bkTrx("Backup_transaksi.txt");
            for (int i = 0; i < jumlahTransaksi; i++) 
                bkTrx << daftarTransaksi[i].idTransaksi << " " << daftarTransaksi[i].tanggal << " " << daftarTransaksi[i].totalBelanja << "\n";
            
            ofstream bkRet("Backup_retur.txt");
            for (int i = 0; i < jumlahRetur; i++) 
                bkRet << daftarRetur[i].idRetur << " " << daftarRetur[i].idTransaksi << " " << daftarRetur[i].skuBarang << " " << daftarRetur[i].alasan << "\n";

            tambahLog("Melakukan ekspor/backup seluruh data sistem.");
            cout << "[+] Seluruh data berhasil di-backup!\n";
        }
    }

    void pengaturanSistem() {
        bool stay = true;
        while (stay) {
            cout << "\n=== [PENGATURAN SISTEM] ===" << endl;
            cout << "1. Ubah PIN Admin\n";
            cout << "2. Lihat Log Aktivitas\n";
            cout << "3. Backup Database Manual\n";
            cout << "4. Kembali ke Menu Utama\n";
            
            int pilihan = inputIntValid("Pilih : ");
            switch(pilihan) {
                case 1: ubahPasswordAdmin(); pauseLayar(); break;
                case 2: lihatLogAktivitas(); pauseLayar(); break;
                case 3: backupDataSistem(); pauseLayar(); break;
                case 4: stay = false; break;
                default: cout << "[-] Pilihan tidak valid.\n"; pauseLayar(); break;
            }
        }
    }

    // ==========================================
    // MENU 9: BANTUAN (HELP)
    // ==========================================

    void tampilkanBantuan() {
        cout << "\n==================== PUSAT BANTUAN SISTEM ====================" << endl;
        cout << "Aplikasi ini adalah Sistem Manajemen Back-Office Kasir V2.0\n";
        cout << "Petunjuk Penggunaan Modul:\n";
        cout << "1. [Karyawan]   : Kelola data staf (CRUD), shift, dan urutkan data.\n";
        cout << "                  Gunakan menu 'Urutkan' agar mudah mencari staf.\n";
        cout << "2. [Shift]      : Alokasikan waktu jaga staf (Pagi/Siang/Malam).\n";
        cout << "3. [Promo]      : Input kode unik diskon. Pastikan status 'Aktif'.\n";
        cout << "4. [Delivery]   : Input resi pengiriman logistik (integrasi ongkir).\n";
        cout << "5. [Penjualan]  : Tinjau semua struk transaksi dan statistik harian.\n";
        cout << "6. [Keuangan]   : Kalkulasi otomatis Laba/Rugi (Pendapatan - Beban).\n";
        cout << "7. [Retur]      : Catat barang kembali dari konsumen (mengurangi laba).\n";
        cout << "8. [Pengaturan] : Ganti PIN, lihat log, atau backup database.\n";
        cout << "--------------------------------------------------------------\n";
        cout << "* Catatan: Sistem mem-backup data secara realtime ke file .txt\n";
        cout << "==============================================================\n";
        tambahLog("Membuka menu bantuan (Help).");
        pauseLayar();
    }

public:
    Admin() {
        // Load config & data saat objek dicreate
        loadKonfigurasiSistem();
        loadDataKaryawan();
        loadDataPromo();
        loadDataDelivery();
        loadDataTransaksi();
        loadDataRetur();
        loadDataLog();
    }

    void tampilkanMenu() {
        validasiKeamanan(); 
        
        bool isLogin = true;
        while (isLogin) {
            cout << "\n========================================================" << endl;
            cout << "       SISTEM KASIR TERPADU - DASHBOARD ADMIN           " << endl;
            cout << "========================================================" << endl;
            cout << "1. Manajemen Karyawan\n";
            cout << "2. Manajemen Shift & Jadwal\n";
            cout << "3. Kelola Diskon & Promo\n";
            cout << "4. Kelola Data Delivery (Pengiriman)\n";
            cout << "5. Lihat Laporan & Statistik Penjualan\n";
            cout << "6. Lihat Laporan Keuangan (Laba/Rugi)\n";
            cout << "7. Kelola Retur Barang\n";
            cout << "8. Pengaturan Sistem (Log & Backup)\n";
            cout << "9. Bantuan (Help)\n";
            cout << "10. Logout Keluar Aplikasi\n";
            cout << "========================================================" << endl;
            
            int pilihan = inputIntValid("Pilih Menu : ");

            switch (pilihan) {
                case 1: menuManajemenKaryawan(); break;
                case 2: aturShiftKaryawan(); break;
                case 3: kelolaDiskonPromo(); break;
                case 4: kelolaDelivery(); break;
                case 5: lihatLaporanPenjualan(); break;
                case 6: lihatLaporanKeuangan(); break;
                case 7: kelolaRetur(); break;
                case 8: pengaturanSistem(); break;
                case 9: tampilkanBantuan(); break;
                case 10: 
                    tambahLog("Logout dari sistem.");
                    cout << "\n[+] Menyimpan semua data dan menutup sesi... Sampai jumpa, " << usernameAdmin << "!" << endl;
                    isLogin = false; 
                    break;
                default: 
                    cout << "\n[-] Pilihan tidak valid. Silakan coba lagi." << endl; 
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
	private:
	    // -----------------------------------------------------------------
	    // HELPER PRIVATE 1: Memulihkan kuantitas stok di katalogProduk.txt
	    // -----------------------------------------------------------------
	    void pulihkanStokKatalogGudang(string skuBarang, int jumlahAman) {
	        ifstream fKatalogIn("katalogProduk.txt");
	        ofstream fKatalogTemp("tempKatalog.txt");
	
	        string line;
	        if (fKatalogIn.is_open() && fKatalogTemp.is_open()) {
	            while (getline(fKatalogIn, line)) {
	                if (line.empty()) continue;
	                stringstream ss(line);
	                
	                Barang b; 
					string hStr, sStr, tStr;
	
	                getline(ss, b.id, ';');       
					getline(ss, b.nama, ';');       
					getline(ss, b.jenis, ';');
	                getline(ss, hStr, ';');       
					getline(ss, sStr, ';');         
					getline(ss, tStr, ';');
	                
	                stringstream(hStr) >> b.harga;
	                stringstream(sStr) >> b.stok;
	                stringstream(tStr) >> b.terjual;
	
	                if (b.id == skuBarang) {
	                    b.stok += jumlahAman;       
	                    b.terjual -= jumlahAman;    
	                    if (b.terjual < 0) b.terjual = 0;
	                }
	
	                fKatalogTemp << b.id << ";" << b.nama << ";" << b.jenis << ";" 
	                             << b.harga << ";" << b.stok << ";" << b.terjual << endl;
	            }
	            fKatalogIn.close();
	            fKatalogTemp.close();
	
	            remove("katalogProduk.txt");                  
	            rename("tempKatalog.txt", "katalogProduk.txt"); 
	        }
	    }
	
	    // -----------------------------------------------------------------
	    // HELPER PRIVATE 2: Format Rupiah Standar 
	    // -----------------------------------------------------------------
	    string formatRupiah(long long nominal) {
	        stringstream ss;
	        ss << nominal;
	        return ss.str();
	    }
	
	    // -----------------------------------------------------------------
	    // HELPER PRIVATE 3: Fungsi Cetak Struk Bukti Klaim Retur Customer
	    // -----------------------------------------------------------------
	    void cetakStrukNotaReturKasir(DataRetur data) {
	        cout << "\n========================================";
	        cout << "\n       SUPERMARKET TANDA TERIMA RETUR   ";
	        cout << "\n========================================";
	        cout << "\nID Bukti Retur: " << data.idRetur;
	        cout << "\nNo Nota Asli  : " << data.noStruk;      
	        cout << "\nID Konsumen   : " << data.idCustomer;
	        cout << "\nTanggal Proses: " << data.tanggal;       
	        cout << "\n----------------------------------------";
	        cout << "\nID SKU Barang : " << data.skuBarang;     
	        cout << "\nJumlah Unit   : " << data.jumlah << " pcs";
	        cout << "\nStatus Hasil  : [" << data.status << "]";
	        cout << "\nCatatan Kasir : " << data.catatan_admin;
	        cout << "\n========================================";
	        cout << "\n      Dokumen Sah Sistem Kasir     ";
	        cout << "\n========================================\n";
	    }
	
	    // -----------------------------------------------------------------
	    // HELPER PRIVATE 4: Mesin Printer Cetak Struk Format Alfamidi (.txt)
	    // -----------------------------------------------------------------
	    // Tambahkan variabel nominalDiskon ke parameter agar dikenal oleh fungsi!
	    void prosesPembayaranDanCetakAlfamidi(string idStruk, string idCustomer, string tanggalNota, long long nominalDiskon) {
	        long long uangBayar;
	        long long totalWajibBayar = subtotal - nominalDiskon;
	
	        do {
	            cout << "Masukkan Nominal Uang Tunai Pembayaran: Rp ";
	            cin >> uangBayar;
	            if (uangBayar < totalWajibBayar) {
	                cout << "? Uang Tunai Kurang!\n";
	            }
	        } while (uangBayar < totalWajibBayar);
	
	        long long kembalian = uangBayar - totalWajibBayar;
	
	        ifstream fAsli("transaksi.txt");
	        ofstream fTemp("tempTransaksi.txt");
	        bool isUpdate = false;
	
	        if (fAsli.is_open() && fTemp.is_open()) {
	            string line;
	            while (getline(fAsli, line)) {
	                if (line.empty()) continue;
	                stringstream ss(line); string idCheck; getline(ss, idCheck, ';');
	                if (idCheck == idStruk) {
	                    fTemp << idStruk << ";" << idCustomer << ";" << subtotal << ";" << nominalDiskon << ";" 
	                          << 0.0 << ";" << totalWajibBayar << ";" << "Tunai" << ";" << jumlahKeranjang << ";" << tanggalNota << endl;
	                    isUpdate = true;
	                } else fTemp << line << endl;
	            }
	            fAsli.close(); fTemp.close();
	            remove("transaksi.txt"); rename("tempTransaksi.txt", "transaksi.txt");
	        }
	
	        if (!isUpdate) {
	            ofstream fTrxOut("transaksi.txt", ios::app);
	            if (fTrxOut.is_open()) {
	                fTrxOut << idStruk << ";" << idCustomer << ";" << subtotal << ";" << nominalDiskon << ";" 
	                        << 0.0 << ";" << totalWajibBayar << ";" << "Tunai" << ";" << jumlahKeranjang << ";" << tanggalNota << endl;
	                fTrxOut.close();
	            }
	        }
	
	        // Mulai Cetak File Nota Txt
	        string namaFileNota = "Nota_" + idStruk + ".txt";
	        ofstream fileNota(namaFileNota.c_str());
	
	        if (fileNota.is_open()) {
	            fileNota << "         ALFAMIDI KI AGENG PEMANAHAN\n";
	            fileNota << "          KI AGENG PEMANAHAN [KAPB]\n\n";
	            fileNota << "            JL. KI AGENG PEMANAHAN\n";
	            fileNota << "--------------------------------------------------\n";
	            fileNota << left << setw(18) << ("Bon  " + idStruk) << "Kasir : MUKHAMMAD\n";
	            fileNota << "--------------------------------------------------\n";
	            
	            ifstream fileDetailIn2("detailTransaksi.txt");
	            if (fileDetailIn2.is_open()) {
	                string barisDetail;
	                while (getline(fileDetailIn2, barisDetail)) {
	                    if (barisDetail.empty()) continue;
	                    stringstream ss(barisDetail);
	                    string idTrxTemp, namaBrgTemp, qtyTemp, subTemp;
	                    getline(ss, idTrxTemp, ';'); getline(ss, namaBrgTemp, ';'); getline(ss, qtyTemp, ';'); getline(ss, subTemp, ';');
	
	                    if (idTrxTemp == idStruk) {
	                        long long subTotalItem = 0; int qtyItem = 0;
	                        stringstream(subTemp) >> subTotalItem;
	                        stringstream(qtyTemp) >> qtyItem;
	                        long long hargaSatuan = (qtyItem > 0) ? (subTotalItem / qtyItem) : subTotalItem;
	
	                        fileNota << left << setw(25) << namaBrgTemp << "\n";
	                        fileNota << left << "  " << setw(10) << qtyItem << setw(15) << formatRupiah(hargaSatuan) 
	                                 << right << setw(21) << formatRupiah(subTotalItem) << "\n";
	                    }
	                }
	                fileDetailIn2.close();
	            }
	            
	            if (nominalDiskon > 0) {
	                fileNota << left << " Disc. " << right << setw(31) << ("-" + formatRupiah(nominalDiskon)) << "\n";
	            }
	            fileNota << "--------------------------------------------------\n";
	            fileNota << left << setw(20) << "Total Item" << setw(5) << jumlahKeranjang << right << setw(13) << formatRupiah(subtotal) << "\n";
	            fileNota << left << setw(25) << "Total Disc." << right << setw(13) << formatRupiah(nominalDiskon) << "\n";
	            fileNota << left << setw(25) << "Total Belanja" << right << setw(13) << formatRupiah(totalWajibBayar) << "\n";
	            fileNota << left << setw(25) << "QRIS BRI BRIMO" << right << setw(13) << formatRupiah(uangBayar) << "\n";
	            fileNota << left << setw(25) << "Kembalian" << right << setw(13) << formatRupiah(kembalian) << "\n";
	            
	            double dpp = totalWajibBayar / 1.11; double ppn = dpp * 0.11;
	            fileNota << left << "REGULER   " << "DPP: " << left << setw(10) << fixed << setprecision(0) << dpp << "PPN: " << fixed << setprecision(0) << ppn << "\n";
	            fileNota << "--------------------------------------------------\n";
	            fileNota << "Tgl. " << tanggalNota << " 17:59:56 V.2025.11.0\n";
	            fileNota << "       PT MIDI UTAMA INDONESIA TBK\n";
	            fileNota << "==================================================\n";
	            fileNota.close(); 
	            cout << "\n? [PRINTER OK] Struk Alfamidi sukses dicetak ke file: " << namaFileNota << endl;
	        }
	        cout << "\nKembalian : Rp " << formatRupiah(kembalian) << "\n========================================\n";
	    }

			
		public:
			
			 // Konstruktor Kasir (Inisialisasi data transaksi & keranjang)
		    // Banyak baris kode : 11 baris
		    static const int MAX_KERANJANG = 50;
		    ItemTransaksi keranjang[MAX_KERANJANG];
		    int jumlahKeranjang;
		    long long subtotal;
		    long long nominalDiskon;
		    string kodeDiskonAktif;
		
		Kasir() // ?? TAMBAHKAN TANDA KURUNG () DI SINI!
	    {
	        jumlahKeranjang = 0;
	        subtotal = 0;
	        nominalDiskon = 0;
	        kodeDiskonAktif = "-";
	   };
			
			
		// =========================================================================
	    // FUNGSI UTAMA: INPUT TRANSAKSI POS KASIR (PILIHAN otomatis / FISIK MANUAL)
	    // =========================================================================
	    void inputTransaksi() {
	        jumlahKeranjang = 0;
	        subtotal = 0;
	        nominalDiskon = 0;
	        kodeDiskonAktif = "-";
	
	        cout << "\n========================================";
	        cout << "\n           SISTEM POS KASIR";
	        cout << "\n========================================\n";
	        cout << "1. Proses ID Struk Pembelian Customer (Otomatis Load)\n";
	        cout << "2. Input Transaksi Manual / Belanja Fisik Langsung\n";
	        cout << "Pilih Jenis Pelayanan (1-2): ";
	        int jenisPelayanan;
	        cin >> jenisPelayanan;
	
	        // ---------------------------------------------------------------------
	        // ALUR OPSI 1: OTOMATIS LOAD STRUK BELANJA DARI FILE
	        // ---------------------------------------------------------------------
	        if (jenisPelayanan == 1) {
	            string idStrukCari;
	            cout << "Masukkan ID Nomor Struk Pembelian Customer: ";
	            cin >> idStrukCari;
	
	            ifstream fileTrxIn("transaksi.txt");
	            if (!fileTrxIn.is_open()) {
	                cout << "? Error: Berkas transaksi pusat tidak ditemukan!\n";
	                return;
	            }
	
	            string line;
	            bool notaDitemukan = false;
	            RiwayatTransaksi trxTarget; 
	
	            while (getline(fileTrxIn, line)) {
	                if (line.empty()) continue;
	                stringstream ss(line);
	                string subStr, discStr, ongStr, totStr, itemStr;
	
	                getline(ss, trxTarget.idTransaksi, ';');
	                getline(ss, trxTarget.idCustomer,  ';');
	                getline(ss, subStr,                ';');
	                getline(ss, discStr,               ';');
	                getline(ss, ongStr,                ';');
	                getline(ss, totStr,                ';');
	                getline(ss, trxTarget.metode,      ';'); 
	                getline(ss, itemStr,               ';');
	                getline(ss, trxTarget.tanggal,     ';');
	
	                stringstream(subStr) >> trxTarget.subtotal;
	                stringstream(discStr)   >> trxTarget.diskon;
	                stringstream(totStr)    >> trxTarget.totalBelanja; 
	                trxTarget.jumlahItem = atoi(itemStr.c_str());
	
	                if (trxTarget.idTransaksi == idStrukCari) {
	                    if (trxTarget.metode != "Belum_Lunas") {
	                        cout << "? Gagal! ID Struk " << idStrukCari << " sudah berstatus LUNAS.\n";
	                        fileTrxIn.close(); 
	                        return;
	                    }
	                    notaDitemukan = true; 
	                    break;
	                }
	            } // Tutup perulangan while database transaksi induk
	            fileTrxIn.close();
	
	            if (!notaDitemukan) {
	                cout << "? Maaf, ID Struk tidak ditemukan di sistem!\n";
	                return;
	            }
	
	            subtotal = trxTarget.subtotal;
	            nominalDiskon = trxTarget.diskon;
	
	            cout << "\n========================================";
	            cout << "\n    RINCIAN BARANG STRUK: " << idStrukCari;
	            cout << "\n========================================";
	            cout << "\n" << left << setw(20) << "Nama Barang" << setw(10) << "Qty" << "Subtotal\n";
	            cout << "----------------------------------------\n";
	
	            ifstream fileDetailIn("detailTransaksi.txt");
	            if (fileDetailIn.is_open()) {
	                while (getline(fileDetailIn, line)) {
	                    if (line.empty()) continue;
	                    stringstream ss(line);
	                    string idTrxTemp, namaBrgTemp, qtyTemp, subTemp;
	                    getline(ss, idTrxTemp, ';'); getline(ss, namaBrgTemp, ';'); getline(ss, qtyTemp, ';'); getline(ss, subTemp, ';');
	
	                    if (idTrxTemp == idStrukCari) {
	                        cout << left << setw(20) << namaBrgTemp << setw(10) << qtyTemp << "Rp " << subTemp << endl;
	                        
	                        long long subTotalItem = 0;
	                        int qtyItem = 0;
	                        stringstream(subTemp) >> subTotalItem;
	                        stringstream(qtyTemp) >> qtyItem;
	
	                        keranjang[jumlahKeranjang].nama = namaBrgTemp;
	                        keranjang[jumlahKeranjang].qty = qtyItem;
	                        keranjang[jumlahKeranjang].subtotal = subTotalItem;
	                        keranjang[jumlahKeranjang].harga = (qtyItem > 0) ? (subTotalItem / qtyItem) : subTotalItem;
	                        jumlahKeranjang++;
	                    }
	                } // Tutup perulangan while detail rincian item
	                fileDetailIn.close();
	            }
	            cout << "----------------------------------------\n";
	            cout << "Total Belanja Bersih: Rp " << formatRupiah(subtotal) << endl;
	            cout << "========================================\n";
	
	            // Selesaikan pembayaran dengan melemparkan parameter diskon yang sah
	            prosesPembayaranDanCetakAlfamidi(idStrukCari, trxTarget.idCustomer, trxTarget.tanggal, nominalDiskon);
	        }
	        // ---------------------------------------------------------------------
	        // ALUR OPSI 2: INPUT MANUAL BELANJA FISIK LANGSUNG (KODE ASLI ANDA)
	        // ---------------------------------------------------------------------
	        else if (jenisPelayanan == 2) {
	            char lanjut = 'y';
	            while (lanjut == 'y' || lanjut == 'Y') {
	                if (jumlahKeranjang >= MAX_KERANJANG) {
	                    cout << "\n[GAGAL] Keranjang sudah penuh (maks " << MAX_KERANJANG << " item)!\n";
	                    break;
	                }
	
	                char sumber;
	                cout << "\nAmbil barang dari data Gudang? (y/n) : ";
	                cin >> sumber;
	
	                ItemTransaksi item;
	
	                if (sumber == 'y' || sumber == 'Y') {
	                    string id;
	                    cout << "ID Barang di Gudang : "; cin >> id;
	
	                    int idx = -1;
	                    for (int i = 0; i < jumlahBarang; i++) {
	                        if (daftarBarang[i].id == id) { idx = i; break; }
	                    }
	
	                    if (idx == -1) {
	                        cout << "\n[GAGAL] Barang dengan ID tersebut tidak ada di gudang!\n";
	                        cout << "Tambah barang lain? (y/n): "; cin >> lanjut; 
	                        continue;
	                    }
	
	                    cout << "Nama Barang   : " << daftarBarang[idx].nama;
	                    cout << "\nHarga         : Rp" << formatRupiah(daftarBarang[idx].harga);
	                    cout << "\nStok Tersedia : " << daftarBarang[idx].stok;
	                    cout << "\nJumlah Beli   : "; cin >> item.qty;
	
	                    if (item.qty <= 0 || item.qty > daftarBarang[idx].stok) {
	                        cout << "\n[GAGAL] Jumlah tidak valid atau melebihi stok!\n";
	                        cout << "Tambah barang lain? (y/n): "; cin >> lanjut; 
	                        continue;
	                    }
	
	                    item.nama     = daftarBarang[idx].nama;
	                    item.harga    = daftarBarang[idx].harga;
	                    item.subtotal = item.harga * item.qty;
	
	                    daftarBarang[idx].stok    -= item.qty;
	                    daftarBarang[idx].terjual += item.qty;
	                } else {
	                    bersihkanInput();
	                    cout << "Nama Barang   : "; getline(cin, item.nama);
	                    cout << "Harga Barang  : Rp"; cin >> item.harga;
	                    cout << "Jumlah Barang : "; cin >> item.qty;
	                    item.subtotal = item.harga * item.qty;
	                }
	
	                keranjang[jumlahKeranjang] = item;
	                jumlahKeranjang++;
	                subtotal += item.subtotal;
	
	                cout << "\n[ TAMBAH ] " << item.nama << " x" << item.qty << " = Rp" << formatRupiah(item.subtotal);
	                cout << "\nTambah barang lain? (y/n): "; cin >> lanjut;
	            } // Tutup loop perulangan input barang manual
	
	            cout << "\n========================================";
	            cout << "\nSubtotal sementara: Rp" << formatRupiah(subtotal);
	            cout << "\n========================================\n";
	
	            string idNotaManual = ValidasiDanGenerate::generateIDOtomatis("TRX", "transaksi.txt");
	            string tanggalHariIni = ValidasiDanGenerate::generateTanggal();
	            
	            prosesPembayaranDanCetakAlfamidi(idNotaManual, "CUS_OFFLINE", tanggalHariIni, nominalDiskon);
	        }
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
	            trx.totalBelanja      = total;
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
	
	  	// Fungsi Kelola Retur (Mengajukan retur barang customer)
	    // Banyak baris kode : 31 baris
	    // Fungsi Utama Kasir untuk Memproses dan Memvalidasi Klaim Retur dari Customer
	    void kelolaReturKasir() {
	    cout << "\n=================================================================================";
	    cout << "\n                        SISTEM VERIFIKASI RETUR (KASIR)";
	    cout << "\n=================================================================================\n";
	
	    // 1. TAMPILKAN SEMUA ANTREAN RETUR LANGSUNG DARI FILE
	    ifstream fileIn("dataRetur.txt");
	    if (!fileIn.is_open()) {
	        cout << "                 [INFO] Berkas database retur belum tersedia di sistem.          \n";
	        cout << "=================================================================================\n";
	        return;
	    }
	
	    string line;
	    int antreanCount = 0;
	    
	    // Header Tampilan
	    cout << left 
			 << setw(10) << "ID Retur" 
			 << setw(12) << "No Struk" 
			 << setw(10) << "SKU Brg" 
	         << setw(10) << "ID Cus" 
			 << setw(6) << "Qty" 
			 << setw(12) << "Tanggal" 
			 << "Status Keluhan\n";
	    cout << "---------------------------------------------------------------------------------\n";
	
	    while (getline(fileIn, line)) {
	        if (line.empty()) continue;
	        stringstream ss(line);
	        DataRetur r;
	        string jumlahStr;
	
	        // parsing data sesuai urutan struct DataRetur Anda (disesuaikan dengan noStruk/skuBarang)
	        getline(ss, r.idRetur, ';');       
			getline(ss, r.noStruk, ';');
	        getline(ss, r.skuBarang, ';');     
			getline(ss, r.idCustomer, ';');
	        getline(ss, jumlahStr, ';');       r.jumlah = atoi(jumlahStr.c_str());
			getline(ss, r.alasan, ';');
	        getline(ss, r.tanggal, ';');       
			getline(ss, r.status, ';');
	        getline(ss, r.catatan_admin, ';');
	
	        // Tampilkan antrean di layar kasir
	        cout << left 
				 << setw(10) << r.idRetur 
				 << setw(12) << r.noStruk 
				 << setw(10) << (r.skuBarang.empty() ? "[KOSONG]" : r.skuBarang)
	             << setw(10) << r.idCustomer 
				 << setw(6) << r.jumlah 
				 << setw(12) << r.tanggal 
				 << r.status << endl;
	        
	        if (r.status == "PENDING" || r.status == "pending") {
	            antreanCount++;
	        }
	    }
	    fileIn.close();
	    cout << "---------------------------------------------------------------------------------\n";
	    cout << "Total Antrean Klaim PENDING saat ini: " << antreanCount << " pengajuan.\n";
	    cout << "=================================================================================\n";
	
	    // 2. PROSES PENCARIAN DATA TARGET SECARA MANUAL DI HARDDISK
	    string idReturCari;
	    cout << "Masukkan ID Retur yang ingin diproses (Ketik '-' untuk batal): ";
	    cin >> idReturCari;
	
	    if (idReturCari == "-") {
	        cout << "? Proses manajemen retur kasir dibatalkan.\n";
	        return;
	    }
	
		// buka file kembali untuk menguci data target secara spesifik
	    fileIn.open("dataRetur.txt");
	    bool targetKetemu = false;
	    DataRetur targetData;
	
	    while (getline(fileIn, line)) {
	        if (line.empty()) 
				continue;
				
	        stringstream ss(line);
	        DataRetur r;
	        string jumlahStr;
	
			// parsing 9 kolom sesuai urutan
	        getline(ss, r.idRetur, ';');       
			getline(ss, r.noStruk, ';');
	        getline(ss, r.skuBarang, ';');     
			getline(ss, r.idCustomer, ';');
	        getline(ss, jumlahStr, ';');      r.jumlah = atoi(jumlahStr.c_str());
	        
			getline(ss, r.alasan, ';');	
	        getline(ss, r.tanggal, ';');      
			getline(ss, r.status, ';');
	        getline(ss, r.catatan_admin, ';');
	
	        if (r.idRetur == idReturCari) {
	            if (r.status != "PENDING" && r.status != "pending") {
	                cout << "? Gagal! ID Retur " << idReturCari << " sudah pernah diproses sebelumnya.\n";
	                fileIn.close();
	                return;
	            }
	            targetKetemu = true;
	            targetData = r; // Mengunci data target ke variabel tunggal
	            break;
	        }
	    }
	    fileIn.close();
	
	    if (!targetKetemu) {
	        cout << "? Maaf, ID Retur '" << idReturCari << "' tidak terdaftar di sistem!\n";
	        return;
	    }
	
	    // Tampilkan detail keluhan konsumen terpilih
	    cout << "\n>>> DETAIL KLAIM KONSUMEN <<<";
	    cout << "\nNomor Struk Transaksi : " << targetData.noStruk;
	    cout << "\nID Pelanggan (User)   : " << targetData.idCustomer;
	    cout << "\nSKU Kode Barang       : " << targetData.skuBarang;
	    cout << "\nKuantitas Pengembalian: " << targetData.jumlah << " unit";
	    cout << "\nAlasan Klaim Cacat    : " << targetData.alasan;
	    cout << "\n-------------------------------------------------\n";
	
	    // 3. INPUT VERIFIKASI FISIK OLEH KASIR
	    cout << "Hasil Pemeriksaan Fisik Barang (1. Disetujui / 2. Ditolak): ";
	    int keputusanKasir;
	    cin >> keputusanKasir;
	
	    string catatanKeputusan;
	    cin.ignore(1000, '\n');
	    cout << "Masukkan Catatan Penjelasan Kasir: ";
	    getline(cin, catatanKeputusan);
	
	    string statusBaru = "Ditolak";
	    if (keputusanKasir == 1) {
	        statusBaru = "Disetujui";
	        // Memanggil fungsi internal private Anda untuk memulihkan stok gudang
	        pulihkanStokKatalogGudang(targetData.skuBarang, targetData.jumlah);
	        cout << "\n? Berhasil! Stok barang otomatis dikembalikan ke sistem gudang utama.\n";
	    } else if (keputusanKasir != 2) {
	        cout << "? Pilihan tindakan ilegal! Operasi dibatalkan.\n";
	        return;
	    }
	
	    // 4. ALGORITMA PENULISAN ULANG FILE MANUAL (STREAM SWITCHING - TANPA VECTOR)
	    ifstream fAsli("dataRetur.txt");
	    ofstream fTemp("tempRetur.txt");
	
	    if (fAsli.is_open() && fTemp.is_open()) {
	        while (getline(fAsli, line)) {
	            if (line.empty()) continue;
	            stringstream ss(line);
	            string idRTemp;
	            getline(ss, idRTemp, ';'); // Ambil ID Retur di baris ini untuk pembanding
	
	            if (idRTemp == idReturCari) {
	                // Tulis baris baru hasil modifikasi kasir
	                fTemp << targetData.idRetur << ";" 
	                      << targetData.noStruk << ";" 
	                      << targetData.skuBarang << ";" 
	                      << targetData.idCustomer << ";" 
	                      << targetData.jumlah << ";" 
	                      << targetData.namaBarang << ";" 
	                      << targetData.alasan << ";" 
	                      << targetData.tanggal << ";" 
	                      << statusBaru << ";" 
	                      << catatanKeputusan << endl;
	            } else {
	                // Salin baris data asli lainnya tanpa mengubah isi teks
	                fTemp << line << endl;
	            }
	        }
	        fAsli.close();
	        fTemp.close();
	
	        // Operasi penggantian file murni bawaan harddisk
	        remove("dataRetur.txt");
	        rename("tempRetur.txt", "dataRetur.txt");
	    }
	
	    // 5. CETAK NOTA TANDA TERIMA RETUR FISIK UNTUK CUSTOMER
	    targetData.status = statusBaru;
	    targetData.catatan_admin = catatanKeputusan;
	    cetakStrukNotaReturKasir(targetData);
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
	                 << "Rp" << setw(12) << formatRupiah(trx.totalBelanja)
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
	                cout << "\nTotal        : Rp" << formatRupiah(trx.totalBelanja);
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
	        long long transaksiTertinggi = daftarTransaksi[0].totalBelanja;
	        long long transaksiTerendah  = daftarTransaksi[0].totalBelanja;
	        string    noStrukTertinggi   = daftarTransaksi[0].noStruk;
	
	        for (int i = 0; i < jumlahTransaksi; i++){
	            totalOmzet += daftarTransaksi[i].totalBelanja;
	
	            if (daftarTransaksi[i].totalBelanja > transaksiTertinggi){
	                transaksiTertinggi = daftarTransaksi[i].totalBelanja;
	                noStrukTertinggi   = daftarTransaksi[i].noStruk;
	            }
	            if (daftarTransaksi[i].totalBelanja < transaksiTerendah){
	                transaksiTerendah = daftarTransaksi[i].totalBelanja;
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

        cout << "\n[ SIMPAN ] Data supplier berhasil disimpan.";
        cout << "\n========================================\n";
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

        daftarBarang[idx].harga = hargaBaru;
        
        simpanKeFile();

        cout << "\n[ UPDATE ] Harga berhasil diperbarui.";
        cout << "\n========================================\n";
    }


    void urutkanBarang(){
        cout << "\n========================================";
        cout << "\n          URUTKAN DATA BARANG";
        cout << "\n========================================\n";

        if (jumlahBarang < 2){
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

        // bubble sort sederhana
        for (int i = 0; i < jumlahBarang - 1; i++){
            for (int j = 0; j < jumlahBarang - 1 - i; j++){
                bool tukar = false;

                if      (pilih == 1 && daftarBarang[j].nama  > daftarBarang[j + 1].nama)  tukar = true;
                else if (pilih == 2 && daftarBarang[j].harga > daftarBarang[j + 1].harga) tukar = true;
                else if (pilih == 3 && daftarBarang[j].stok  < daftarBarang[j + 1].stok)  tukar = true;

                if (tukar){
                    Barang temp        = daftarBarang[j];
                    daftarBarang[j]    = daftarBarang[j + 1];
                    daftarBarang[j+1]  = temp;
                }
            }
        }

        cout << "\n[ SUKSES ] Data barang berhasil diurutkan.\n";
        tampilStok();
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

        // salin data supaya urutan asli daftarBarang tidak berubah
        Barang temp[MAX_BARANG];
        for (int i = 0; i < jumlahBarang; i++) temp[i] = daftarBarang[i];

        for (int i = 0; i < jumlahBarang - 1; i++){
            for (int j = 0; j < jumlahBarang - 1 - i; j++){
                if (temp[j].terjual < temp[j + 1].terjual){
                    Barang t   = temp[j];
                    temp[j]    = temp[j + 1];
                    temp[j+1]  = t;
                }
            }
        }

        int batas = jumlahBarang < 5 ? jumlahBarang : 5;
        cout << left << setw(6) << "Rank" << setw(20) << "Nama" << "Terjual\n";
        cout << "----------------------------------------\n";
        for (int i = 0; i < batas; i++){
            cout << left << setw(6) << (i + 1) << setw(20) << temp[i].nama
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
class Staff {
private:
    Kasir kasir;
    Gudang gudang;

public:
    void menuKasir() {
        int pilih;
        do {
            cout << "\n========================================";
            cout << "\n              MENU KASIR";
            cout << "\n========================================";
            cout << "\n1. Input Transaksi POS (Fisik / Struk Online)";
            cout << "\n2. Lihat / Batalkan Item Keranjang";
            cout << "\n3. Input Voucher Diskon Toko";
            cout << "\n4. Tampilkan Ringkasan Isi Keranjang Aktif";
            cout << "\n5. Fitur Split Bill (Bagi Tagihan)";
            cout << "\n6. Validasi Klaim Retur Barang Cacat";
            cout << "\n7. Lihat Jurnal Riwayat Transaksi";
            cout << "\n8. Cari Berkas Transaksi (No Struk)";
            cout << "\n9. Statistik Grafik Penjualan Toko";
            cout << "\n0. Kembali ke Menu Utama Akses";
            cout << "\n========================================";
            cout << "\nPilih menu : ";
            cin >> pilih;

            switch (pilih) {
                case 1: 
                    kasir.inputTransaksi(); // Memproses belanja sekaligus bayar & cetak struk Alfamidi
                    break;
                case 2: 
                    kasir.voidItemKeranjang(); 
                    break;
                case 3: 
                    kasir.inputDiskon(); 
                    break;
                case 4: 
                    kasir.tampilKeranjang(); // Menampilkan isi keranjang belanja saat ini
                    break;
                case 5: 
                    kasir.inputSplitBill(); 
                    break;
                case 6: 
                    kasir.kelolaReturKasir(); // Fungsi murni tanpa vektor yang sudah kita buat sukses
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
    
    
    void menuGudang() {
        int pilihGudang;
        do {
            cout << "\n==================================================";
            cout << "\n           MENU STAFF INVENTARIS GUDANG";
            cout << "\n==================================================";
            cout << "\n1.  Input Barang Baru ke Katalog";
            cout << "\n2.  Hapus Barang dari Sistem Gudang";
            cout << "\n3.  Cari Barang (ID / SKU / Nama)";
            cout << "\n4.  Input Restock / Tambah Jumlah Stok";
            cout << "\n5.  Input Data Vendor / Supplier Baru";
            cout << "\n6.  Input Penyesuaian Harga Produk";
            cout << "\n7.  Urutan Data Barang / Sorting Katalog";
            cout << "\n8.  Tampilkan Statistik Barang Terlaris";
            cout << "\n9.  Tampilkan Stok Barang Saat Ini";
            cout << "\n10. Tampilkan Notifikasi Stok Menipis";
            cout << "\n11. Tampilkan Inventaris Global";
            cout << "\n12. Tampilkan Daftar Harga Jual Resmi";
            cout << "\n0.  Kembali ke Menu Utama Akses";
            cout << "\n==================================================";
            cout << "\nPilih menu gudang (0-12): ";
            cin >> pilihGudang;

            switch (pilihGudang) {
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
                case 6: 
                    gudang.inputPenyesuaianHarga(); 
                    break;
                case 7: 
                    gudang.urutkanBarang(); 
                    break;
                case 8: 
                    gudang.barangTerlaris(); 
                    break;
                case 9: 
                    gudang.tampilStok(); 
                    break;
                case 10: 
                    gudang.tampilNotifikasiStok(); 
                    break;
                case 11: 
                    gudang.tampilInventaris(); 
                    break;
                case 12: 
                    gudang.tampilDaftarHarga(); // Diselaraskan dari typo 'tampilDaftqarHarga'
                    break;
                case 0: 
                    cout << "\nKembali ke menu akses utama staff...\n"; 
                    break;
                default: 
                    cout << "? Pilihan tidak tersedia! Masukkan angka antara 0 hingga 12.\n";
                    break;
            }
        } while (pilihGudang != 0);
    }
    
    
};


    void menuUtama(){
    	Staff staff;

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
                    staff.menuKasir();
                    break;

                case 2:
                    staff.menuGudang();
                    break;

                case 0:
                    cout << "\nProgram selesai.\n";
                    break;

                default:
                    cout << "\nMenu tidak tersedia!\n";
            }

        }while(pilih != 0);
    
};


// ======================================================
// CLASS CUSTOMER : Johan Hanggara Hasiholan
// ======================================================

class Customer {
private:
    DataUtama databaseUser[100];
    ItemTransaksi keranjang[50];
	int jumlahKeranjang = 0;
	string idCustomerAktif = "";
    string namaCustomerAktif = "";

    // Fungsi pembantu untuk format mata uang Rupiah
    string formatRupiah(long long nominal) {
       stringstream ss;
       ss << "Rp " << nominal;
       return ss.str();
    }
    
    // Setter sesi setelah customer berhasil login
    void setSesiCustomer(string id, string nama) {
        this->idCustomerAktif = id;
        this->namaCustomerAktif = nama;
        this->jumlahKeranjang = 0; 
    }


    // Sub-fungsi Pembantu untuk Mencetak Struk Bukti Pengembalian Dana/Barang
    void cetakStrukNotaReturKasir(DataRetur r) {
        cout << "\n========================================";
        cout << "\n       SUPERMARKET TANDA TERIMA RETUR   ";
        cout << "\n========================================";
        cout << "\nID Bukti Retur: " << r.idRetur;
        cout << "\nNo Nota Asli  : " << r.idTransaksi;
        cout << "\nID Konsumen   : " << r.idCustomer;
        cout << "\nTanggal Proses: " << r.tanggal;
        cout << "\n----------------------------------------";
        cout << "\nSKU Barang    : " << r.skuBarang;
        cout << "\nJumlah Unit   : " << r.jumlah << " pcs";
        cout << "\nStatus Hasil  : [" << r.status << "]";
        cout << "\nCatatan Kasir : " << r.catatan_admin;
        cout << "\n========================================";
        cout << "\n      Dokumen Sah Sistem Kasir M-OOP    ";
        cout << "\n========================================\n";
    }

public:
	// Fungsi Registrasi Customer (Menyimpan data customer)
	// Banyak baris kode : 33 baris
    void registrasi() {
        DataUtama data;

        cout << "\n=== REGISTRASI CUSTOMER ===\n";
        // Generate ID otomatis
        data.idUser = ValidasiDanGenerate::generateIDCustomer();
        cout << "ID Customer Anda: " << data.idUser << endl;
        
		cin.ignore(1000, '\n'); // bersihkan buffer
        
        cout << "\nNama     : ";
        getline(cin, data.nama);   // gunakan getline
        
	      
        // validasi input username
    do{
    	cout << "\nUsername : ";
        getline(cin, data.username);
       
        	
			if(!ValidasiDanGenerate::validasiUsername(data.username))
        	{
				cout << "<X> Username harus minimal 5 karakter!\n";
			} else if(ValidasiDanGenerate::usernameSudahAda(data.username))
			{
				cout << "<X> Username sudah digunakan oleh orang lain\n";
			}

	} while(!ValidasiDanGenerate::validasiUsername(data.username) || ValidasiDanGenerate::usernameSudahAda(data.username));
          
	    
    	// validasi email
    do {
        cout << "Masukkan Email   : "; 
		getline(cin, data.email);
        
		if (ValidasiDanGenerate::emailSudahAda(data.email)) {
            cout << "? Email sudah terdaftar! Gunakan email lain.\n";
        }
    } while (ValidasiDanGenerate::emailSudahAda(data.email));


		// validasi no hp
    do {
        cout << "Masukkan No HP (08...)  : "; 
		getline(cin, data.noHp);
		
        if (!ValidasiDanGenerate::validasiNoHP(data.noHp)) {
            cout << "? Nomor HP tidak valid (Minimal 12 digit angka)!\n";
        }
    } while (!ValidasiDanGenerate::validasiNoHP(data.noHp));

        
       // 5. Validasi Perulangan Input Password & Konfirmasi
    bool passwordValid = false;
    do {
        cout << "Password : "; getline(cin, data.password);
        cout << "Ulangi Password: "; getline(cin, data.ulang);

        if (!ValidasiDanGenerate::validasiPassword(data.password)) {
            cout << "<X> Password harus minimal 8 karakter!\n";
            passwordValid = false;
        } else if (!ValidasiDanGenerate::konfirmasiPassword(data.password, data.ulang)) {
            cout << "<X> Maaf, password tidak cocok! Silakan ulangi.\n";
            passwordValid = false;
        } else {
            passwordValid = true;
        }
    } while (!passwordValid);

    // 6. PROSES SIMPAN FILE (Harus berada di dalam fungsi registrasi)
    ofstream fileSimpan("dataCustomer.txt", ios::app); // Saya ganti namanya menjadi fileSimpan agar aman
    if (fileSimpan.is_open()) {
        fileSimpan << data.idUser 	<< ";" 
                   << data.nama 	<< ";" 
                   << data.username << ";" 
                   << data.email 	<< ";" 
                   << data.noHp 	<< ";" 
                   << data.password << ";" 
                   << data.ulang << endl;
                   
        fileSimpan.close();
        cout << "? Data berhasil disimpan! Tanggal: " << ValidasiDanGenerate::generateTanggal() << endl;
    } else {
        cout << "Maaf, Data tidak tersimpan, silakan coba lagi!." << endl;
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
    bool Login(){
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
        		if(line.empty())
        			continue;
        			
        		stringstream ss(line);
    			DataUtama data;
        	
        		getline(ss, data.idUser, 		';');
        		getline(ss,data.nama, 		';');
        		getline(ss,data.username, 	';');
        		getline(ss,data.email, 		';');
        		getline(ss,data.noHp, 		';');
        		getline(ss,data.password, 	';');
        		getline(ss, data.ulang, 	';');
        	
        	 // Lakukan pengecekan akun
            if (data.username == usernameInput && data.password == passwordInput) {
                cout << "\n[OKE!] Login berhasil! Selamat datang, " << data.nama << endl;
                found = true;
                
                this -> idCustomerAktif = data.idUser;
                this -> namaCustomerAktif = data.nama;
                
                return true;
            }
        }
        inFile.close();
        
        if (!found) {
            cout << "[SALAH] Login gagal! Username atau password salah.\n";
        } 
    } else {
        cout << "[X] Gagal sistem! Berkas database customer tidak dapat ditemukan.\n";
    }
}
	

	// Fungsi Katalog Produk (Menampilkan daftar produk yang diperbarui)
	void KatalogProduk() {
	    cout << "\n================================================================================";
	    cout << "\n                                KATALOG PRODUK TOKO";
	    cout << "\n================================================================================\n";
	
	    // Header tabel 
	    cout << left << setw(10) << "ID" 
	         << setw(25) << "Nama Barang"
	         << setw(15) << "Jenis/Kategori"  
	         << setw(15) << "Harga" 
	         << setw(10) << "Stok" 
	         << "Terjual"
	         << endl;
	    cout << "--------------------------------------------------------------------------------\n";
	    
	    ifstream inFile("katalogProduk.txt");
	    
	    if (!inFile.is_open()) {
	        cout << "? File katalogProduk.txt gagal dibuka atau belum dibuat!" << endl;
	        return;
	    }
	    
	    string line;
	    
	    while (getline(inFile, line)) {
	        if (line.empty()) continue; // Perlindungan jika ada baris kosong di file teks
	        
	        stringstream ss(line);
	        Barang item;
	        
	        string hargaStr;
	        string stokStr;
	        string terjualStr; 
	        
	        // 1. Parsing data string dari file dipisahkan karakter ';'
	        getline(ss, item.id, ';');
	        getline(ss, item.nama, ';');
	        getline(ss, item.jenis, ';');
	        getline(ss, hargaStr, ';');
	        getline(ss, stokStr, ';');
	        getline(ss, terjualStr, ';'); // Mengambil data kolom terjual
	        
	        // 2. Konversi tipe data string ke numerik menggunakan sstream
	        stringstream(hargaStr) >> item.harga;
	        stringstream(stokStr) >> item.stok;
	        stringstream(terjualStr) >> item.terjual; // Konversi kolom terjual
	        
	        // 3. Tampilkan ke layar dengan susunan tabel rapi
	        cout << left 
	             << setw(10) << item.id 
	             << setw(25) << item.nama
	             << setw(15) << item.jenis 
	             << "Rp " << setw(12) << item.harga
	             << setw(10) << item.stok 
	             << item.terjual // Cetak total item terjual di ujung kanan
	             << endl;
	    }
	    
	    inFile.close();
	    cout << "================================================================================\n";
}
	

	// Fungsi Pencarian Produk (Mencari produk berdasarkan ID atau Nama)
	void pencarianProduk() {
	    string keyword;
	    
	    // PENTING: Bersihkan sisa buffer agar getline tidak terloncat otomatis
	    cin.ignore(1000, '\n'); 
	    
	    cout << "\n========================================";
	    cout << "\n           CARI PRODUK";
	    cout << "\n========================================";
	    
	    cout << "\nNama Produk / ID : ";
	    getline(cin, keyword);
	    
	    cout << "\n================================================================================\n";
	    cout << left 
			 << setw(10) << "ID" 
	         << setw(25) << "Nama Barang"
	         << setw(15) << "Jenis/Kategori"  
	         << setw(15) << "Harga" 
	         << setw(10) << "Stok" 
	         << "Terjual"
	         << endl;
	    cout << "--------------------------------------------------------------------------------\n";
	    
	    ifstream inFile("katalogProduk.txt");
	    string line;
	    bool found = false;
	    
	    if (!inFile.is_open()) {
	        cout << "? File katalogProduk.txt gagal dibuka!" << endl;
	        return;
	    }
	    
	    while (getline(inFile, line)) {
	        if (line.empty()) continue; // Perlindungan jika ada baris kosong
	        
	        stringstream ss(line);
	        Barang item;
	        
	        string hargaStr;
	        string stokStr;
	        string terjualStr; // Variabel penampung kolom ke-6
	        
	        // 1. Parsing 6 kolom data dipisahkan dengan ';' sesuai format terbaru
	        getline(ss, item.id, ';');
	        getline(ss, item.nama, ';');
	        getline(ss, item.jenis, ';');
	        getline(ss, hargaStr, ';');
	        getline(ss, stokStr, ';');
	        getline(ss, terjualStr, ';'); // Parsing kolom terjual
	        
	        // 2. Konversi string ke tipe data numerik masing-masing
	        stringstream(hargaStr) >> item.harga;
	        stringstream(stokStr) >> item.stok;
	        stringstream(terjualStr) >> item.terjual;
	        
	        // 3. Logika Pencarian: Berdasarkan ID persis ATAU mengandung kata kunci pada Nama Barang
	        // Menggunakan string::npos membuat pencarian kata kunci parsial (tidak harus sama persis 100%) menjadi aktif
	        if (item.id == keyword || item.nama.find(keyword) != string::npos) {
	            cout << left 
	                 << setw(10) << item.id 
	                 << setw(25) << item.nama
	                 << setw(15) << item.jenis 
	                 << "Rp " << setw(12) << item.harga
	                 << setw(10) << item.stok 
	                 << item.terjual
	                 << endl;
	            found = true;
	        }
	    }
	    
	    inFile.close();
	    cout << "================================================================================\n";
	    
	    if (!found) {
	        cout << "\n? Produk dengan kata kunci '" << keyword << "' tidak ditemukan!\n";
	    }
	}

	
	//==================================
	// Fungsi Tambah Keranjang
	//==================================
	
	// Fungsi Tambah Keranjang Belanja Baru Berbasis File Handling
	void tambahKeranjang() {
	    string idCari;
	    int qtyBeli;
	
	    cout << "\n========================================";
	    cout << "\n         TAMBAH KE KERANJANG";
	    cout << "\n========================================";
	
	    // Batasan kapasitas array keranjang sesuai deklarasi awal Anda (maksimal 50 item)
	    if (jumlahKeranjang >= 50) {
	        cout << "\n? Gagal! Keranjang belanja Anda sudah penuh.\n";
	        return;
	    }
	
	    cout << "\nMasukkan ID / SKU Barang: ";
	    cin >> idCari;
	
	    // 1. MEMBACA DATA REAL-TIME DARI FILE KATALOG GUDANG
	    ifstream inFile("katalogProduk.txt");
	    if (!inFile.is_open()) {
	        cout << "? Sistem Error: File katalogProduk.txt tidak dapat ditemukan!\n";
	        return;
	    }
	
	    string line;
	    bool ditemukan = false;
	    Barang itemGudang; // Menggunakan struct Barang terbaru Anda
	
	    while (getline(inFile, line)) {
	        if (line.empty()) continue;
	        stringstream ss(line);
	        string hargaStr, stokStr, terjualStr;
	
	        // Parsing 6 kolom data dipisahkan dengan karakter ';'
	        getline(ss, itemGudang.id, ';');
	        getline(ss, itemGudang.nama, ';');
	        getline(ss, itemGudang.jenis, ';');
	        getline(ss, hargaStr, ';');
	        getline(ss, stokStr, ';');
	        getline(ss, terjualStr, ';');
	
	        stringstream(hargaStr) >> itemGudang.harga;
	        stringstream(stokStr) >> itemGudang.stok;
	        stringstream(terjualStr) >> itemGudang.terjual;
	
	        if (itemGudang.id == idCari) {
	            ditemukan = true;
	            break; // Keluar dari loop pencarian file jika ID cocok
	        }
	    }
	    inFile.close();
	
	    // 2. PROSES VALIDASI STOK DAN KUANTITAS
	    if (ditemukan) {
	        cout << "Nama Barang   : " << itemGudang.nama << endl;
	        cout << "Harga Barang  : Rp " << itemGudang.harga << endl;
	        cout << "Stok Gudang   : " << itemGudang.stok << endl;
	        cout << "----------------------------------------\n";
	        cout << "Masukkan Jumlah Beli: ";
	        cin >> qtyBeli;
	
	        if (qtyBeli <= 0) {
	            cout << "\n? Jumlah pembelian tidak valid!\n";
	            return;
	        }
	
	        if (qtyBeli > itemGudang.stok) {
	            cout << "\n? Gagal! Stok di gudang tidak mencukupi (Sisa stok: " << itemGudang.stok << ").\n";
	            return;
	        }
	
	        // 3. LOGIKA UPDATE QUANTITY (Jika barang yang sama sudah ada di keranjang)
	        for (int i = 0; i < jumlahKeranjang; i++) {
	            if (keranjang[i].nama == itemGudang.nama) {
	                // Pastikan akumulasi jumlah baru tidak menjebol sisa stok gudang
	                if (keranjang[i].qty + qtyBeli > itemGudang.stok) {
	                    cout << "\n? Gagal! Akumulasi di keranjang melampaui sisa stok gudang.\n";
	                    return;
	                }
	                keranjang[i].qty += qtyBeli;
	                keranjang[i].subtotal = keranjang[i].harga * keranjang[i].qty;
	                cout << "\n? Berhasil memperbarui jumlah item di keranjang belanja!\n";
	                return;
	            }
	        }
	
	        // 4. MEMASUKKAN BARANG BARU KE SLOT ARRAY KERANJANG SEMENTARA
	        ItemTransaksi itemBaru;
	        itemBaru.nama = itemGudang.nama;
	        itemBaru.harga = itemGudang.harga;
	        itemBaru.qty = qtyBeli;
	        itemBaru.subtotal = itemGudang.harga * qtyBeli;
	
	        keranjang[jumlahKeranjang] = itemBaru;
	        jumlahKeranjang++;
	
	        cout << "\n? Berhasil! Barang sukses ditambahkan ke keranjang belanja.\n";
	
	    } else {
	        cout << "\n? Maaf, barang dengan ID '" << idCari << "' tidak ditemukan!\n";
	    }
	}


	// =========================
	// Fungsi Lihat Keranjang
	// =========================
	// Fungsi Lihat Keranjang Belanja (Lengkap dengan Fitur Edit & Hapus Item)
	void lihatKeranjang() {
	    cout << "\n========================================================";
	    cout << "\n                   KERANJANG BELANJA";
	    cout << "\n========================================================";
	
	    if (jumlahKeranjang == 0) {
	        cout << "\n?? Keranjang belanja Anda masih kosong.\n";
	        cout << "========================================================\n";
	        return;
	    }
	
	    long long total = 0;
	
	    // Menyesuaikan lebar header tabel agar pas dengan data di bawahnya
	    cout << "\n" << left    
	         << setw(5)  << "No"
	         << setw(25) << "Nama Barang"
	         << setw(10) << "Qty"
	         << setw(15) << "Harga"
	         << "Subtotal"
	         << endl;
	
	    cout << "----------------------------------------------------------------------\n";
	
	    // LOOP MENAMPILKAN DATA KERANJANG
	    for (int i = 0; i < jumlahKeranjang; i++) {
	        cout << left
	             << setw(5)  << i + 1
	             << setw(25) << keranjang[i].nama
	             << setw(10) << keranjang[i].qty
	             << "Rp " << setw(12) << formatRupiah(keranjang[i].harga)
	             << "Rp " << formatRupiah(keranjang[i].subtotal)
	             << endl;
	
	        total += keranjang[i].subtotal;
	    }
	
	    cout << "----------------------------------------------------------------------";
	    cout << "\nTotal Belanja : Rp " << formatRupiah(total);
	    cout << "\n========================================================" << endl;
	
	    // =========================================================================
	    // PENGEMBANGAN FITUR AKSES MANIPULASI ISI KERANJANG
	    // =========================================================================
	    int pilihanSub;
	    cout << "Opsi Keranjang:\n";
	    cout << "1. Ubah Kuantitas Item Belanja\n";
	    cout << "2. Hapus Salah Satu Item Barang\n";
	    cout << "3. Lanjut ke Pembayaran (Checkout)\n";
	    cout << "4. Kembali ke Menu Utama Customer\n";
	    cout << "Pilih opsi (1-4): ";
	    cin >> pilihanSub;
	
	    if (pilihanSub == 1) { // ?? FITUR UBAH KUANTITAS
	        int nomorUrut, qtyBaru;
	        cout << "Masukkan Nomor Urut barang yang ingin diubah: ";
	        cin >> nomorUrut;
	
	        // Validasi nomor urut sesuai daftar tabel di layar
	        if (nomorUrut > 0 && nomorUrut <= jumlahKeranjang) {
	            int index = nomorUrut - 1;
	            cout << "Barang Terpilih: " << keranjang[index].nama << " (Qty saat ini: " << keranjang[index].qty << ")\n";
	            cout << "Masukkan Jumlah Kuantitas Baru: ";
	            cin >> qtyBaru;
	
	            if (qtyBaru <= 0) {
	                cout << "? Kuantitas tidak valid! Gunakan menu hapus jika ingin membatalkan.\n";
	                return;
	            }
	
	            // Catatan: Anda bisa menambahkan validasi cek stok file katalogProduk.txt di sini jika diperlukan
	            keranjang[index].qty = qtyBaru;
	            keranjang[index].subtotal = keranjang[index].harga * qtyBaru;
	            cout << "? Kuantitas barang berhasil diperbarui!\n";
	        } else {
	            cout << "? Nomor urut tidak terdaftar di keranjang!\n";
	        }
	    } 
	    else if (pilihanSub == 2) { // ?? FITUR HAPUS ITEM (DELETE ARRAY ELEMENT)
	        int nomorUrut;
	        cout << "Masukkan Nomor Urut barang yang ingin dihapus: ";
	        cin >> nomorUrut;
	
	        if (nomorUrut > 0 && nomorUrut <= jumlahKeranjang) {
	            int indexHapus = nomorUrut - 1;
	            string namaTerhapus = keranjang[indexHapus].nama;
	
	            // Logika menggeser elemen array ke kiri untuk menghapus slot memory
	            for (int i = indexHapus; i < jumlahKeranjang - 1; i++) {
	                keranjang[i] = keranjang[i + 1];
	            }
	            jumlahKeranjang--; // Kurangi counter total jenis barang di keranjang
	
	            cout << "? Barang '" << namaTerhapus << "' berhasil dikeluarkan dari keranjang!\n";
	        } else {
	            cout << "? Nomor urut tidak terdaftar di keranjang!\n";
	        }
	    }
	    else if (pilihanSub == 3) { // ?? LANJUT CHECKOUT
	        // Panggil fungsi checkout milik Anda
	        // prosesCheckout(); atau checkout();
	        cout << "\nPengalihan sistem menuju antrean checkout...\n";
	    }
	}
	
	
	
	// ===========================
	// Fungsi Chekout Produk
	//============================
	
	// Fungsi Checkout Produk Customer Tingkat Lanjut (Murni File Handling - TANPA VECTOR)
	void checkoutCustomer(string idCustomerSesi) {
	    if (jumlahKeranjang == 0) {
	        cout << "\n? Gagal! Keranjang belanja Anda masih kosong.\n";
	        return;
	    }
	
	    long long subtotal = 0;
	    int totalKuantitasItem = 0;
	
	    // 1. HITUNG TOTAL BELANJA DARI ARRAY KERANJANG
	    for (int i = 0; i < jumlahKeranjang; i++) {
	        subtotal += keranjang[i].subtotal;
	        totalKuantitasItem += keranjang[i].qty;
	    }
	
	    cout << "\n========================================";
	    cout << "\n         PROSES CHECKOUT BELANJA";
	    cout << "\n========================================";
	    cout << "\nTotal Belanja Bersih : Rp " << formatRupiah(subtotal);
	    cout << "\nTotal Kuantitas Item : " << totalKuantitasItem << " unit";
	    cout << "\n========================================\n";
	
	    cout << "Apakah Anda yakin ingin menyelesaikan pesanan ini? (y/n): ";
	    char konfirmasi;
	    cin >> konfirmasi;
	
	    if (konfirmasi != 'y' && konfirmasi != 'Y') {
	        cout << "\n? Proses checkout dibatalkan.\n";
	        return;
	    }
	
	    // 2. GENERATE ID TRANSAKSI & TANGGAL OTOMATIS VIA UTILITY
	    string idStrukBaru = ValidasiDanGenerate::generateIDOtomatis("TRX", "transaksi.txt");
	    string tanggalHariIni = ValidasiDanGenerate::generateTanggal();
	
	    // 3. LOGIKA HUBUNGAN GUDANG: MEMOTONG STOK REAL-TIME (STREAM SWITCHING - TANPA VECTOR)
	    ifstream fileBarangIn("katalogProduk.txt");
	    ofstream fileBarangOut("tempKatalog.txt"); // Membuat file penampung sementara
	    
	    if (fileBarangIn.is_open() && fileBarangOut.is_open()) {
	        string line;
	        while (getline(fileBarangIn, line)) {
	            if (line.empty()) continue;
	            stringstream ss(line);
	            Barang b;
	            string hStr, sStr, tStr;
	
	            getline(ss, b.id, ';');
	            getline(ss, b.nama, ';');
	            getline(ss, b.jenis, ';');
	            getline(ss, hStr, ';');
	            getline(ss, sStr, ';'); 
	            getline(ss, tStr, ';'); 
	
				// konversi tipe data
				stringstream(hStr) >> b.harga;
				stringstream(sStr) >> b.stok;
				stringstream(tStr) >> b.terjual;
				
	            // COCOKKAN DAN POTONG STOK JIKA BARANG DIBELI OLEH CUSTOMER
	            for (int i = 0; i < jumlahKeranjang; i++) {
	                if (keranjang[i].nama == b.nama) {
	                    b.stok -= keranjang[i].qty;       // Mengurangi stok barang asli
	                    b.terjual += keranjang[i].qty;    // Menambah statistik produk terjual toko
	                }
	            }
	            
	            // LANGSUNG TULIS KE FILE TEMPORARY (Tanpa ditampung ke vector RAM)
	            fileBarangOut << b.id << ";" << b.nama << ";" << b.jenis << ";" 
	                          << b.harga << ";" << b.stok << ";" << b.terjual << endl;
	        }
	        fileBarangIn.close();
	        fileBarangOut.close();
	
	        // Proses penimpaan file database utama gudang via harddisk murni
	        remove("katalogProduk.txt");
	        rename("tempKatalog.txt", "katalogProduk.txt");
	    } else {
	        cout << "? Error Fatal: Gagal menyambungkan data ke sistem stok gudang!\n";
	        return;
	    }
	
	    // 4. MENCATAT NOTA BARU KE BERKAS REKAM JEJAK "transaksi.txt"
	    ofstream fileTrxOut("transaksi.txt", ios::app);
	    if (fileTrxOut.is_open()) {
	        fileTrxOut << idStrukBaru << ";" 
	                   << idCustomerSesi << ";" 
	                   << subtotal << ";" 
	                   << 0 << ";"               
	                   << 0.0 << ";"             
	                   << subtotal << ";"        
	                   << "Belum_Lunas" << ";"   
	                   << totalKuantitasItem << ";" 
	                   << tanggalHariIni << endl;
	        fileTrxOut.close();
	    }
	
	    // 5. MENCATAT ITEM RINCIAN DETIL KE BERKAS "detailTransaksi.txt"
	    ofstream fileDetailOut("detailTransaksi.txt", ios::app);
	    if (fileDetailOut.is_open()) {
	        for (int i = 0; i < jumlahKeranjang; i++) {
	            fileDetailOut << idStrukBaru << ";" 
	                          << keranjang[i].nama << ";" 
	                          << keranjang[i].qty << ";" 
	                          << keranjang[i].subtotal << endl;
	        }
	        fileDetailOut.close();
	    }
	
	    cout << "\n? [ CHECKOUT BERHASIL ]";
	    cout << "\nID Nomor Struk Anda : " << idStrukBaru;
	    cout << "\nTanggal Transaksi   : " << tanggalHariIni;
	    cout << "\n\n? Silakan bawa ID Struk Anda ke meja Kasir untuk melakukan pembayaran!\n";
	    cout << "========================================\n";
	
	    // Kosongkan kembali seluruh isi array keranjang setelah sukses dibukukan
	    jumlahKeranjang = 0;
	}


	// Riwayat Transaksi
	// Fungsi Menampilkan Riwayat Transaksi Khusus Customer yang Sedang Login
	void riwayatTransaksi(string idCustomerSesi) {
	    cout << "\n=========================================================================================";
	    cout << "\n                                 RIWAYAT TRANSAKSI ANDA";
	    cout << "\n=========================================================================================\n";
	
	    // Header tabel dibuat lengkap dan rapi sesuai struktur struct RiwayatTransaksi
	    cout << left << setw(12) << "No Struk" 
	         << setw(15) << "Tanggal"
	         << setw(15) << "Subtotal"  
	         << setw(12) << "Diskon" 
	         << setw(15) << "Status Kasir" 
	         << "Total Bayar" 
	         << endl;
	    cout << "-----------------------------------------------------------------------------------------\n";
	
	    // Membaca file database transaksi pusat
	    ifstream inFile("transaksi.txt");
	    
	    if (!inFile.is_open()) {
	        cout << "                               [INFO] Belum ada riwayat transaksi.                       \n";
	        cout << "=========================================================================================\n";
	        return;
	    }
	    
	    string line;
	    bool found = false;
	    long long akumulasiBelanjaCustomer = 0; // Tambahan statistik pengeluaran pribadi customer
	
	    while (getline(inFile, line)) {
	        if (line.empty()) continue; // Perlindungan jika ada baris kosong
	        
	        stringstream ss(line);
	        RiwayatTransaksi trx; // Menggunakan struct RiwayatTransaksi Anda
	        
	        string subtotalStr, diskonStr, ongkirStr, totalStr, itemStr;
	        
	        // 1. Parsing seluruh kolom data transaksi dipisahkan karakter ';'
	        getline(ss, trx.idTransaksi, ';');
	        getline(ss, trx.idCustomer,  ';');
	        getline(ss, subtotalStr,     ';');
	        getline(ss, diskonStr,       ';');
	        getline(ss, ongkirStr,       ';');
	        getline(ss, totalStr,        ';');
	        getline(ss, trx.metode,      ';'); // Kolom status/metode pembayaran
	        getline(ss, itemStr,         ';');
	        getline(ss, trx.tanggal,     ';');
	
	        // 2. Konversi tipe data string ke numerik
	        stringstream(subtotalStr) >> trx.subtotal;
	        stringstream(diskonStr)   >> trx.diskon;
	        stringstream(totalStr)    >> trx.totalBelanja;
	
	        // 3. SINKRONISASI HAK AKSES: Hanya tampilkan jika ID Customer di file cocok dengan user aktif
	        if (trx.idCustomer == idCustomerSesi) {
	            cout << left 
	                 << setw(12) << trx.idTransaksi 
	                 << setw(15) << trx.tanggal
	                 << "Rp " << setw(11) << trx.subtotal
	                 << "Rp " << setw(9)  << trx.diskon
	                 << setw(15) << trx.metode // Menampilkan apakah statusnya "Belum_Lunas" atau metode bayar asli
	                 << "Rp " << trx.totalBelanja
	                 << endl;
	            
	            // Hitung akumulasi belanja jika statusnya sudah divalidasi kasir (bukan Belum_Lunas)
	            if (trx.metode != "Belum_Lunas") {
	                akumulasiBelanjaCustomer += trx.totalBelanja;
	            }
	            found = true;
	        }
	    }
	    inFile.close();
	    
	    cout << "-----------------------------------------------------------------------------------------\n";
	    if (found) {
	        cout << "TOTAL PENGELUARAN BELANJA VALID ANDA: Rp " << formatRupiah(akumulasiBelanjaCustomer) << endl;
	    } else {
	        cout << "                                 Anda belum pernah berbelanja.                           \n";
	    }
	    cout << "=========================================================================================\n";
	}

	// Fungsi retur barang
	void ajukanReturCustomer() {
		string idReturOtomatis = ValidasiDanGenerate::generateIDOtomatis("RTR", "dataRetur.txt");
		
        cout << "\n========================================";
        cout << "\n         PENGAJUAN RETUR BARANG";
        cout << "\n========================================\n";
        // tampilkan no retur otomatis
        cout << "KODE RETUR ANDA: " << idReturOtomatis;
        cout << "\n========================================\n";

        // Menggunakan variabel pengganti non-rancu sesuai instruksi Anda
        DataRetur data; 
        data.idRetur = idReturOtomatis;
        data.idCustomer = idCustomerAktif;
        

        cout << "Masukkan Nomor Struk (ID Transaksi): "; 
		cin >> data.idTransaksi;
		
        cout << "Masukkan Jumlah Unit Diretur       : "; 
		cin >> data.jumlah;
        
        cin.ignore(1000, '\n'); // Bersihkan buffer input agar getline tidak terloncat
        cout << "Masukkan Alasan Detail Kerusakan   : "; getline(cin, data.alasan);

        // Pengisian field otomatis sebelum divalidasi kasir fisik toko
        data.tanggal = ValidasiDanGenerate::generateTanggal();
        data.status = "PENDING"; 
        data.catatan_admin = "-";

        // Tulis berkas ajuan langsung ke database teks pusat "dataRetur.txt"
        ofstream fileOut("dataRetur.txt", ios::app);
        if (fileOut.is_open()) {
            fileOut << data.idRetur 	<< ";" 
                    << data.idTransaksi << ";" 
                    << data.skuBarang 	<< ";" 
                    << data.idCustomer 	<< ";" 
                    << data.jumlah 		<< ";" 
                    << data.alasan 		<< ";" 
                    << data.tanggal 	<< ";" 
                    << data.status 		<< ";" 
                    << data.catatan_admin << endl;
            fileOut.close();
            
            cout << "\n? Sukses! Kode Pengajuan Retur Anda: " << data.idRetur << endl;
            cout << "Silakan bawa fisik barang beserta struk ke kasir toko untuk divalidasi.\n";
        } else {
            cout << "? Gagal! Berkas database retur tidak merespons.\n";
        }
    }
    
    void menuCustomer() {
    int pilihan = 0;
    do {
        cout << "\n========================================";
        cout << "\n          DASHBOARD CUSTOMER";
        cout << "\n========================================";
        cout << "\n1. Jelajahi Katalog Produk Toko";
        cout << "\n2. Cari Produk / Barang";
        cout << "\n3. Masukkan Item Ke Keranjang belanja";
        cout << "\n4. Lihat & Kelola Isi Keranjang (Edit/Hapus)";
        cout << "\n5. Lakukan Checkout Pembelian";
        cout << "\n6. Ajukan Klaim Retur Barang";
        cout << "\n7. Logout (Kembali ke menu utama";
        cout << "\nPilih Menu (1-7): ";
        cin >> pilihan;

        switch(pilihan) {
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
				break; // Kirim ID Sesi aktif
            case 5: 
				checkoutCustomer(idCustomerAktif);  
				break;
			case 6:
				ajukanReturCustomer();
				break;
            case 7: 
				cout << "\nLogout sukses! Kembali ke gerbang utama...\n"; 
				break;
            default: 
				cout << "<X> Pilihan tidak valid!\n";
        }
    } while (pilihan != 7); // ?? Mengunci program di sini sampai user menekan angka 6
}
    

   // Fungsi Menu Utama Customer (Navigasi login dan registrasi)
  // Banyak baris kode : 36 baris
  void menuUtama(){ 
  Customer cust; 
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
            	if(cust.Login())
				{
            		cust.menuCustomer();
            	}
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


//=====================================================
// MENU UTAMA
//=====================================================

class MenuUtama {
public:
	void menuSistem(
		Admin &adminSistem, 
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
                int subPilihStaff;
	            cout << "\n========================================";
	            cout << "\n            GERBANG AKSES STAFF";
	            cout << "\n========================================";
	            cout << "\n1. Masuk Sebagai Staff Kasir (POS)";
	            cout << "\n2. Masuk Sebagai Staff Gudang (Logistik)";
	            cout << "\nPilih Sub-Role Staff: ";
	            cin >> subPilihStaff;
	
	            if (subPilihStaff == 1) {
	                staff.menuKasir(); 
	            } else if (subPilihStaff == 2) {
	                staff.menuGudang(); 
	            } else {
	                cout << " Pilihan sub-role staff tidak tersedia!\n";
	            }
	            break;

            case 3:
            	customer.menuUtama();
                break;

			case 4:
				cout << "\nProgram Selesai.\n";
				break;
            default:
                cout << "\nMenu tidak tersedia!\n";
                break;
                
            	}
            }	while(pilih != 4);  
            
        }

};


//=====================================================
//	MAIN
//=====================================================
int main() {

    Admin adminSistem;

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
