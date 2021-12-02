#pragma once
#include <iostream>
#include <time.h>

using namespace sf;
using namespace std;

const int window_x = 1000;
const int window_y = 1000;
const Event zaglushka;//заглушка для функции движения
class Object {
	// protected, чтобы не изменять координаты извне, а только функцией move
protected:
	Vector2f cords;//координаты
	float speed; //скорость
	Vector2f hitbox; //размер хитбокса от левого верхнего угла
public:
	virtual void move(Event event = zaglushka) = 0;//абстрактная функция для движения
	virtual Vector2f get_cords() { //геттер , который возвращает вектор координат
		return cords;
	}
	virtual Vector2f get_hitbox() { //геттер , который возвращает вектор хитбокса
		return hitbox;
	}
	virtual void object(RenderWindow& window) = 0;//создание объекта и его отрисовка
};
class Bullet :public Object {
public:
	Bullet() {
		static int counter = -1;//статическая перменная, чтобы записывать id каждой пули
		counter++;
		if (counter > 50-1) {
			counter -= 51-1;
		}
		//cout << counter << endl;
		id = counter;
	}
	virtual int get_id() {//геттер id
		return id;
	}
	virtual void hit() {
		cords = Vector2f(2000, 2000);//при попадании пуля уносится за пределы поля
	}
	virtual bool get_side() {//геттер стороны
		return side;
	}
	virtual void set_direction(float x,float y) {//задает направление пули
		direction = Vector2f(x, y);
	}
protected:
	Vector2f direction;//направление движения пули. задается единичной окружностью 
	int id;
	bool side=false;//характеристика пули добро/зло
	
};
class Bullet_character :public Bullet {
public:
	Bullet_character(float x = window_x / 2, float y = 3 * window_y / 4)
	{
		direction = Vector2f(0, 1);
		side = true;
		speed = 30;
		hitbox = Vector2f(10, 10);
		cords = Vector2f(x, y);
	}
	void move(Event event = zaglushka)
	{
		if (cords.y > 0) {//останавливвет пулю за пределом экрана
			int y = cords.y;
			cords = Vector2f(cords.x + speed * direction.x, y - speed * direction.y);
		}
		else {
			hit();
		}

	}
	void object(RenderWindow& window) {
		fig.setRadius(5);
		fig.setFillColor(Color(102, 255, 0));
		fig.setPosition(cords);
		window.draw(fig);
	}
private:
	CircleShape fig;//фигура для предварительной модели игры 
};
class Bullet_enemy :public Bullet {
public:	
	Bullet_enemy(float x = window_x / 2, float y = 3 * window_y / 4)
	{
		direction = Vector2f(0, -1);
		//direction = Vector2f(0.25, 1);
		speed = 8;
		hitbox = Vector2f(10, 10);
		cords = Vector2f(x, y);
	}
	void move(Event event = zaglushka)
	{
		if (cords.y < window_x) {
			int y = cords.y;
			cords = Vector2f(cords.x +speed*direction.x, y - speed*direction.y);
		}
		else {
			hit();
		}
		//cout << cords.y << endl;

	}
	void object(RenderWindow& window) {
		fig.setRadius(5);
		fig.setFillColor(Color(255, 220, 100));
		fig.setPosition(cords);
		window.draw(fig);
	}
private:
	CircleShape fig;//фигура для предварительной модели игры 
};
class Сharacter : public Object { // класс персонажей(враг/гг)
protected:
	int hp; //здоровье
public:
	virtual int get_hp() { //геттер здоровья
		return hp;
	}
	virtual void hit() {
		hp -= 1;
	}
	virtual void shoot(Bullet*clip[]) = 0; //виртуальная функция выстрела с указателем на массив указателей пуль
};
class Main_Сharacter :public Сharacter {
public:
	Main_Сharacter(float x = window_x / 2, float y = 3 * window_y / 4) {//с возможностью задать начальные координаты
		hp = 3;
		hitbox = Vector2f(90, 90);
		cords = Vector2f(x, y);//начальная координата
		speed = 5;
	}
	void object(RenderWindow& window) {
		Texture texture;
		texture.loadFromFile("images/hero.png");
		fig.setTexture(&texture);
		fig.setRadius(45);
		//fig.setFillColor(Color(0, 255, 0));
		fig.setPointCount(6);
		fig.setPosition(cords);
		//fig.scale(1.0f, 0.5f);
		window.draw(fig);
	}
	void shoot(Bullet* clip[])
	{
		Bullet* r = new Bullet_character(cords.x + hitbox.x / 2, cords.y + 20);//спавн пули посередине хитбокса и чуть выше 
		clip[r->get_id()] = r;
	};
	void move(Event event = zaglushka) 
	{ 
		switch (event.key.code)
		{
		case 71: {
			if (cords.x > 0) {
				cords = Vector2f(cords.x - speed, cords.y);
			}
			break;
		}
		case 72: {
			if (cords.x+hitbox.x <window_x) {
				cords = Vector2f(cords.x + speed, cords.y);
			}
			break;
		}
		case 73: {
			if (cords.y > 0) {
				cords = Vector2f(cords.x , cords.y -speed);
			}
			break;
		}
		case 74: {
			if (cords.y+hitbox.y < window_y) {
				cords = Vector2f(cords.x , cords.y + speed);
			}
			break;
		}
		default:
			break;
		}

	};
	
private:
	CircleShape fig;//фигура для предварительной модели игры 
};
class Enemy : public Сharacter {
private:
	float dir_x;
	float dir_y;
	
protected:
	int exp;//кол-во очков за врага
	Vector2f size_move_area;//размер области движения
	Vector2f move_area_x;//координаты области движения по х(мин,макс)
	Vector2f move_area_y;//координаты области движения по y (мин,макс)
public:
	Enemy() {
		speed = 3;
		dir_x= speed * pow(-1, rand() % 2);
		dir_y= speed * pow(-1, rand() % 2);
	}
	virtual void shoot(Bullet* clip[]) {
		Bullet* r = new Bullet_enemy(cords.x + hitbox.x / 2, cords.y + hitbox.y);//создается пуля посередине хитбокса и ниже объекта
		clip[r->get_id()]=r;
	}
	virtual void move(Event event =zaglushka)
	{
		
		float x = cords.x;
		float y = cords.y;
		if (x > move_area_x.y - hitbox.x-2*speed || x < move_area_x.x+2*speed || x<2*speed || x>window_x - 2*speed)
		{
			dir_x *= -1;
			dir_y *= pow(-1, rand() % 2);
		};
		if (y > move_area_y.y - hitbox.y - 2*speed || y < move_area_y.x + 2*speed || y<2*speed || y>window_y -2*speed)
		{
			dir_y *= -1;
			dir_x *= pow(-1, rand() % 2);
		};;
		x += dir_x;
		y += dir_y;
		cords = Vector2f(x, y);
	}
	virtual int death() {
		cords = Vector2f(-2000, 2000);//при смерти, отправляется на кладбище
		return exp;
	}
};
class Enemy_1 :public Enemy {
public:
	Enemy_1(int exp_boost, int HP, float x = window_x / 2, float y = window_y / 2) {//с возможностью задать начальные координаты
		hp = HP;
		exp = 20;
		if (exp_boost != 0) {
			exp = exp * exp_boost;
		}
		hitbox = Vector2f(100, 100);
		cords = Vector2f(x,y);//начальная координата
		size_move_area = Vector2f(300, 250);
		move_area_x = Vector2f(cords.x - size_move_area.x, cords.x + size_move_area.x);
		move_area_y = Vector2f(cords.y - size_move_area.y, cords.y + size_move_area.y);
		//cords = Vector2f(cords.x + 15, cords.y -10 );//смещение для задачи угла отражения
	}
	
