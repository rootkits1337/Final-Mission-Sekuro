#include "main.h"

WarState::WarState() {
		m_map = make_shared<Map>();
		m_robot_x = m_map->get_robot_x();
		m_robot_y = m_map->get_robot_y();
		m_kurama_x = m_map->get_kurama_x();
		m_kurama_y = m_map->get_kurama_y();

		m_robot = make_shared<Character>(100, 10); // set health = 100, dmg = 10
		m_kurama = make_shared<Character>(50, 8); // set health = 50, dmg = 8
}

void WarState::robot_keatas() {
	if (m_robot_y == 20) {
		cout << "Error: Robot tidak dapat keluar dari peta" << endl;
		return;
	}
	m_map->set(m_robot_x, m_robot_y, 0);
	m_map->set(m_robot_x, m_robot_y + 1, 1);
	cout << "Robot bergerak dari (" << m_robot_x << "," << m_robot_y << ")"
		<< " ke " << "(" << m_robot_x << ", " << m_robot_y + 1 << ")" << endl;
	++m_robot_y;
}

void WarState::robot_kebawah() {

}

void WarState::robot_kekanan() {

}

void WarState::robot_kekiri() {

}

int WarState::jarak_x() {
	return 0;
}

int WarState::jarak_y() {
	return 0;
}

double WarState::hitung_jarak() {
	return 0;
}

int WarState::isgameover() {
	// periksa apakah perang sudah berakhir.
	return 0;
}

void WarState::serang_kurama() {

}

void WarState::serang_robot() {

}

void WarState::main_loop() {
	int option;
	do {
		cout << "Pilih Aksi" << endl;
		cout << "0. Keluar" << endl;
		cout << "1. Tampilkan peta" << endl;
		cout << "2. Robot ke atas" << endl;
		cout << "3. Robot ke bawah" << endl;
		cout << "4. Robot ke kanan" << endl;
		cout << "5. Robot ke kiri" << endl;
		cin >> option;
		switch (option)
		{
		case 1:
			print_map();
			break;
		case 2:
			robot_keatas();
			break;
		case 0: 
			return;
		default:
			if (isgameover()) {
				cout << "Perang sudah berakhir" << endl;
				// tampilkan informasi akhir
				return;
			}
		}
		
	} while (option != 0);
}


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