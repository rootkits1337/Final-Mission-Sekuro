#include "main.h"

/*
*	Inisiasi state perang
* Generate Peta
* Simpan koordinat robot dan kurama sekarang
*/
WarState::WarState() {
	m_map = make_shared<Map>();
	m_robot_x = m_map->get_robot_x();
	m_robot_y = m_map->get_robot_y();
	m_kurama_x = m_map->get_kurama_x();
	m_kurama_y = m_map->get_kurama_y();

	m_robot = make_shared<Character>(
		default_robot_hp, default_robot_dmg, default_robot_range
	); // set health = 100, dmg = 10
	m_kurama = make_shared<Character>(
		default_kurama_hp, default_kurama_dmg, default_kurama_range
	); // set health = 50, dmg = 8
	m_kill = 0;
	is_robot_act = 0;
}

bool WarState::robot_keatas() {
	// robot bergerak ke atas
	// lakukan boundary checking
	if (m_robot_y == 20) {
		cout << "Error: Robot tidak dapat keluar dari peta" << endl;
		return false;
	}
	if (m_robot_x == m_kurama_x && m_robot_y + 1 == m_kurama_y) {
		cout << "Error: Robot tidak dapat menabrak kurama" << endl;
		return false;
	}
	m_map->set(m_robot_x, m_robot_y, 0);
	m_map->set(m_robot_x, m_robot_y + 1, 1);
	cout << "Robot bergerak dari (" << m_robot_x << "," << m_robot_y << ")"
		<< " ke " << "(" << m_robot_x << ", " << m_robot_y + 1 << ")" << endl;
	++m_robot_y;
	return true;
}

bool WarState::robot_kebawah() {
	// robot bergerak ke bawah
	// lakukan boundary checking
	if (m_robot_y == 0){
		cout<< "Error: Robot tidak dapat keluar dari peta" << endl;
		return false;
	}
	if (m_robot_x == m_kurama_x && m_robot_y - 1 == m_kurama_y) {
		cout << "Error: Robot tidak dapat menabrak kurama" << endl;
		return false;
	}
	m_map->set(m_robot_x, m_robot_y, 0);
	m_map->set(m_robot_x, m_robot_y - 1, 1);
	cout << "Robot bergerak dari (" << m_robot_x << "," << m_robot_y << ")"
		<< " ke " << "(" << m_robot_x << ", " << m_robot_y - 1 << ")" << endl;
	--m_robot_y;
	return true;
}

bool WarState::robot_kekanan() {
	// robot bergerak ke kanan
	// lakukan boundary checking
	if (m_robot_x == 20){
		cout<< "Error: Robot tidak dapat keluar dari peta" << endl;
		return false;
	}
	if (m_robot_x + 1 == m_kurama_x && m_robot_y == m_kurama_y) {
		cout << "Error: Robot tidak dapat menabrak kurama" << endl;
		return false;
	}
	m_map->set(m_robot_x, m_robot_y, 0);
	m_map->set(m_robot_x + 1, m_robot_y, 1);
	cout << "Robot bergerak dari (" << m_robot_x << "," << m_robot_y << ")"
		<< " ke " << "(" << m_robot_x + 1<< ", " << m_robot_y << ")" << endl;
	++m_robot_x;
	return true;
}

bool WarState::robot_kekiri() {
	// robot bergerak ke kiri
	// lakukan boundary checking
	if (m_robot_x == 0){
		cout<< "Error: Robot tidak dapat keluar dari peta" << endl;
		return false;
	}
	if (m_robot_x - 1 == m_kurama_x && m_robot_y == m_kurama_y) {
		cout << "Error: Robot tidak dapat menabrak kurama" << endl;
		return false;
	}
	m_map->set(m_robot_x, m_robot_y, 0);
	m_map->set(m_robot_x - 1, m_robot_y, 1);
	cout << "Robot bergerak dari (" << m_robot_x << "," << m_robot_y << ")"
		<< " ke " << "(" << m_robot_x - 1<< ", " << m_robot_y << ")" << endl;
	--m_robot_x;
	return false;
}

int WarState::jarak_x() {
	// mengembalikan jarak x antara robot dan kurama
	return (abs(m_robot_x - m_kurama_x));
}

int WarState::jarak_y() {
	// mengembalikan jarak y antara robot dan kurama
	return (abs(m_robot_y - m_kurama_y));
}

double WarState::hitung_jarak() {
	// mengembalikan jarak antara robot dan kurama (euclidean ? manhattan ? chebyshev ?)
	// digunakan jarak euclidean
	return (sqrt(pow(jarak_x(),2)+pow(jarak_y(), 2)));
}

bool WarState::isgameover() {
	// periksa apakah perang sudah berakhir.
	Character robot = *m_robot.get();
	if(robot.get_health() <= 0){
		return true;
	}
	return false;
}

bool WarState::serang_kurama() {
	// robot menyerang kurama
	// hp kurama berkurang sesuai dengan besarnya serangan robot
	// keluarkan pesar error bila kurama di luar jangkauan serang robot
	double jarak = hitung_jarak();
	if (jarak > m_robot->get_range()) {
		cout << "Kurama di luar jangkauan!\n";
		return false;
	}
	double old_kurama_hp = m_kurama->get_health();
	double robot_dmg = m_robot->get_dmg();
	m_kurama->set_health(old_kurama_hp - robot_dmg);
	cout << "Menyerang kurama dengan " << robot_dmg << " damage" << endl;
	cout << "HP Kurama saat ini: " << m_kurama->get_health() << endl << endl;
	return true;
}

