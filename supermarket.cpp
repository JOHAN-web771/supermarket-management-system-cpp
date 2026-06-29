#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
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