	void object(RenderWindow& window) {
		Texture texture;
		texture.loadFromFile("images/enemy1.png");
		fig.setTexture(&texture);
		fig.setRadius(50);
		fig.setPointCount(4);
		fig.setPosition(cords);
		window.draw(fig);
	}
private:
	CircleShape fig;//фигура для предварительной модели игры 
};
class Enemy_2 :public Enemy {
public:
	Enemy_2(int exp_boost,int HP,float x = window_x / 2, float y = window_y / 2) {//с возможностью задать начальные координаты
		hp = HP;
		exp = 50;
		if (exp_boost != 0) {
			exp = exp * exp_boost;
		}
		hitbox = Vector2f(200, 200);
		cords = Vector2f(x, y);//начальная координата
		size_move_area = Vector2f(300, 250);
		move_area_x = Vector2f(cords.x - size_move_area.x, cords.x + size_move_area.x);
		move_area_y = Vector2f(cords.y - size_move_area.y, cords.y + size_move_area.y);
		//cords = Vector2f(cords.x + 15, cords.y -10 );//смещение для задачи угла отражения
	}
	void object(RenderWindow& window) {
		Texture texture;
		texture.loadFromFile("images/enemy2.png");
		fig.setTexture(&texture);
		fig.setRadius(100);
		//fig.setFillColor(Color(255, 0, 0));
		fig.setPointCount(6);
		fig.setPosition(cords);
		window.draw(fig);
	}
	void shoot(Bullet* clip[]) {
		Bullet* r;
		for (int i = 0; i < 3; i++) {
			r = new Bullet_enemy(cords.x + hitbox.x / 2, cords.y + hitbox.y);//создается пуля посередине хитбокса и ниже объекта
			r->set_direction(-0.25 + i * 0.25, -1);
			clip[r->get_id()] = r;
		}
	}
	
private:
	CircleShape fig;//фигура для предварительной модели игры 
};
class Enemy_3 :public Enemy {
public:
	Enemy_3(int exp_boost,int HP, float x = window_x / 2, float y = window_y / 2) {//с возможностью задать начальные координаты
		hp = HP;
		exp = 100;
		if (exp_boost != 0) {
			exp = exp * exp_boost;
		}
		hitbox = Vector2f(100,100);
		cords = Vector2f(x, y);//начальная координата
		size_move_area = Vector2f(300, 250);
		move_area_x = Vector2f(cords.x - size_move_area.x, cords.x + size_move_area.x);
		move_area_y = Vector2f(cords.y - size_move_area.y, cords.y + size_move_area.y);
		//cords = Vector2f(cords.x + 15, cords.y -10 );//смещение для задачи угла отражения
	}
	void object(RenderWindow& window) {
		Texture texture;
		texture.loadFromFile("images/enemy3.png");
		fig.setTexture(&texture);
		fig.setRadius(50);
		fig.setPointCount(8);
		fig.setPosition(cords);
		window.draw(fig);
	}
	void shoot(Bullet* clip[]) {
		static int stak = 0;
		if (stak < 3) {
			Bullet* r = new Bullet_enemy(cords.x + hitbox.x / 2, cords.y + hitbox.y);//создается пуля посередине хитбокса и ниже объекта
			r->set_direction(-0.25 + stak * 0.25, -1);
			clip[r->get_id()] = r;
			stak++;
		}
		if (stak == 3) {
			stak -= 3;
		}
		
	}
private:
	CircleShape fig;//фигура для предварительной модели игры 
};

