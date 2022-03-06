#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <memory>

using vect2 = std::vector<std::vector<int>>;
using vect = std::vector<int>;
static const size_t length = 21;
static const size_t width = 21;

using std::cin;
using std::cout;
using std::endl;
using std::srand;
using std::time;
using std::shared_ptr;
using std::make_shared;

class Map {
private:
	vect2 m_map;
	int m_robot_x;
	int m_robot_y;
	int m_kurama_x;
	int m_kurama_y;

public:
	Map() {
		m_map.resize(length, vect(width, 0));
		// set posisi kurama secara random
		do {
			m_kurama_x = rand() % length;
			m_kurama_y = rand() % width;
		} while (m_kurama_x == 0 && m_kurama_y == 0); // jangan sampai mecha-kurama spawn di tempat yang sama dengan robot
		this->set(m_kurama_x, m_kurama_y, -1);
		this->set(0, 0, 1);
		// robot selalu mulai di koordinat 0, 0
		m_robot_x = 0;
		m_robot_y = 0;
	}
	// tampilkan sesuai koordinat kartesius
	void print_map() {
		for (size_t i = 0; i < length; ++i) {
			for (size_t j = 0; j < width; ++j) {
				cout << m_map[length - i - 1][j] << " ";
			}
			cout << endl;
		}
		cout << "Ket: 1 adalah robot dan -1 adalah Mecha-Kurama" << endl;
	}
	void set(size_t x, size_t y, int val) {
		m_map[y][x] = val;
	}
	int get(size_t x, size_t y) {
		return m_map[y][x];
	}
	int get_robot_x() {
		return m_robot_x;
	}
	int get_robot_y() {
		return m_robot_y;
	}
	int get_kurama_x() {
		return m_kurama_x;
	}
	int get_kurama_y() {
		return m_kurama_y;
	}
};

class Character {
private:
	int m_health;
	int m_dmg;
public:
	Character(int health, int dmg) {
		m_health = health;
		m_dmg = dmg;
	}
	int get_health() {
		return m_health;
	}
	int get_dmg() {
		return m_dmg;
	}
	void set_health(int new_health) {
		m_health = new_health;
	}
};

class WarState {
private:
	shared_ptr<Map> m_map;
	shared_ptr<Character> m_robot;
	shared_ptr<Character> m_kurama;
	int m_robot_x;
	int m_robot_y;
	int m_kurama_x;
	int m_kurama_y;
public:
	WarState();
	void robot_keatas();
	void robot_kebawah();
	void robot_kekiri();
	void robot_kekanan();
	int jarak_x(); // selisih x antara robot dengan dengan
	int jarak_y(); // selisih y antara robot dengan kurama
	double hitung_jarak();
	void serang_kurama(); // robot menyerang kurama
	void serang_robot(); // kurama menyerang robot
	int isgameover(); // periksa apakah perang sudah berakhir
	void print_map();
	void main_loop();
};