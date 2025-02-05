﻿#include "Game.h"
#include "RoomTemplates.h"

// Направления движения
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

Game::Game()
{
	SDL_Point player_spawn_point{ 0,0 }; // Точка появления игрока
	SDL_Point player_size{45, 45}; // Размеры игрока
	player_1 = new GameActor(player_spawn_point, player_size.x, player_size.y,
		0, 10, 10, 5, true, 1); // Создаём объект-персонаж типа "1" - первый игрок

	active_room = nullptr;

	current_actors = nullptr;

	current_objects = nullptr;

	map = new Map(10);
	map->Gen();
}

Game::~Game()
{
	delete active_room;
}

GameObject* Game::check_all_collisions(GameObject* object)
{
	// Проверяем столкновение с границами комнаты
	auto obj_rect = object->get_object_rect();
	if (obj_rect.x < 0 || obj_rect.x + obj_rect.w > active_room->get_size().x ||
		obj_rect.y < 0 || obj_rect.y + obj_rect.h > active_room->get_size().y)
		return new GameObject(WALL); // Возвращаем объект с типом "-1"
	
	// Проверка на столкновение с игроком
	auto player = get_player_1();
	if (object != player) {
		if (SDL_HasIntersection(&object->get_object_rect(), &player->get_object_rect())) {
			return player; // объект, с которым пересеклись
		}
	}
	// Проверка на пересечение со всеми объектами в комнате
	auto obj_vector = get_current_objects();
	// Обходим все объекты
	for (int i = 0; i < obj_vector->size(); i++) {
		GameObject* current_obj = obj_vector->at(i); // Текущий объект, с которым ведётся сравнение
		if (current_obj != object) { // Проверяем, чтобы этот объект не является самим первым объектом
			if (SDL_HasIntersection(&object->get_object_rect(), &current_obj->get_object_rect())) {
				return current_obj; // объект, с которым пересеклись
			}
		}
	}
	// Проверка на пересечение со всеми персонажами в комнате
	auto act_vector = get_current_actors();
	// Обходим всех персонажей
	for (int i = 0; i < act_vector->size(); i++) {
		GameObject* current_act = act_vector->at(i); // Текущий объект, с которым ведётся сравнение
		if (current_act != object) { // Проверяем, чтобы этот объект не является самим первым объектом
			if (SDL_HasIntersection(&object->get_object_rect(), &current_act->get_object_rect())) {
				return current_act; // Возвращаем персонажа, с которым пересеклись
			}
		}
	}
	return nullptr;
}

void Game::load_test_room()
{
	//active_room = new GameRoom(1, 1, 0);
	active_room = map->matrix[5][6];
	auto player = get_player_1();
	current_actors = active_room->get_actors();
	current_objects = active_room->get_objects();
}

void Game::load_room(int i, int j)
{
	active_room = map->matrix[i][j];
	current_actors = map->matrix[i][j]->get_actors();
	current_objects = map->matrix[i][j]->get_objects();
}

void Game::CreateBullet()
{
	SDL_Point CenterPoint = player_1->getCoord();
	SDL_Point CenterPoint2 = player_1->get_size();
	CenterPoint.x += CenterPoint2.x / 2 - 10;
	CenterPoint.y += CenterPoint2.y / 2 - 10;
	GameObject* bullet = new  GameObject(CenterPoint, 20, 20, false, false, true, false, 3);
	bullet->set_speed(2);
	bullet->set_direction(player_1->get_direction());
	bullet->set_type(5);
	current_objects->push_back(bullet);
}

