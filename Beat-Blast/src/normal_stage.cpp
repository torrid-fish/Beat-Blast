#include "normal_stage.h"

bool IS_2P;
bool LEVEL; // 1 HARD 0 EASY
// Whether all monsters are cleared.
bool finish = false;
static ALLEGRO_SAMPLE_ID effect_id;
static ALLEGRO_SAMPLE_ID bgm_id;

NORMAL_STAGE::NORMAL_STAGE() {
	name = "NORMAL_STAGE";
	map = create_map("Assets/map.txt");
	// Initialize players and monsters
	for (int x = 0; x < map->col_num; x++)
		for (int y = 0; y < map->row_num; y++) {
			if (is_p1(map, x, y))
				player1 = new PLAYER(x, y, 2, 2, "p1", map);
			if (is_p2(map, x, y))
				player2 = new PLAYER(x, y, 2, 2, "p2", map);
			// if (is_big_monster(map, x, y)) {
			// 	BIG_MONSTER* temp = new BIG_MONSTER(x, y, "big monster", map);
			// 	big_monsters.emplace_back(temp);
			// }
			if (is_crystal(map, x, y)) {
				CRYSTAL* temp = new CRYSTAL(x, y, 0);
				crystals.emplace_back(temp);
			}
		}
	if (!IS_2P) player2 = NULL;
	if (LEVEL == 1) {
		for (int i = 0; i < HARD_BIG_MONSTER_NUM; i++) {
			BIG_MONSTER* temp = new BIG_MONSTER("big monster", map);
			big_monsters.emplace_back(temp);
		}
		for (int i = 0; i < HARD_SMALL_MONSTER_NUM; i++) {
			SMALL_MONSTER* temp = new SMALL_MONSTER("small monster", map);
			small_monsters.emplace_back(temp);
		}
	}
	else {
		for (int i = 0; i < EASY_BIG_MONSTER_NUM; i++) {
			BIG_MONSTER* temp = new BIG_MONSTER("big monster", map);
			big_monsters.emplace_back(temp);
		}
		for (int i = 0; i < EASY_SMALL_MONSTER_NUM; i++) {
			SMALL_MONSTER* temp = new SMALL_MONSTER("small monster", map);
			small_monsters.emplace_back(temp);
		}
	}
	// Initialize damping
	damping = false;
	dampCNT = 0;
	dampThreshold = 128; 
	
	// Play enter intro
	stop_bgm(effect_id);
	effect_id = play_audio(ENTER_INTRO, music_volume);

	// State control
	state = DISPLAY_PLAYER;
	counter = 0;

	death_timer1 = al_create_timer(1.0f); // 1 tick / sec
	al_set_timer_count(death_timer1, 0);
	death_timer2 = al_create_timer(1.0f); // 1 tick / sec
	al_set_timer_count(death_timer2, 0);
	beat_timer = al_create_timer(60.0f / 76);
	al_set_timer_count(beat_timer, 0);
	al_start_timer(beat_timer);
}

NORMAL_STAGE::~NORMAL_STAGE() {
	// TODO
}

