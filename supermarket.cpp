#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdio>
#include <vector>
using namespace std;

// Bagian Admin
struct Karyawan {
    string id;
    string nama;
    string role;
    string shift;
};


struct DataUtama {
    string nama;
    string email;
    string noHp;      
    string username;
    string password;
    string ulang;
};

struct Katalog{
	int id;
	string namaProduk;
	double harga;
	int stok;
	string jenisBarang;
};



class MenuAdmin {
private:
    string namaAdmin;
    Karyawan daftarKaryawan[100]; 
    int jumlahKaryawan = 0;       

public:
    MenuAdmin(string nama) {
        namaAdmin = nama;
    }

    void registrasiKaryawan() {
        cout << "\n=== [1] REGISTRASI KARYAWAN ===" << endl;
        
        if (jumlahKaryawan >= 100) {
            cout << "Kapasitas data penuh!" << endl;
            return;
        }

        cout << "ID Karyawan     : ";
        cin >> daftarKaryawan[jumlahKaryawan].id;
        cout << "Nama (1 Kata)   : ";
        cin >> daftarKaryawan[jumlahKaryawan].nama;
        cout << "Posisi/Role     : ";
        cin >> daftarKaryawan[jumlahKaryawan].role;
        
        daftarKaryawan[jumlahKaryawan].shift = "Belum diatur";
        jumlahKaryawan++;
        
        cout << "Data berhasil ditambahkan!" << endl;
    }

    void aturShiftKaryawan() {
        cout << "\n=== [2] ATUR SHIFT KARYAWAN ===" << endl;
        
        if (jumlahKaryawan == 0) {
            cout << "Data karyawan kosong." << endl;
            return;
        }

        for (int i = 0; i < jumlahKaryawan; i++) {
            cout << i + 1 << ". " << daftarKaryawan[i].nama << " | Shift: " << daftarKaryawan[i].shift << endl;
        }

        int pilihan;
        cout << "\nPilih nomor karyawan : ";
        cin >> pilihan;

        if (pilihan > 0 && pilihan <= jumlahKaryawan) {
            int index = pilihan - 1;
            int shiftPil;
            
            cout << "Pilih Shift (1=Pagi, 2=Siang, 3=Malam) : ";
            cin >> shiftPil;

            if (shiftPil == 1) daftarKaryawan[index].shift = "Pagi";
            else if (shiftPil == 2) daftarKaryawan[index].shift = "Siang";
            else if (shiftPil == 3) daftarKaryawan[index].shift = "Malam";
            else cout << "Pilihan tidak valid." << endl;
            
            cout << "Shift " << daftarKaryawan[index].nama << " diubah menjadi " << daftarKaryawan[index].shift << endl;
        } else {
            cout << "Nomor tidak ditemukan." << endl;
        }
    }

    void kelolaDiskonPromo() {
        cout << "\n=== [3] KELOLA DISKON ===" << endl;
    }

    void tampilkanMenu() {
        int pilihan;
        bool isLogin = true;

        while (isLogin) {
            cout << "\n--- MENU ADMIN ---" << endl;
            cout << "1. Registrasi Karyawan" << endl;
            cout << "2. Atur Shift Karyawan" << endl;
            cout << "3. Kelola Diskon" << endl;
            cout << "8. Logout" << endl;
            cout << "Pilih Menu : ";
            cin >> pilihan;

            switch (pilihan) {
                case 1: 
					registrasiKaryawan(); 
					break;
                case 2: 
					aturShiftKaryawan(); 
					break;
                case 3: 
					kelolaDiskonPromo(); 
					break;
                	case 8: isLogin = false; 
					break;
                default: 
					cout << "Pilihan salah." << endl; 
					break;
            }
        }
    }
};



void bersihkanInput(){

    cin.clear();

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
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

    for(int i = s.size() - 1; i >= 0; i--){

        hasil = s[i] + hasil;

        hitung++;

        if(hitung % 3 == 0 && i != 0){

            hasil = "." + hasil;
        }
    }

    return hasil;
}

