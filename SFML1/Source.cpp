#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<vector>
#include"view.h"
#include<iostream>
#include<vector>
#include"level.h"
#include<math.h>

using namespace sf;

//--------------------------------------------------------------------------------------------ARCHI
enum astate
{
	stop,
	jump, 
	down,
	zapad, 
	vostok,
};


enum vecstate
{
	ne,
	es,
	sw,
	wn,
	centr,
};



//1
class hero;

//2
class actor;

//3
class enemy;

//3
void updatescene(hero &hero, enemy &demon, float time);

//4
float checkrange(float px1, float  py1, float px2, float py2);


//--------------------------------------------------------------------------------------------ACTOR
class actor
{
protected:
	//------rpg-----//
	
	int health = 100;

	//1
	String actorname;

	//2
	std::vector<Object> mapobje; //vector object of map

	//3
	float px = 0;
	float py = 0;
	bool onGround = false;
	float speed = 0;

	//4
	int we = 0;
	int hi = 0;
	float cadrframe = 0;

	//5
	String neededfile;
	Image actorimage;
	Texture actortexture;
	Sprite actorsprite;

	//con//
public:

	bool alive = true;

	actor(String actorname, int px, int py, int speed, int we, int hi, String neededfile)
	{
		//1
		this->actorname = actorname;
		this->py = py;
		this->px = px;
		this->speed = speed;
		this->we = we;
		this->hi = hi;
		this->neededfile = neededfile;

		
		//2
		actorimage.loadFromFile("images/" + this->neededfile);
		actorimage.createMaskFromColor(Color(255, 255, 255));
		actortexture.loadFromImage(actorimage);
		actorsprite.setTexture(actortexture);
		
		//3
		actorsprite.setTextureRect(IntRect(0, 0, this->we, this->hi));
		actorsprite.setPosition(100, 100);
	}

	//------------------
	float Get_X_actor();
	float Get_Y_actor();
	Sprite getSprite();
	FloatRect getRect();
	//------------------
};



//--------------------------------------------------------------------------------------------HERO
class hero : public actor
{
protected:

	friend class enemy;

	friend void updatescene(hero &hero, enemy &demon, float time);

	//------rpg-----//
	bool isblock = false;

	bool isfight = false;
	float fightframe = 0;

	SoundBuffer kata;
	Sound kat;
	SoundBuffer kdead;
	Sound kdea;

	//1
	astate state = astate::vostok;
	float acex = 0;
	float acey = 0;
	bool isMove = false;

	//con//
public:
	hero(String heroname,Level lvlobj, int px, int py, int speed, int we, int hi, String neededfile) : actor(heroname, px,  py,  speed,  we,  hi, neededfile)
	{
		mapobje = lvlobj.GetAllObjects();
		kata.loadFromFile("knight strike.ogg");
		kat.setBuffer(kata);
		kdead.loadFromFile("knight dead.ogg");
		kdea.setBuffer(kdead);
	}

	//--------------------------------------
	void updatehero(float time);
	void heromove();
	void checkcollision(float Dx, float Dy);

	bool iffight()
	{
		return isfight;
	}

	float getfight()
	{
		return fightframe;
	}
	//--------------------------------------
};


//--------------------------------------------------------------------------------------------ENEMY
class enemy : public actor
{
protected:

	friend void updatescene(hero &hero, enemy &demon, float time);

	//------rpg-----//
	bool isfight = false;
	float fightframe = 0;

	bool isvision = false;
	vecstate state = vecstate::centr;

	float atackrange = 0;

	int atacktype = 1;

	SoundBuffer data;
	Sound dat;
	SoundBuffer dede;
	Sound ded;

	//1
	float acex = 0;
	float acey = 0;

	bool isMove = false;
	float moveTimer;

	//con//
public:
	enemy(String enemyname, Level lvlobj, int px, int py, int speed, int we, int hi, String neededfile) : actor(enemyname, px, py, speed, we, hi, neededfile)
	{
		mapobje = lvlobj.GetAllObjects();
		data.loadFromFile("demon atack.ogg");
		dede.loadFromFile("demon dead.ogg");
		dat.setBuffer(data);
		ded.setBuffer(dede);
	}

	//--------------------------------------
	void updateenemy(hero & hero, float time);
	void enemymove(hero hero);
	void checkcollision(hero hero, vecstate state);
	//--------------------------------------
};