void WarState::serang_robot() {
	// kurama menyerang robot
	// hp robot berkurang sesuai serangan kurama
	// karena serangan kurama otomatis, maka tidak perlu dilakukan pengecekan jarak di sini
	// ketika fungsi serang_robot dipanggil, maka robot sudah pasti ada di jarak serang kurama
	double old_robot_hp = m_robot->get_health();
	double kurama_dmg = m_kurama->get_dmg();
	m_robot->set_health(old_robot_hp - kurama_dmg);
	cout << "Kurama menyerang robot dengan " << kurama_dmg << " damage" << endl;
	cout << "HP Robot saat ini menjadi: " << m_robot->get_health() << endl << endl;
}

// AI Kurama
void WarState::kurama_turn() {
	// Memilih langkah terbaik bagi kurama
	// Algoritma
	// 1. Periksa apakah robot sudah berada pada jarak serang kurama
	// 2. Jika sudah berada pada range, serang robot (panggil fungsi serang_robot)
	// 3. Jika belum berada pada range, pilih langkah terbaik untuk mendekati robot
	// Langkah terbaik didapatkan dengan membandingkan posisi (x,y) kurama dan robot

	double jarak = hitung_jarak();
	if (jarak <= m_kurama->get_range()) {
		serang_robot();
		return;
	}
	// gerak ke arah yg jaraknya paling jauh
	if (jarak_x() < jarak_y()) {
		// gerak searah sumbu y
		if (m_kurama_y < m_robot_y) {
			// kurama gerak ke atas
			m_map->set(m_kurama_x, m_kurama_y, 0);
			m_map->set(m_kurama_x, m_kurama_y + 1, -1);
			cout << "Kurama bergerak dari (" << m_kurama_x << "," << m_kurama_y << ")"
				<< " ke " << "(" << m_kurama_x << ", " << m_kurama_y + 1 << ")" << endl;
			++m_kurama_y;
		}
		else {
			// kurama gerak ke bawah
			m_map->set(m_kurama_x, m_kurama_y, 0);
			m_map->set(m_kurama_x, m_kurama_y - 1, -1);
			cout << "Kurama bergerak dari (" << m_kurama_x << "," << m_kurama_y << ")"
				<< " ke " << "(" << m_kurama_x << ", " << m_kurama_y - 1 << ")" << endl;
			--m_kurama_y;
		}
	}
	else {
		// gerak searah sumbu x
		if (m_kurama_x < m_robot_x) {
			// kurama gerak ke kanan
			m_map->set(m_kurama_x, m_kurama_y, 0);
			m_map->set(m_kurama_x + 1, m_kurama_y, -1);
			cout << "Kurama bergerak dari (" << m_kurama_x << "," << m_kurama_y << ")"
				<< " ke " << "(" << m_kurama_x + 1 << ", " << m_kurama_y << ")" << endl;
			++m_kurama_x;
		}
		else {
			// kurama gerak ke kiri
			m_map->set(m_kurama_x, m_kurama_y, 0);
			m_map->set(m_kurama_x - 1, m_kurama_y, -1);
			cout << "Kurama bergerak dari (" << m_kurama_x << "," << m_kurama_y << ")"
				<< " ke " << "(" << m_kurama_x - 1<< ", " << m_kurama_y << ")" << endl;
			--m_kurama_x;
		}
	}
}

void WarState::main_loop() {
	int option;
	do {
		// periksa apakah robot telah mati
		if (isgameover()) {
			cout << "Robot telah mati!" << endl;
			cout << "Anda telah membunuh " << m_kill << " ekor kurama" << endl;
			return;
		}
		cout << "Pilih Aksi" << endl;
		cout << "0. Keluar" << endl;
		cout << "1. Tampilkan peta dan jarak" << endl;
		cout << "2. Robot ke atas" << endl;
		cout << "3. Robot ke bawah" << endl;
		cout << "4. Robot ke kanan" << endl;
		cout << "5. Robot ke kiri" << endl;
		cout << "6. Serang Kurama" << endl;
		cout << "\nAksi yang dipilih: ";
		cin >> option;
		switch (option)
		{
		case 0:
			cout << "Anda telah membunuh " << m_kill << " ekor kurama" << endl;
			return;
		case 1:
			print_map();
			cout << "Jarak antara robot dan mecha-kurama adalah " << hitung_jarak() << endl;
			break;
		case 2:
			is_robot_act = robot_keatas();
			break;
		case 3:
			is_robot_act = robot_kebawah();
			break;
		case 4:
			is_robot_act = robot_kekanan();
			break;
		case 5:
			is_robot_act = robot_kekiri();
			break;
		case 6:
			is_robot_act = serang_kurama();
			double kurama_hp = m_kurama->get_health();
			if (kurama_hp <= 0) {
				// karena kurama mati, maka old kurama di delete dan buat objek kurama baru
				// note: sudah terdelete otomatis oleh smart pointer
				cout << "Kurama Mati!" << endl;
				m_map->set(m_kurama_x, m_kurama_y, 0); // set nilai pada koordinat kurama terakhir dengan 0
				m_kurama = make_shared<Character>(
					default_kurama_hp, default_kurama_dmg, default_kurama_range
					);
				m_map->random_kurama();
				m_kurama_x = m_map->get_kurama_x();
				m_kurama_y = m_map->get_kurama_y();
				++m_kill;
			}
			break;
		}
		// kurama akan memilih langkah terbaik untuk menghadapi robot
		if (is_robot_act) {
			// jika robot sudah bergerak atau berhasil menyerang, maka kurama juga akan bergerak atau menyerang
			kurama_turn();
			is_robot_act = 0;
		}
	} while (option != 0);
}

// menuliskan peta sesuai koordinat kartesius
void WarState::print_map() {
	m_map->print_map();
}

int main() {
	srand((unsigned int)time(NULL)); // seed
	int ctr = 0;
	WarState war_state;
	war_state.main_loop();
	return 0;
}