// ======================================================
// STRUCT
// ======================================================

struct Barang{

    string id;
    string nama;

    long long harga;

    int stok;
};

struct Supplier{

    string nama;
    string alamat;
    string telepon;
};

struct ItemTransaksi{

    string nama;

    long long harga;

    int qty;

    long long subtotal;
};

struct RiwayatTransaksi{

    string noStruk;

    long long subtotal;

    long long diskon;

    long long total;

    string metode;

    int jumlahItem;
};

struct DataRetur{

    string noStruk;

    string namaBarang;

    string alasan;

    string status;
};

// ======================================================
// DATABASE
// ======================================================

vector<Barang> databaseBarang;

vector<Supplier> databaseSupplier;

vector<RiwayatTransaksi> databaseTransaksi;

vector<DataRetur> databaseRetur;

// ======================================================
// CLASS KASIR
// ======================================================

class Kasir{

public:

    vector<ItemTransaksi> keranjang;

    long long subtotal;

    long long nominalDiskon;

    string kodeDiskonAktif;

    Kasir(){

        subtotal = 0;

        nominalDiskon = 0;

        kodeDiskonAktif = "-";
    }

    void inputTransaksi(){

        keranjang.clear();

        subtotal = 0;

        nominalDiskon = 0;

        kodeDiskonAktif = "-";

        cout << "\n========================================";
        cout << "\n           INPUT TRANSAKSI";
        cout << "\n========================================\n";

        char lanjut = 'y';

        while(lanjut == 'y' || lanjut == 'Y'){

            ItemTransaksi item;

            bersihkanInput();

            cout << "\nNama Barang   : ";
            getline(cin, item.nama);

            cout << "Harga Barang  : Rp";
            cin >> item.harga;

            cout << "Jumlah Barang : ";
            cin >> item.qty;

            item.subtotal =
                item.harga * item.qty;

            keranjang.push_back(item);

            subtotal += item.subtotal;

            cout << "\n[ TAMBAH ] "
                 << item.nama
                 << " x"
                 << item.qty
                 << " = Rp"
                 << formatRupiah(item.subtotal);

            cout << "\nTambah barang lain? (y/n): ";
            cin >> lanjut;
        }

        cout << "\n========================================";

        cout << "\nSubtotal sementara : Rp"
             << formatRupiah(subtotal);

        cout << "\n========================================\n";
    }

    void inputDiskon(){

        cout << "\n========================================";
        cout << "\n             INPUT DISKON";
        cout << "\n========================================\n";

        if(keranjang.empty()){

            cout << "Belum ada transaksi berjalan.\n";
            return;
        }

        string kode;

        cout << "Kode Diskon : ";
        cin >> kode;

        int persen = 0;

        if(kode == "DISKON10"){

            persen = 10;
        }
        else if(kode == "DISKON20"){

            persen = 20;
        }
        else if(kode == "MEMBER5"){

            persen = 5;
        }
        else{

            persen = -1;
        }

        if(persen == -1){

            cout << "\nKode diskon tidak valid.\n";

            nominalDiskon = 0;

            kodeDiskonAktif = "-";
        }
        else{

            kodeDiskonAktif = kode;

            nominalDiskon =
                subtotal * persen / 100;

            cout << "\nDiskon berhasil diterapkan.";

            cout << "\nNominal Diskon : Rp"
                 << formatRupiah(nominalDiskon);
        }

        cout << "\n========================================\n";
    }