void Game::tact(){
	for(int i = 0 ;i < current_actors->size(); i++){
		auto cur_actor = current_actors->at(i);
		if (cur_actor->get_type() == 7){ // Проверка на то, что cur_actor - враг
			if (getDistance(get_player_1(),cur_actor) < 300){ // Если враг "видит" нас, то он начинает нас преследовать
				int dx = (get_player_1()->getCoord().x + get_player_1()->get_size().x) - (cur_actor->getCoord().x + cur_actor->get_size().x);
				if (dx < 0) // Если игрок левее врага
				{
					if (!move_gameObject(cur_actor, LEFT))
						if (!move_gameObject(cur_actor, (rand() % 2 == 1) ? 2 : 0))
							move_gameObject(cur_actor, RIGHT);
				}
				
				else if (dx > 0) // Если игрок правее врага
				{
					if (!move_gameObject(cur_actor, RIGHT))
						if (!move_gameObject(cur_actor, (rand() % 2 == 1) ? 2 : 0))
							move_gameObject(cur_actor, LEFT);
				}
				int dy = (get_player_1()->getCoord().y + get_player_1()->get_size().y) - (cur_actor->getCoord().y + cur_actor->get_size().y);
				if (dy < 0) // Если игрок выше врага
					move_gameObject(cur_actor, UP);
				else if (dy > 0) // Если игрок ниже врага
					move_gameObject(cur_actor, DOWN);
			}
			move_gameObject(cur_actor, rand()%4); // Если cur_actor - враг, двигаем его в случайную сторону
		}
	}
}


double Game::getDistance(GameObject* obj1, GameObject* obj2)
{
	SDL_Rect obj1_rect = obj1->get_object_rect();
	SDL_Rect obj2_rect = obj2->get_object_rect();

	//расстояния по х и у между двумя обь
	int distanseX = obj2_rect.x + obj2_rect.w / 2 - (obj1_rect.x + obj1_rect.w / 2);
	int distanseY = obj2_rect.y + obj2_rect.h / 2 - (obj1_rect.y + obj1_rect.h / 2);

	return (sqrt(distanseX * distanseX + distanseY * distanseY));
}

//если с чем-то столкнулись вернет true, иначе false
bool Game::move_gameObject(GameObject* object, int direction)
{
	object->move(direction);
	GameObject* object_which_collissed = check_all_collisions(object);
	// Игрок не останавливается, 
	if (!object_which_collissed) { // если не пересекаемся ни с одним объектом
		return true;
	}
	// Тестовая проверка на двери
	else if (object == get_player_1() && object_which_collissed->get_type() == 50) { // Дверь вверх
		load_room(active_room->get_i() - 1, active_room->get_j());
		//active_room = map->matrix[active_room->get_i() - 1][active_room->get_j()];
		get_player_1()->setCoord(new SDL_Point{ 0,0 });
		//get_player_1()->setCoord(new SDL_Point{ active_room->get_size().x / 2, active_room->get_size().y - get_player_1()->get_size().y - 15 });
	}
	else if (object == get_player_1() && object_which_collissed->get_type() == 51) { // Дверь вправо
		load_room(active_room->get_i(), active_room->get_j()+1);
		//active_room = map->matrix[active_room->get_i()][active_room->get_j() + 1];
		get_player_1()->setCoord(new SDL_Point{ 0,0 });
		//get_player_1()->setCoord(new SDL_Point{ 0, active_room->get_size().y/2 });
	}
	else if (object == get_player_1() && object_which_collissed->get_type() == 52) { // Дверь вниз
		load_room(active_room->get_i() + 1, active_room->get_j());
		//active_room = map->matrix[active_room->get_i() + 1][active_room->get_j()];
		get_player_1()->setCoord(new SDL_Point{ 0,0 });
		//get_player_1()->setCoord(new SDL_Point{ active_room->get_size().x / 2, 15});
	}
	else if (object == get_player_1() && object_which_collissed->get_type() == 53) { // Дверь влево
		load_room(active_room->get_i(), active_room->get_j()-1);
		//active_room = map->matrix[active_room->get_i()][active_room->get_j() - 1];
		get_player_1()->setCoord(new SDL_Point{ 0,0 });
		//get_player_1()->setCoord(new SDL_Point{ active_room->get_size().x - get_player_1()->get_size().x, active_room->get_size().y/2 });
	}
	//
	else if (object_which_collissed->get_is_pushable()) { // либо объект, с которым пересекаемся, разрешено толкать
		move_gameObject(object_which_collissed, direction);
		// object_which_collissed->move(direction);
	}
	else if (object_which_collissed->get_is_passable()) { // либо через объект, с которым пересекаемся, разрешено проходить
		return true;
	}
	object->move((direction+2)%4);
	return false;
}