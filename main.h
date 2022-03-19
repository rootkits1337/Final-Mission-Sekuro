#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <memory>

using vect2 = std::vector<std::vector<int>>;
using vect = std::vector<int>;
static const int default_map_length = 21;
static const int default_map_width = 21;
static const double default_robot_hp = 100;
static const double default_robot_dmg = 10;
static const double default_robot_range = 10;
static const double default_kurama_hp = 50;
static const double default_kurama_dmg = 8;
static const double default_kurama_range = 5;

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
	int m_length;
	int m_width;
	int m_robot_x;
	int m_robot_y;
	int m_kurama_x;
	int m_kurama_y;

public:
	/*
	* Inisiasi Map, set semua value di map menjadi 0,
	* set koordinat robot (0,0) dengan mengisi koordinat (0,0) pada robot menjadi 1
	* set koordinat kurama secara random dan assign nilai pada koordinat tersebut pada robot menjadi -1
	*/
	Map() {
		m_length = default_map_length;
		m_width = default_map_width;
		m_map.resize(m_length, vect(m_width, 0));
		this->set(0, 0, 1);
		// robot selalu mulai di koordinat 0, 0
		m_robot_x = 0;
		m_robot_y = 0;
		random_kurama();
	}
	void random_kurama() {
		// set posisi kurama secara random
		do {
			m_kurama_x = rand() % m_length;
			m_kurama_y = rand() % m_width;
		} while (m_kurama_x == m_robot_x && m_kurama_y == m_robot_y); // jangan sampai mecha-kurama spawn di tempat yang sama dengan robot
		this->set(m_kurama_x, m_kurama_y, -1);
		cout << "Kurama di spawn pada koordinat (" << m_kurama_x << ", "
			<< m_kurama_y << ")" << endl;
	}
	// tampilkan sesuai koordinat kartesius
	void print_map() {
		for (size_t i = 0; i < m_length; ++i) {
			for (size_t j = 0; j < m_width; ++j) {
				cout << m_map[m_length - i - 1][j] << " ";
			}
			cout << endl;
		}
		cout << "Ket: 1 adalah robot dan -1 adalah Mecha-Kurama" << endl;
	}
	// set nilai pada koordinat x, y
	void set(size_t x, size_t y, int val) {
		m_map[y][x] = val;
	}
	// mengembalikan nilai pada koordinat x, y
	int get(size_t x, size_t y) {
		return m_map[y][x];
	}
	// mengembalikan nilai pada koordinat x robot
	int get_robot_x() {
		return m_robot_x;
	}
	// mengembalikan nilai pada koordinat y robot
	int get_robot_y() {
		return m_robot_y;
	}
	// mengembalikan nilai pada koordinat x kurama
	int get_kurama_x() {
		return m_kurama_x;
	}
	// mengembalikan nilai pada koordinat y kurama
	int get_kurama_y() {
		return m_kurama_y;
	}
	~Map() = default;
};

class Character {
private:
	double m_health;
	double m_dmg;
	double m_att_range;
public:
	// set health dan dmg pada karakter
	Character(double health, double dmg, double range)
		: m_health(health), m_dmg(dmg), m_att_range(range)
	{

	}
	// mengembalikan hp karakter sekarang
	int get_health() {
		return m_health;
	}
	// mengembalikan damage yang dapat dihasilkan
	int get_dmg() {
		return m_dmg;
	}
	double get_range() {
		return m_att_range;
	}
	// set hp karakter menjadi new_health
	void set_health(double new_health) {
		m_health = new_health;
	}
	~Character() = default;
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
	int m_kill;
	bool is_robot_act; // cek robot sudah bergerak atau berhasil menyerang
public:
	WarState();
	bool robot_keatas();
	bool robot_kebawah();
	bool robot_kekiri();
	bool robot_kekanan();
	int jarak_x(); // selisih x antara robot dengan dengan
	int jarak_y(); // selisih y antara robot dengan kurama
	double hitung_jarak();
	bool serang_kurama(); // robot menyerang kurama
	void serang_robot(); // kurama menyerang robot
	bool isgameover(); // periksa apakah perang sudah berakhir
	void print_map();
	void main_loop();
	void kurama_turn();
	~WarState() = default;
};