    void inputPembayaran(){

        if(keranjang.empty()){

            cout << "\nBelum ada transaksi berjalan.\n";
            return;
        }

        long long total =
            subtotal - nominalDiskon;

        cout << "\n========================================";
        cout << "\n          INPUT PEMBAYARAN";
        cout << "\n========================================\n";

        cout << "Total Belanja : Rp"
             << formatRupiah(total);

        string metode;

        cout << "\nMetode Pembayaran : ";
        cin >> metode;

        long long bayar;

        cout << "Nominal Bayar : Rp";
        cin >> bayar;

        if(bayar < total){

            cout << "\nPembayaran kurang!\n";
            return;
        }

        long long kembalian =
            bayar - total;

        int noInt =
            ambilNomorBaru();

        string noStruk = "TRX";

        if(noInt < 10){

            noStruk += "00" + intToStr(noInt);
        }
        else if(noInt < 100){

            noStruk += "0" + intToStr(noInt);
        }
        else{

            noStruk += intToStr(noInt);
        }

        tampilStruk(noStruk, metode, total);

        RiwayatTransaksi trx;

        trx.noStruk = noStruk;

        trx.subtotal = subtotal;

        trx.diskon = nominalDiskon;

        trx.total = total;

        trx.metode = metode;

        trx.jumlahItem =
            keranjang.size();

        databaseTransaksi.push_back(trx);

        cout << "\nKembalian : Rp"
             << formatRupiah(kembalian);

        cout << "\n========================================\n";

        keranjang.clear();

        subtotal = 0;

        nominalDiskon = 0;

        kodeDiskonAktif = "-";
    }

    void inputSplitBill(){

        cout << "\n========================================";
        cout << "\n             SPLIT BILL";
        cout << "\n========================================\n";

        long long totalBelanja;

        int jumlahOrang;

        cout << "Total Belanja : Rp";
        cin >> totalBelanja;

        cout << "Jumlah Orang : ";
        cin >> jumlahOrang;

        if(jumlahOrang <= 0){

            cout << "\nJumlah orang tidak valid.\n";
            return;
        }

        long long perOrang =
            totalBelanja / jumlahOrang;

        cout << "\nSetiap orang membayar : Rp"
             << formatRupiah(perOrang);

        cout << "\n========================================\n";
    }

    void inputRetur(){

        cout << "\n========================================";
        cout << "\n            RETUR BARANG";
        cout << "\n========================================\n";

        string noStruk;

        string namaBarang;

        string alasan;

        cout << "Nomor Struk : ";
        cin >> noStruk;

        bersihkanInput();

        cout << "Nama Barang : ";
        getline(cin, namaBarang);

        cout << "Alasan : ";
        getline(cin, alasan);

        DataRetur retur;

        retur.noStruk = noStruk;

        retur.namaBarang = namaBarang;

        retur.alasan = alasan;

        retur.status = "Disetujui";

        databaseRetur.push_back(retur);

        cout << "\nRetur berhasil disimpan.";

        cout << "\n========================================\n";
    }

    void riwayatTransaksi(){

        cout << "\n========================================";
        cout << "\n         RIWAYAT TRANSAKSI";
        cout << "\n========================================\n";

        if(databaseTransaksi.empty()){

            cout << "Belum ada transaksi.\n";
            return;
        }

        for(int i = 0;
            i < databaseTransaksi.size();
            i++){

            RiwayatTransaksi &trx =
                databaseTransaksi[i];

            cout << left
                 << setw(10)
                 << trx.noStruk

                 << "Rp"
                 << setw(12)
                 << formatRupiah(trx.total)

                 << setw(10)
                 << trx.metode

                 << trx.jumlahItem
                 << " item\n";
        }

        cout << "========================================\n";
    }

    void laporanPenjualan(){

        cout << "\n========================================";
        cout << "\n         LAPORAN PENJUALAN";
        cout << "\n========================================\n";

        long long total = 0;

        for(int i = 0;
            i < databaseTransaksi.size();
            i++){

            cout << databaseTransaksi[i].noStruk
                 << " | Rp"
                 << formatRupiah(
                        databaseTransaksi[i].total
                    )
                 << endl;

            total +=
                databaseTransaksi[i].total;
        }

        cout << "\nTotal Penjualan : Rp"
             << formatRupiah(total);

        cout << "\n========================================\n";
    }