//--------------------------------------------------------------------------------------------MAIN
int main()
{
	//1
	RenderWindow window(VideoMode(1280,720), "PESHCHERA MAMKI VLADA");
	heroview.reset(FloatRect(0, 0, 640, 360));
	
	//2
	Level lvl;
	lvl.LoadFromFile("map.tmx");
	
	//3
	Clock clock;
	float time;


	//4*
	Font namefont;
	namefont.loadFromFile("13859.ttf");
	Text nametext("", namefont, 8);

	

	//5
	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	hero hero("Firmeldir", lvl, player.rect.left, player.rect.top, 14, 56, 40, "hero1.png");

	Object kheron_demon = lvl.GetObject("kheron_demon");
	enemy demon("Demon", lvl, kheron_demon.rect.left, kheron_demon.rect.top, 7, 160, 132, "lol.png");
	
	//main_main
	while(window.isOpen())
	{
		Event event;
		while(window.pollEvent(event))
		{
			if(event.type == Event::Closed)
			{
				window.close();
			}
		}

		//std::cout << hero.Get_X_actor() <<" & " << hero.Get_Y_actor() << std::endl;

		///1
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		///2
		updatescene(hero, demon, time);

		///3
		window.setView(heroview);

		///4
		window.clear(Color::Color(30,29,22));
		
		lvl.Draw(window);

		if(hero.alive)
		{
			window.draw(hero.getSprite());
			///5*
			nametext.setString("firmeldir");
			nametext.setPosition(hero.Get_X_actor() + 10, hero.Get_Y_actor() - 5);
			window.draw(nametext);
		}


		if(demon.alive)
		{
			window.draw(demon.getSprite());
		}
		

		

		///6
		window.display();
	}
}



//--------------------------------------------------------------------------------------------

//1
void updatescene(hero &hero, enemy &demon, float time)
{
	time = time / 80000;
	if(hero.alive)
	{
		hero.updatehero(time);
	}
	
	demon.updateenemy(hero, time);

}

//1,2
float checkrange(float px1, float py1, float px2, float py2)
{
	return (  std::sqrtf(((px1 - px2) * (px1 - px2)) + ((py1 - py2) * (py1 - py2)))      );
}

//2
float actor::Get_X_actor()
{
	return this->px;
}

//3
float actor::Get_Y_actor()
{
	return this->py;
}

//4
Sprite actor::getSprite()
{
	return actorsprite;
}

//5
FloatRect actor::getRect()
{
	return FloatRect(px, py, we, hi);
}

//6
void hero::updatehero(float time)
{
	if (health < 1)
	{
		if (fightframe <= 9)
		{
			fightframe += 0.009;
			actorsprite.setTextureRect(IntRect(27 + (42 * (int)fightframe), 128, 42, 42));
		}
		else
		{
			kdea.play();
			alive = false;
		}

	}

	if(health > 0)
	{
		//1
		heromove();

		if (isfight)
		{
			if (fightframe <= 10)
			{
				fightframe += 0.05;
				if ((state == astate::zapad))
				{
					actorsprite.setTextureRect(IntRect(23 + we + (79 * (int)fightframe), 50, -(we + 2), hi));
				}
				else if ((state == astate::vostok))
				{
					actorsprite.setTextureRect(IntRect(23 + (79 * (int)fightframe), 50, we + 2, hi));
				}
			}
			else
			{
				fightframe = 0;
				isfight = false;
			}



		}
		else
		{
			//2
			if (state == astate::stop)
			{
				actorsprite.setTextureRect(IntRect(0, 0, we, hi));
			}
			else if (state == astate::jump)
			{
			}
			else if (state == astate::down)
			{
			}
			else if (state == astate::zapad)
			{
				if (isMove)
				{
					cadrframe += 0.01;
					if (cadrframe >= (8)) { cadrframe = 0; }
					actorsprite.setTextureRect(IntRect(22 + we + (42 * (int)cadrframe), 9, -we, hi));

					acex = (-1) * speed;
					isMove = true;
				}
				else if (isblock)
				{
					fightframe += 0.1;
					if (fightframe >= 7) { fightframe = 6.2; }
					actorsprite.setTextureRect(IntRect(22 + we + (42 * (int)fightframe), 89, -(we), hi));
				}
				else
				{
					actorsprite.setTextureRect(IntRect(23 + we, 50, -we, hi));
				}


			}
			else if (state == astate::vostok)
			{
				if (isMove)
				{
					cadrframe += 0.01;
					if (cadrframe >= (8)) { cadrframe = 0; }
					actorsprite.setTextureRect(IntRect(22 + (42 * (int)cadrframe), 9, we, hi));

					acex = speed;
					isMove = true;
				}
				else if (isblock)
				{
					fightframe += 0.1;
					if (fightframe >= 7) { fightframe = 6.2; }
					actorsprite.setTextureRect(IntRect(22 + (42 * (int)fightframe), 89, (we), hi));
				}
				else
				{
					actorsprite.setTextureRect(IntRect(23, 50, we, hi));
				}
			}
		}

		//3
		px += acex * time;
		checkcollision(acex, 0);

		py += acey * time;
		checkcollision(0, acey);

		acex = 0;

		actorsprite.setPosition(px, py);

		coordinateforview(Get_X_actor(), Get_Y_actor());

		if (health <= 0) { alive = false; }
		isMove = false;

		acey += 1.5 * time;

	}

	
}