void NORMAL_STAGE::update() {
	if (is_exit(map, player1->grid_x, player1->grid_y) && is_exit(map, player2->grid_x, player2->grid_y)) {
		next_scene = new WIN();
		stop_bgm(bgm_id);
	}
	// Update counter
	counter++;
	beatCNT = al_get_timer_count(beat_timer) % 12;
	// FSM 
	if (state == DISPLAY_PLAYER && counter >= 274) 
		state = DISPLAY_MAP; 
	else if (state == DISPLAY_MAP && counter >= 487) 
		state = DISPLAY_CRYSTAL;
	else if (state == DISPLAY_CRYSTAL && counter >= 599)
		state = DISPLAY_SMALL_MONSTER;
	else if (state == DISPLAY_SMALL_MONSTER && counter >= 710)
		state = DISPLAY_BIG_MONSTER;
	else if (state == DISPLAY_BIG_MONSTER && counter >= 815)
		state = DISPLAY_START;
	else if (state == DISPLAY_START && counter >= 978) {
		state = GAME_PROCEEDING;
		counter = 0;
		bgm_id = play_bgm(GAME_BGM, music_volume);
	}
	
	// Game update only when GAME_PROCEEDING
	if (state == GAME_PROCEEDING) {
		// Damping process
		if (damping) {
			dampCNT += 1;
			float damped_val = dampedOscillation(10, dampCNT, pi / 16, 0.05);
			if (dampCNT > dampThreshold)
				damping = false, dampCNT = 0;
			map_offset_x = fix_map_offset_x + damped_val;
			map_offset_y = fix_map_offset_y;
		}
		else {
			map_offset_x = fix_map_offset_x;
			map_offset_y = fix_map_offset_y;
		}

		// Ammo update
		for (int i = 0; i < ammos.size(); i++) {
			AMMO* ammo = ammos[i];
			if (ammo->pattern == FIREBALL && ammo->timeCNT == 64) {
				stop_bgm(effect_id);
				effect_id = play_audio(SAMSONG_NOTIFICATION_EFFECT, effect_volume / 8);
				damping = true, dampCNT = 0; // Trigger, and reset damping
			}
			ammo->update();
			bool delete_ammo = false;
			// If the fireball exploded 
			if (ammo->pattern == FIREBALL && ammo->timeCNT >= 74)
				delete_ammo = true;
			for (int j = 0; j < map->col_num; j++)
				for (int k = 0; k < map->row_num; k++)
					if (map->map[k][j] == '#' || is_exit(map, j, k)) // Touches wall to clear
						delete_ammo |= RecOverlap(get_rec_by_pt_on_board(j, k, 1, 1), ammo->drawrec);
			if (delete_ammo) {
				ammos.erase(ammos.begin() + i);
				i--;
				delete ammo;
			}
		}

		// Crystal update
		for (auto crystal : crystals) {
			if (beatCNT == 0)
				crystal->beatCNT = 0;
			else
				crystal->beatCNT = (crystal->beatCNT + 1) % 128;
			crystal->update();
		}

		// Small monster update
		for (auto monster : small_monsters)
			monster->update();

		// Big monster update
		for (auto monster : big_monsters) {
			monster->update();
			// Chasing strategy update
			if (al_get_timer_count(death_timer1) > 0 && al_get_timer_count(death_timer2) > 0)
				monster->next_facing = generatRandomDirection();
			else if (!IS_2P || al_get_timer_count(death_timer2) > 0)
				monster->next_facing = A_star(map, monster->grid_w, monster->grid_h, monster->grid_x, monster->grid_y, player1->grid_x, player1->grid_y);
			else if (al_get_timer_count(death_timer1) > 0)
				monster->next_facing = A_star(map, monster->grid_w, monster->grid_h, monster->grid_x, monster->grid_y, player2->grid_x, player2->grid_y);
			else if (EuclideanDistanceSquare(monster->grid_x, monster->grid_y, player1->grid_x, player1->grid_y) <
				EuclideanDistanceSquare(monster->grid_x, monster->grid_y, player2->grid_x, player2->grid_y))
				monster->next_facing = A_star(map, monster->grid_w, monster->grid_h, monster->grid_x, monster->grid_y, player1->grid_x, player1->grid_y);
			else
				monster->next_facing = A_star(map, monster->grid_w, monster->grid_h, monster->grid_x, monster->grid_y, player2->grid_x, player2->grid_y);
		}

		// Update ammo collision
		for (int i = 0; i < ammos.size(); i++) {
			AMMO* ammo = ammos[i];
			// Check collision with monster
			for (int j = 0; j < small_monsters.size(); j++) {
				SMALL_MONSTER* monster = small_monsters[j];
				if (RecOverlap(ammo->drawrec, monster->drawrec)) {
					if (ammo->pattern == FIREBALL)
						monster->hp -= 30;
					else
						monster->hp -= 15;
					// Delete monster and generate items
					if (monster->hp <= 0) {
						// Generate item
						ITEM* item = new ITEM(monster->drawrec.midx(), monster->drawrec.midy(), monster->drawrec.w / 2, monster->drawrec.h / 2, 0);
						items.emplace_back(item);
						// Delete monster
						small_monsters.erase(small_monsters.begin() + j);
						j--;
						delete monster;
					}
					ammos.erase(ammos.begin() + i);
					i--;
					delete ammo;
					break;
				}
			}
			for (int j = 0; j < big_monsters.size(); j++) {
				BIG_MONSTER* monster = big_monsters[j];
				if (RecOverlap(ammo->drawrec, monster->drawrec)) {
					if (ammo->pattern == FIREBALL)
						monster->hp -= 30;
					else
						monster->hp -= 15;
					if (monster->hp <= 0) {
						// Generate item
						ITEM* item = new ITEM(monster->drawrec.midx(), monster->drawrec.midy(), monster->drawrec.w / 2, monster->drawrec.h / 2, 0);
						items.emplace_back(item);
						// Delete monster
						big_monsters.erase(big_monsters.begin() + j);
						j--;
						delete monster;
					}
					ammos.erase(ammos.begin() + i);
					i--;
					delete ammo;
					break;
				}
			}
		}

		/* Player 1 */
		if (al_get_timer_count(death_timer1) > 0)
			player1->next_facing = NONE;
		// Player update
		player1->update();

		// Update collision with small monsters
		for (auto monster : small_monsters)
			if (RecOverlap(player1->drawrec, monster->drawrec))
				player1->hp -= 1;

		// Update collision with big monster
		for (auto monster : big_monsters)
			if (RecOverlap(player1->drawrec, monster->drawrec))
				player1->hp -= 5;

		if (player1->hp < 0) {
			al_start_timer(death_timer1);
			stop_bgm(effect_id);
			effect_id = play_audio(THE_BRUH_EFFECT, effect_volume);
		}
		// Update collision with items
		for (int i = 0; i < items.size(); i++) {
			ITEM* item = items[i];
			if (RecOverlap(item->rec, player1->drawrec)) {
				player1->mp = fmin(100.0, player1->mp + 20);
				items.erase(items.begin() + i);
				i--;
				delete item;
			}
		}

		/* Player 2 */
		if (al_get_timer_count(death_timer2) > 0)
			player2->next_facing = NONE;
		if (IS_2P) {
			// Player update
			player2->update();

			// Update collision with small monsters
			for (auto monster : small_monsters)
				if (RecOverlap(player2->drawrec, monster->drawrec))
					player2->hp -= 1;

			// Update collision with big monster
			for (auto monster : big_monsters)
				if (RecOverlap(player2->drawrec, monster->drawrec))
					player2->hp -= 5;

			if (player2->hp < 0) {
				al_start_timer(death_timer2);
				stop_bgm(effect_id);
				effect_id = play_audio(THE_BRUH_EFFECT, effect_volume);
			}

			for (int i = 0; i < items.size(); i++) {
				ITEM* item = items[i];
				if (RecOverlap(item->rec, player2->drawrec)) {
					player2->mp = fmin(100.0, player2->mp + 20);
					items.erase(items.begin() + i);
					i--;
					delete item;
				}
			}
		}

		if (small_monsters.empty() && big_monsters.empty())
			finish = true;
	}

	if (al_get_timer_count(death_timer1) == 5) {
	    player1->hp = 100;
		player1->mp = 0;
		al_stop_timer(death_timer1);
		al_set_timer_count(death_timer1, 0);
	}
	if (al_get_timer_count(death_timer2) == 5) {
	    player2->hp = 100;
		player2->mp = 0;
		al_stop_timer(death_timer2);
		al_set_timer_count(death_timer2, 0);
	}
}
void NORMAL_STAGE::draw() {
	if (state == DISPLAY_PLAYER) {
		player1->draw();
		if (IS_2P) player2->draw();
	}
	if (state == DISPLAY_MAP) {
		draw_map(map);
		player1->draw();
		if (IS_2P) player2->draw();
	}
	else if (state == DISPLAY_CRYSTAL) {
		draw_map(map);
		player1->draw();
		if (IS_2P) player2->draw();
		for (auto crystal : crystals)
			crystal->draw();
	}
	else if (state == DISPLAY_SMALL_MONSTER) {
		draw_map(map);
		player1->draw();
		if (IS_2P) player2->draw();
		for (auto monster : small_monsters)
			monster->draw();
		for (auto crystal : crystals)
			crystal->draw();
	}
	else if (state == DISPLAY_BIG_MONSTER) {
		draw_map(map);
		player1->draw();
		if (IS_2P) player2->draw();
		for (auto monster : small_monsters)
			monster->draw();
		for (auto monster : big_monsters)
			monster->draw();
		for (auto crystal : crystals)
			crystal->draw();
	}
	else if (state == DISPLAY_START) {
		draw_map(map);
		player1->draw();
		if (IS_2P) player2->draw();
		for (auto monster : small_monsters)
			monster->draw();
		for (auto monster : big_monsters)
			monster->draw();
		for (auto crystal : crystals)
			crystal->draw();
	}
	if (state == GAME_PROCEEDING) {
		draw_map(map);
		player1->draw();
		if (IS_2P) player2->draw();
		for (auto monster : small_monsters)
			monster->draw();
		for (auto monster : big_monsters)
			monster->draw();
		for (auto crystal : crystals)
			crystal->draw();
		for (auto ammo : ammos)
			ammo->draw();
		for (auto item : items)
			item->draw();
	}
}