    void laporanKeuangan(){

        cout << "\n========================================";
        cout << "\n         LAPORAN KEUANGAN";
        cout << "\n========================================\n";

        long long pendapatan = 0;

        for(int i = 0;
            i < databaseTransaksi.size();
            i++){

            pendapatan +=
                databaseTransaksi[i].total;
        }

        long long pengeluaran = 1500000;

        long long laba =
            pendapatan - pengeluaran;

        cout << "Pendapatan : Rp"
             << formatRupiah(pendapatan);

        cout << "\nPengeluaran : Rp"
             << formatRupiah(pengeluaran);

        cout << "\nLaba Bersih : Rp"
             << formatRupiah(laba);

        cout << "\n========================================\n";
    }

    void tampilStruk(
        string noStruk,
        string metode,
        long long total
    ){

        cout << "\n========================================";
        cout << "\n     ALFAMIDI KI AGENG PEMANAHAN";
        cout << "\n========================================";

        cout << "\nNo Struk : "
             << noStruk;

        cout << "\nKasir : Mukhamma\n";

        for(int i = 0;
            i < keranjang.size();
            i++){

            ItemTransaksi &item =
                keranjang[i];

            cout << "\n"
                 << item.nama
                 << " x"
                 << item.qty
                 << " = Rp"
                 << formatRupiah(item.subtotal);
        }

        cout << "\n----------------------------------------";

        cout << "\nTotal : Rp"
             << formatRupiah(total);

        cout << "\nPembayaran : "
             << metode;

        cout << "\n========================================\n";
    }
};

// ======================================================
// CLASS GUDANG
// ======================================================

class Gudang{

public:

    int cariIndex(string id){

        for(int i = 0;
            i < databaseBarang.size();
            i++){

            if(databaseBarang[i].id == id){

                return i;
            }
        }

        return -1;
    }

    void inputBarangBaru(){

        Barang b;

        cout << "\n========================================";
        cout << "\n        INPUT BARANG BARU";
        cout << "\n========================================\n";

        cout << "ID Barang : ";
        cin >> b.id;

        if(cariIndex(b.id) != -1){

            cout << "\nID sudah digunakan!\n";
            return;
        }

        bersihkanInput();

        cout << "Nama Barang : ";
        getline(cin, b.nama);

        cout << "Harga : Rp";
        cin >> b.harga;

        cout << "Stok : ";
        cin >> b.stok;

        databaseBarang.push_back(b);

        cout << "\nBarang berhasil ditambahkan.";

        cout << "\n========================================\n";
    }

    void tampilStok(){

        cout << "\n========================================";
        cout << "\n            DETAIL STOK";
        cout << "\n========================================\n";

        if(databaseBarang.empty()){

            cout << "Belum ada barang.\n";
        }
        else{

            for(int i = 0;
                i < databaseBarang.size();
                i++){

                cout << databaseBarang[i].id
                     << " | "
                     << databaseBarang[i].nama
                     << " | Rp"
                     << formatRupiah(
                            databaseBarang[i].harga
                        )
                     << " | "
                     << databaseBarang[i].stok
                     << endl;
            }
        }

        cout << "========================================\n";
    }

    void inputRestock(){}
    void inputSupplier(){}
    void inputPenyesuaianHarga(){}
    void tampilNotifikasiStok(){}
    void tampilInventaris(){}
    void tampilDaftarHarga(){}
};

// ======================================================
// CLASS STAFF
// ======================================================

class Staff{

private:

    Kasir kasir;

    Gudang gudang;

public:

