#include <SFML/Graphics.hpp>
#include "Classes.h"


void delete_obj(Object* obj) {
	
}
void chek_hit(Bullet*clip[],Enemy*enemies[],Main_Сharacter* GG,int count_enemy) {//проверка попаданий
	
	for (int i = 0; i < 50; i++){
		for (int j = 0; j < count_enemy; j++) {//проверка попаданий персонажем
			if(clip[i]->get_cords().y<enemies[j]->get_cords().y+ enemies[j]->get_hitbox().y)//нижняя линия хитбокса
			{
				if (clip[i]->get_cords().y + clip[i]->get_hitbox().y > enemies[j]->get_cords().y) //верхняя линия хитбокса
				{
					if (clip[i]->get_cords().x+ clip[i]->get_hitbox().x > enemies[j]->get_cords().x )//левая линия хитбокса
					{
						if (clip[i]->get_cords().x < enemies[j]->get_cords().x+ enemies[j]->get_hitbox().x) //правая линия хитбокса
						{
							if(clip[i]->get_side()){
								clip[i]->hit();
								enemies[j]->hit();
								if (enemies[j]->get_hp() == 0) {
									enemies[j]->death();
								}
							}
						}
					}
				}
			}
		}
		//попадания по персонажу
		if (clip[i]->get_cords().y < GG->get_cords().y + GG->get_hitbox().y)//нижняя линия хитбокса
		{
			if (clip[i]->get_cords().y + clip[i]->get_hitbox().y > GG->get_cords().y) //верхняя линия хитбокса
			{
				if (clip[i]->get_cords().x + clip[i]->get_hitbox().x > GG->get_cords().x)//левая линия хитбокса
				{
					if (clip[i]->get_cords().x < GG->get_cords().x + GG->get_hitbox().x) //правая линия хитбокса
					{
						if (!(clip[i]->get_side())) {
							clip[i]->hit();
							GG->hit();
							cout << GG->get_hp() << endl;
						}
					}
				}
			}
		}
		
	}
}
int main()
{
	int timer=0;
	Enemy* ar[3];
	Enemy* wr = new Enemy_1(100, 150);
	Enemy* wr_2 = new Enemy_2(400, 150);
	Enemy* wr_3 = new Enemy_3(700, 150);
	ar[0] = wr;
	ar[1] = wr_2;
	ar[2] = wr_3;
	Bullet* clip[50];//массив хранящий все пули
	for (int i = 0; i < 50; i++) {
		clip[i] = new Bullet_character(2000, 2000);//пуля заглушка, чтобы все работало
	}
	Main_Сharacter* GG = new Main_Сharacter;
	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(window_x, window_y), "SFML Works!");
	window.setFramerateLimit(60);
	srand(time(NULL));
	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				// тогда закрываем его
				window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Space)//стрельба по нажатии на пробел
				{
					GG->shoot(clip);
					//wr->shoot(clip);
					//wr_2->shoot(clip);
				}

			}
		}
		window.clear(Color(0, 0, 0, 0));
		
		if (timer == 30) {
			ar[2]->shoot(clip);
		}
		for (int i = 0; i < 3; i++) {
			ar[i]->object(window);
			ar[i]->move();
			if (timer == 60) {
				ar[i]->shoot(clip);//стрелять каждую секунду 
			}
		}
		for (int i = 0; i < 50; i++) {
			clip[i]->object(window);
			clip[i]->move();
		}
		GG->object(window);
		GG->move(event);
		chek_hit(clip, ar, GG, 3);
		// Отрисовка окна	
		window.display();
		timer++;
		
		if (timer > 60) {
			timer -= 60;
		}
	}
	return 0;
};