void hero::heromove()
{
	if (Keyboard::isKeyPressed && (!isfight)) 
	{
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			state = astate::vostok; isMove = true;
			isblock = false;
			fightframe = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			state = astate::zapad; isMove = true;
			isblock = false;
			fightframe = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Space) && (onGround == true))
		{
			state = astate::jump; acey = -10; onGround = false;
			isblock = false;
			fightframe = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			state = astate::down;
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			isfight = true;
			isblock = false;
			fightframe = 0;
		}

		if (Mouse::isButtonPressed(Mouse::Right))
		{
			isblock = true;
		}
	}
}

void hero::checkcollision(float Dx, float Dy)
{

	for (int i = 0; i < mapobje.size(); i++) 
	{
		if (getRect().intersects(mapobje[i].rect))//проверяем пересечение игрока с объектом
		{
			if (mapobje[i].name == "collision")//если встретили препятствие
			{
				if (Dy > 0) { py = mapobje[i].rect.top - hi;  acey = 0; onGround = true; }
				if (Dy < 0) { py = mapobje[i].rect.top + mapobje[i].rect.height;   acey = 0; }
				if (Dx > 0) { px = mapobje[i].rect.left - we; }
				if (Dx < 0) { px = mapobje[i].rect.left + mapobje[i].rect.width; }
			}
		}
	}
}

//--------------------------------------------------------------------------------------------

void enemy::updateenemy(hero & hero, float time)
{
	if (!isfight)
	{
		enemymove(hero);

		px += acex * time * speed;
		py += acey * time * speed;

		acex = 0;
		acey = 0;

		actorsprite.setPosition(px, py);
		checkcollision(hero, state);
	}
	else
	{
		if(atacktype == 2)
		{
			if(moveTimer < 200)
			{
				moveTimer += time;
				cadrframe += 0.02;
				if (cadrframe >= (6)) { cadrframe = 0; }
				actorsprite.setTextureRect(IntRect(248 + (160 * (int)cadrframe), 384, 160, 132));

			}else
			{
				moveTimer = 0;
				isfight = false;
				atacktype = 1;
			}


			
			if ((checkrange(hero.Get_X_actor(), hero.Get_Y_actor(), px, py) < 132) && (hero.iffight()) && (hero.getfight() <= 0.06) )//если встретили препятствие
			{
				if(health > 60)
				{
					hero.kat.play();
				}				
				health -= 50;
			}
				
		}
		else
		{
			if (fightframe <= 11)
			{
				fightframe += 0.02;
				actorsprite.setTextureRect(IntRect(124+(235*(int)fightframe),135,202,170));

				if ((checkrange(hero.Get_X_actor(), hero.Get_Y_actor(), px, py) < 130) && (isfight) && ((fightframe >= 8)&&(fightframe <= 11)))//если встретили препятствие
				{
					dat.play();
					hero.health -= 50;
				}
				
			}
			else
			{
				fightframe = 0;
				isfight = false;
				atacktype = 2;
			}
		}
	}

	
	if(health < 1)
	{
		if(fightframe <= 5)
		{
			fightframe += 0.009;
			actorsprite.setTextureRect(IntRect(147 + (235 * (int)fightframe), 572, 202, 170));
			ded.play();

		}else
		{
			
			alive = false;
		}
		
	}
}

void enemy::enemymove(hero hero)
{
	//movelogick
	int tarx = 0;
	int tary = 0;

	if (atacktype == 1)
	{
		tarx = hero.Get_X_actor() -15;
		tary = hero.Get_Y_actor() - 110;
	}
	else
	{
		tarx = 1260;
		tary = 176;
	}

	atackrange = checkrange(tarx, tary, px, py);

	if (checkrange(tarx, tary, px + 5, py - 5) < atackrange)
	{
		state = vecstate::ne;
		acex += 1; acey -= 1;
	}

	if (checkrange(tarx, tary, px - 5, py + 5) < atackrange)
	{
		state = vecstate::sw;
		acex -= 1; acey += 1;
	}

	if (checkrange(tarx, tary, px + 5, py + 5) < atackrange)
	{
		state = vecstate::es;
		acex += 1; acey += 1;
	}

	if (checkrange(tarx, tary, px - 5, py - 5) < atackrange)
	{
		state = vecstate::wn;
		acex -= 1; acey -= 1;
	}
		
	cadrframe += 0.025;
	if (cadrframe >= (6)) { cadrframe = 0; }
	actorsprite.setTextureRect(IntRect(248 + (160 * (int)cadrframe), 384, 160, 132));
}

void enemy::checkcollision(hero hero, vecstate state)
{
	int tarx = 0;
	int tary = 0;

	if (atacktype == 1)
	{
		tarx = hero.Get_X_actor() - 15;
		tary = hero.Get_Y_actor() - 110;
	}
	else
	{
		tarx = 1260;
		tary = 176;
	}

	if (checkrange(tarx, tary, px, py) < 25)//если встретили препятствие
	{
		this->isfight = true;
	}



	if ((checkrange(hero.Get_X_actor(), hero.Get_Y_actor(), px, py) < 132) && (hero.iffight()) && (hero.getfight() <= 0.06))//если встретили препятствие
	{
		hero.kat.play();
		health -= 50;
	}

}