    void menuKasir(){

        int pilih;

        do{

            cout << "\n========== MENU KASIR ==========";
            cout << "\n1. Input Transaksi";
            cout << "\n2. Input Diskon";
            cout << "\n3. Input Pembayaran";
            cout << "\n4. Split Bill";
            cout << "\n5. Retur Barang";
            cout << "\n6. Riwayat Transaksi";
            cout << "\n7. Laporan Penjualan";
            cout << "\n8. Laporan Keuangan";
            cout << "\n0. Kembali";
            cout << "\n================================";
            cout << "\nPilih menu : ";

            cin >> pilih;

            switch(pilih){

                case 1:
                    kasir.inputTransaksi();
                    break;

                case 2:
                    kasir.inputDiskon();
                    break;

                case 3:
                    kasir.inputPembayaran();
                    break;

                case 4:
                    kasir.inputSplitBill();
                    break;

                case 5:
                    kasir.inputRetur();
                    break;

                case 6:
                    kasir.riwayatTransaksi();
                    break;

                case 7:
                    kasir.laporanPenjualan();
                    break;

                case 8:
                    kasir.laporanKeuangan();
                    break;

                case 0:
                    cout << "\nKembali...\n";
                    break;

                default:
                    cout << "\nMenu tidak tersedia!\n";
            }

        }while(pilih != 0);
    }

    void menuGudang(){

        int pilih;

        do{

            cout << "\n========== MENU GUDANG ==========";
            cout << "\n1. Input Barang";
            cout << "\n2. Tampil Stok";
            cout << "\n0. Kembali";
            cout << "\n=================================";
            cout << "\nPilih menu : ";

            cin >> pilih;

            switch(pilih){

                case 1:
                    gudang.inputBarangBaru();
                    break;

                case 2:
                    gudang.tampilStok();
                    break;

                case 0:
                    cout << "\nKembali...\n";
                    break;

                default:
                    cout << "\nMenu tidak tersedia!\n";
            }

        }while(pilih != 0);
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



class Customer {
private:
    DataUtama databaseUser[100];

public:
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
            outFile << data.nama << "," << data.email << "," << data.noHp << "," << data.username << "," << data.password << endl;
            outFile.close();
            cout << "Data berhasil disimpan!" << endl;
        } else {
            cout << "Maaf, Data tidak tersimpan, silahkan coba lagi!." << endl;
        }
    }

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
        	
        		getline(ss,data.nama, ',');
        		getline(ss,data.email, ',');
        		getline(ss,data.noHp, ',');
        		getline(ss,data.username, ',');
        		getline(ss,data.password, ',');
        	
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
	
	void KatalogProduk(){
		Katalog item;
		cout << "\n========================================";
    	cout << "\n          KATALOG PRODUK";
    	cout << "\n========================================\n";

    	cout << left << setw(5) << "ID" 
     		 << setw(20) << "Nama Barang" 
     		 << setw(10) << "Harga" 
     		 << setw(10) << "Stok" 
     		 << setw(15) << "Jenis" << endl;
    	cout <<"---------------------------------------------------------\n";
    	
    	ifstream inFile("katalogProduk.txt");
    	string line;
    	
    	if (!inFile.is_open()){
    		cout << "File katalog.txt tidak dapat dibuka!" << endl;
    		return;
		}
		
		while (getline(inFile, line)) {
			stringstream ss(line);
			Katalog item;
			string idStr, hargaStr, stokStr;
			
			// Parsing sesuai format id, namaProduk, jenisBarang, harga, stok
			getline(ss, idStr, ',');
			stringstream(idStr) >> item.id;
			
			getline(ss, item.namaProduk, ',');
			getline(ss, item.jenisBarang, ',');
			
			getline(ss, hargaStr, ',');
			stringstream(hargaStr) >> item.harga;
			
			getline(ss, stokStr, ',');
			stringstream(stokStr) >> item.stok;
			
		cout << left << setw(5) << item.id 
         	 << setw(20) << item.namaProduk 
         	 << setw(10) << item.harga 
        	 << setw(10) << item.stok 
         	 << setw(15) << item.jenisBarang << endl;
		
		}
	inFile.close();
	}	
	