void NORMAL_STAGE::on_key_change(void) {
	bool up1, down1, left1, right1;
	up1		= key_down(ALLEGRO_KEY_W);
	down1	= key_down(ALLEGRO_KEY_S);
	left1	= key_down(ALLEGRO_KEY_A);
	right1	= key_down(ALLEGRO_KEY_D);

	switch (up1 * 8 + down1 * 4 + left1 * 2 + right1 * 1) {
		case 0:	 // 0000
			player1->next_facing = NONE;
			break;
		case 1:	 // 0001
			player1->next_facing = RIGHT;
			break;
		case 2:	 // 0010
			player1->next_facing = LEFT;
			break;
		case 3:	 // 0011
			player1->next_facing = LEFT_RIGHT;
			break;
		case 4:  // 0100
			player1->next_facing = DOWN;
			break;
		case 5:	 // 0101
			player1->next_facing = DOWN_RIGHT;
			break;
		case 6:	 // 0110
			player1->next_facing = DOWN_LEFT;
			break;
		case 7:	 // 0111
			player1->next_facing = NONE;
			break;
		case 8:  // 1000
			player1->next_facing = UP;
			break;
		case 9:  // 1001
			player1->next_facing = UP_RIGHT;
			break;
		case 10: // 1010
			player1->next_facing = UP_LEFT;
			break;
		case 11: // 1011
			player1->next_facing = NONE;
			break;
		case 12: // 1100
			player1->next_facing = UP_DOWN;
			break;
		case 13: // 1101
			player1->next_facing = NONE;
			break;
		case 14: // 1110
			player1->next_facing = NONE;
			break;
		case 15: // 1111
			player1->next_facing = NONE;
			break;
		default:
			player1->next_facing = NONE;
			break;
	}

	if (IS_2P) {
		bool up2, down2, left2, right2;
		up2 = key_down(ALLEGRO_KEY_UP);
		down2 = key_down(ALLEGRO_KEY_DOWN);
		left2 = key_down(ALLEGRO_KEY_LEFT);
		right2 = key_down(ALLEGRO_KEY_RIGHT);

		switch (up2 * 8 + down2 * 4 + left2 * 2 + right2 * 1) {
			case 0:	 // 0000
				player2->next_facing = NONE;
				break;
			case 1:	 // 0001
				player2->next_facing = RIGHT;
				break;
			case 2:	 // 0010
				player2->next_facing = LEFT;
				break;
			case 3:	 // 0011
				player2->next_facing = LEFT_RIGHT;
				break;
			case 4:  // 0100
				player2->next_facing = DOWN;
				break;
			case 5:	 // 0101
				player2->next_facing = DOWN_RIGHT;
				break;
			case 6:	 // 0110
				player2->next_facing = DOWN_LEFT;
				break;
			case 7:	 // 0111
				player2->next_facing = NONE;
				break;
			case 8:  // 1000
				player2->next_facing = UP;
				break;
			case 9:  // 1001
				player2->next_facing = UP_RIGHT;
				break;
			case 10: // 1010
				player2->next_facing = UP_LEFT;
				break;
			case 11: // 1011
				player2->next_facing = NONE;
				break;
			case 12: // 1100
				player2->next_facing = UP_DOWN;
				break;
			case 13: // 1101
				player2->next_facing = NONE;
				break;
			case 14: // 1110
				player2->next_facing = NONE;
				break;
			case 15: // 1111
				player2->next_facing = NONE;
				break;
			default:
				break;
		}
	}
	bool bonus = (
		beatCNT % 16 == 14 ||
		beatCNT % 16 == 15 ||
		beatCNT % 16 == 0 ||
		beatCNT % 16 == 1 ||
		beatCNT % 16 == 2
		);
	if (key_down(ALLEGRO_KEY_SPACE)) {
		if (al_get_timer_count(death_timer1) == 0) {
			if (player1->mp >= 20) {
				int AMMO_NUM = bonus ? 3 : 6;
				float div_angle = 2 * pi / (float)AMMO_NUM;
				for (int i = 0; i < AMMO_NUM; i++) {
					AMMO* temp = AMMO::create(player1->drawrec.midx(), player1->drawrec.midy(), (float)i * div_angle, FIREBALL);
					ammos.emplace_back(temp);
				}
				bool in_range = false;
				for (auto crystal: crystals) 
					if (EuclideanDistanceSquare(player1->grid_x, player1->grid_y, crystal->grid_x, crystal->grid_y) < 4) 
						in_range = true;
				if (!in_range)
					player1->mp -= 20;
			}
			else {
				int AMMO_NUM = bonus ? 4 : 8;
				float div_angle = 2 * pi / (float)AMMO_NUM;
				for (int i = 0; i < AMMO_NUM; i++) {
					AMMO* temp = AMMO::create(player1->drawrec.midx(), player1->drawrec.midy(), (float)i * div_angle, SINWAVE);
					ammos.emplace_back(temp);
				}
			}
		}
	}
	if (key_down(ALLEGRO_KEY_ENTER)) {
		if (al_get_timer_count(death_timer2) == 0) {
			if (player2->mp >= 20) {
				int AMMO_NUM = bonus ? 3 : 6;
				float div_angle = 2 * pi / (float)AMMO_NUM;
				for (int i = 0; i < AMMO_NUM; i++) {
					AMMO* temp = AMMO::create(player2->drawrec.midx(), player2->drawrec.midy(), (float)i * div_angle, FIREBALL);
					ammos.emplace_back(temp);
				}
				bool in_range = false;
				for (auto crystal: crystals) 
					if (EuclideanDistanceSquare(player2->grid_x, player2->grid_y, crystal->grid_x, crystal->grid_y) < 4) 
						in_range = true;
				if (!in_range)
					player2->mp -= 20;
			}
			else {
				int AMMO_NUM = bonus ? 4 : 8;
				float div_angle = 2 * pi / (float)AMMO_NUM;
				for (int i = 0; i < AMMO_NUM; i++) {
					AMMO* temp = AMMO::create(player2->drawrec.midx(), player2->drawrec.midy(), (float)i * div_angle, SINWAVE);
					ammos.emplace_back(temp);
				}
			}
		}
	}
}
void NORMAL_STAGE::on_mouse_down(void) {
}
void NORMAL_STAGE::on_mouse_move(void) {
}
void NORMAL_STAGE::on_mouse_up(void) {
}
void NORMAL_STAGE::on_mouse_scroll(void) {
}
