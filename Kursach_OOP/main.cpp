#include <SFML/Graphics.hpp>
#include "Classes.h"
#include<windows.h>
#include<iostream>
#include<string>
#include<fstream>

int chooseinmenu;

void SetColor(int text, int background){
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void printtitle (string title){
	cout<<"/---------------"<<title<<"---------------/"<<endl;
}

void mainmenu (string title, int highscore){
	system("CLS");
	SetColor(1,0);
	printtitle(title);
	SetColor(13,0);
	cout<<endl<<"1.PLAY EASY MODE"<<endl<<endl<<"2.PLAY MEDIUM MODE"<<endl<<endl<<"3.PLAY HARD MODE"<<endl<<endl<<"4.CLEAR HIGH SCORE"<<endl<<endl<<"5.EXIT"<<endl;
	SetColor(14,0);
	cout<<"\t\t\tHigh Score:"<<highscore<<endl;
	SetColor(15,0);
}
int highscore=0;
int menu (){
		ifstream record("highscore.txt");
		record>>highscore;
		record.close();
		
		while (chooseinmenu != 5)
		{
			mainmenu("GALLAGA", highscore);
			cin>>chooseinmenu;
			if (chooseinmenu==1){
				cout<<"START GAME..."<<endl;
				return 1;
			}
			if (chooseinmenu==2){
				cout<<"START GAME..."<<endl;
				return 2;
			}
			if (chooseinmenu==3){
				cout<<"START GAME..."<<endl;
				return 3;
			}
			if (chooseinmenu==4){
				highscore=0;
				ofstream record("highscore.txt", ios_base::trunc);
				record<<highscore;
				record.close();
			}
			if(chooseinmenu==5){
				return 5;
			}
		}
	}

int hard;
int timehard;

int score = 0;
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
									score+= enemies[j]->death();//запись счета
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
						}
					}
				}
			}
		}
		
	}
}
int status;
int main()
{
	while (status != 5) {

		status = menu();
		if (status == 5) {
			return 0;
		}
		if (status == 1) {
			hard = 0;
		}
		if (status == 2) {
			hard = 2;
		}
		if (status == 3) {
			hard = 5;
		}

		int timer = 0;
		Enemy* ar[3];//1-слабый, 3-средний, 2- сильный
		Enemy* ar_2[3];
		Enemy* ar_3[3];
		Enemy* wr = new Enemy_1(hard, 1 + hard, 100, 150);
		Enemy* wr_2 = new Enemy_1(hard, 1 + hard, 400, 150);
		Enemy* wr_3 = new Enemy_1(hard, 1 + hard, 700, 150);
		Enemy* wr_4 = new Enemy_3(hard, 2 + hard, 100, 150);
		Enemy* wr_5 = new Enemy_3(hard, 2 + hard, 400, 150);
		Enemy* wr_6 = new Enemy_3(hard, 2 + hard, 700, 150);
		Enemy* wr_7 = new Enemy_3(hard, 2 + hard, 300, 150);
		Enemy* wr_8 = new Enemy_2(hard, 3 + hard, 500, 150);
		Enemy* wr_9 = new Enemy_3(hard, 2 + hard, 700, 150);

		ar[0] = wr;
		ar[1] = wr_2;
		ar[2] = wr_3;
		ar_2[0] = wr_4;
		ar_2[1] = wr_5;
		ar_2[2] = wr_6;
		ar_3[0] = wr_7;
		ar_3[1] = wr_8;
		ar_3[2] = wr_9;
		/*Enemy* ar[3];
		Enemy* wr = new Enemy_1(1 + multylvl, 100, 150);
		Enemy* wr_2 = new Enemy_2(2 + multylvl, 400, 150);
		Enemy* wr_3 = new Enemy_3(3 + multylvl, 700, 150);
		ar[0] = wr;
		ar[1] = wr_2;
		ar[2] = wr_3;*/
		Bullet* clip[50];//массив хранящий все пули
		for (int i = 0; i < 50; i++) {
			clip[i] = new Bullet_character(2000, 2000);//пуля заглушка, чтобы все работало
		}
		Main_Сharacter* GG = new Main_Сharacter;
		// Объект, который, собственно, является главным окном приложения
		RenderWindow window(VideoMode(window_x, window_y), "Gallaga!");
		window.setFramerateLimit(60);
		srand(time(NULL));
		Texture background;
		background.loadFromFile("images/background.jpg");
		Sprite backing(background);
		// Главный цикл приложения. Выполняется, пока открыто окно
		while (window.isOpen())
		{
			// Обрабатываем очередь событий в цикле
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();

				window.draw(backing);
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
			window.clear();
			window.draw(backing);
			//if (timer == 30) {
			//	ar[2]->shoot(clip);
			//}
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

			if (ar[0]->get_hp() == 0 && ar[1]->get_hp() == 0 && ar[2]->get_hp() == 0) {
				for (int i = 0; i < 3; i++) {
					ar_2[i]->object(window);
					ar_2[i]->move();
					if (timer == 10) {
						ar_2[i]->shoot(clip);//стрелять каждую секунду 
					}
				}
				chek_hit(clip, ar_2, GG, 3);
			}

			if (ar_2[0]->get_hp() == 0 && ar_2[1]->get_hp() == 0 && ar_2[2]->get_hp() == 0) {
				for (int i = 0; i < 3; i++) {
					ar_3[i]->object(window);
					ar_3[i]->move();
					if (timer == 60) {
						ar_3[i]->shoot(clip);//стрелять каждую секунду 
					}
				}
				chek_hit(clip, ar_3, GG, 3);
			}

			if (ar_3[0]->get_hp() == 0 && ar_3[1]->get_hp() == 0 && ar_3[2]->get_hp() == 0) {

				window.close();
			}

			ofstream record("highscore.txt", ios_base::trunc);
			if (score > highscore) {
				record << score;
			}
			record.close();

			if (GG->get_hp() == 0) {
				score = 0;
				window.close();
			}

			CircleShape image_heart;
			Texture heart;
			heart.loadFromFile("images/heart.png");
			image_heart.setTexture(&heart);
			image_heart.setRadius(20);
			image_heart.setPointCount(6);
			image_heart.setPosition(850,900);
			window.draw(image_heart);

			
			Font font;
			font.loadFromFile("Notalot60.ttf");
			Text main_hp;
			Text hs;
			main_hp.setFont(font);
			hs.setFont(font);
			main_hp.setCharacterSize(30);
			hs.setCharacterSize(25);
			main_hp.setPosition(900, 900);
			hs.setPosition(850, 50);
			string a_hp = "HP:   ";
			string b_hp = to_string(GG->get_hp());
			string a_exp = "EXP:   ";
			string b_exp = to_string(score);
			main_hp.setString(a_hp + b_hp);
			hs.setString(a_exp + b_exp);
			window.draw(main_hp);
			window.draw(hs);
			

			// Отрисовка окна	
			window.display();
			timer++;

			if (timer > 60) {
				timer -= 60;
			}
		}
	}
	return 0;
};