	void pencarianProduk(){
		string keyword;
		cout << "\n=== CARI PRODUK ===";
        cout << "\nNama Produk / ID : ";
        getline(cin, keyword);
        cout << "=======================";
        
        ifstream inFile("katalogProduk.txt");
		string line;
		bool found = false;
		
		if(!inFile.is_open()){
			cout << "File katalogProduk.txt tidak dapat dibuka!" << endl;
			return;
		}
		
		while(getline(inFile, line)){
			stringstream ss(line);
			Katalog item;
			string idStr, hargaStr, stokStr;
			
// Parsing sesuai format id, namaProduk, jenisBarang, harga, stok
			getline(ss, idStr, ',');
			stringstream(idStr) >> item.id;
			
			getline(ss, item.namaProduk, ',');
			getline(ss, item.jenisBarang, ',');
			
			getline(ss, hargaStr, ',');
			stringstream(hargaStr) >> item.harga;
			
			getline(ss, stokStr, ',');
			stringstream(stokStr) >> item.stok;
			
			// Pencarian Berdasarkan ID or Nama
			int idInput;
			stringstream(keyword) >> idInput; 
			if(item.id == idInput ){
				cout << "\nProduk ditemukan:\n";
				cout << left << setw(5) << item.id
					 << setw(20) << item.namaProduk
					 << setw(10) << item.harga
					 << setw(10) << item.stok
					 << setw(15) << item.jenisBarang << endl;
				found = true;
			}
		}
		
		if(!found){
			cout << "\nProduk tidak ditemukan!" << endl;
		}
        
        
	}
	
	 // ================= MENU CUSTOMER =================
    void menuCustomer() {

        int pilih;

        do {

            cout << "\n========================================";
            cout << "\n             MENU CUSTOMER";
            cout << "\n========================================";

            cout << "\n1. Lihat Katalog";
            cout << "\n2. Cari Produk";
            cout << "\n3. Keranjang Belanja";
            cout << "\n4. Checkout";
            cout << "\n5. Riwayat Pembelian";
            cout << "\n6. Retur Barang";
            cout << "\n7. Logout";

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
                    cout << "\nKeranjang Belanja\n";
                    break;

                case 4:
                    cout << "\nCheckout\n";
                    break;

                case 5:
                    cout << "\nRiwayat Pembelian\n";
                    break;

                case 6:
                    cout << "\nRetur Barang\n";
                    break;

                case 7:
                    cout << "\nLogout berhasil!\n";
                    break;

                default:
                    cout << "\nMenu tidak tersedia!\n";
            }

        } while (pilih != 7);
    }
    
    
  void menuUtama(Customer &customer){  
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
                customer.registrasi();
                break;

            case 2:
                customer.Login();
                break;

            case 3:
                cout << "Program selesai." << endl;
                break;

            default:
                cout << "Menu tidak tersedia!" << endl;
        }

    } while(pilih != 3);
}



	// Menu Utama DAri 3 Hak Akses; Admin, Staff, Customer
	void menuSistem(MenuAdmin &adminSistem, Customer &customer){

    int pilih;

    do{

        cout << "\n========================================";
        cout << "\n         SISTEM APLIKASI SUPERMARKET";
        cout << "\n========================================";

        cout << "\n1. Menu Admin";
        cout << "\n2. Menu Customer";
        cout << "\n3. Keluar";

        cout << "\n========================================";
        cout << "\nPilih Menu : ";
        cin >> pilih;
        cin.ignore();

        switch(pilih){

            case 1:
                adminSistem.tampilkanMenu();
                break;

            case 2:
                customer.menuUtama(customer);
                break;

            case 3:
                cout << "\nProgram selesai.\n";
                break;

            default:
                cout << "\nMenu tidak tersedia!\n";
        }

    } while(pilih != 3);
}
};


int main() {

    MenuAdmin adminSistem("Admin Pusat");

    Customer customer;

    customer.menuSistem(adminSistem, customer);

    return 0;
}
