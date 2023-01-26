#include "player.h"

PLAYER::PLAYER(int grid_x, int grid_y, int grid_w, int grid_h, const char* name, Map* map) {
    drawrec = rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);
    printRecInfo(&rec);
    next_facing = facing = NONE;
    next_speed = speed = 4;
    hp = 100.0;
    mp = 0.0;
    moveCNT = 0;
    element = 0;
    combo = 0;
    state = 1; // alive
    this->grid_x = grid_x;
    this->grid_y = grid_y;
    this->grid_w = grid_w;
    this->grid_h = grid_h;
    this->map = map;
    this->name = name;
    color = strcmp(name, "p1") ? al_map_rgb(234, 38, 38) : al_map_rgb(38, 38, 234);
    if (!strcmp(name, "p1")) {
        sprites[0] = load_bitmap("Assets/player_up.png");
        sprites[1] = load_bitmap("Assets/player_left.png");
        sprites[2] = load_bitmap("Assets/player_right.png");
        sprites[3] = load_bitmap("Assets/player_down.png");
        die_sprite = load_bitmap("Assets/player_die.png");
    }
    else {
        sprites[0] = load_bitmap("Assets/player2_up.png");
        sprites[1] = load_bitmap("Assets/player2_left.png");
        sprites[2] = load_bitmap("Assets/player2_right.png");
        sprites[3] = load_bitmap("Assets/player2_down.png");
        die_sprite = load_bitmap("Assets/player2_die.png");
    }
}

PLAYER::~PLAYER() {
    for (auto sprite: sprites)           
		al_destroy_bitmap(sprite);
    al_destroy_bitmap(die_sprite);
}

void PLAYER::draw() {
    /* Draw body */
    // al_draw_filled_circle(
    //     drawrec.midx(), drawrec.midy(), fmin(drawrec.w / 2, drawrec.h / 2),
    //     color
    // );

    // /* Draw eye */
    float dx = 0, dy = 0, shift = 10; // shift 10 px
    // switch (facing) {
    //     case NONE:
    //         break;
    //     case UP:
    //         dy = -1, dx = 0;
    //         break;
    //     case LEFT:
    //         dy = 0, dx = -1;
    //         break;
    //     case RIGHT:
    //         dy = 0, dx = 1;
    //         break;
    //     case DOWN:
    //         dy = 1, dx = 0;
    //         break;
    //     case UP_DOWN:
    //         break;
    //     case LEFT_RIGHT:
    //         break;
    //     case UP_LEFT:
    //         dy = -1, dx = -1;
    //         break;
    //     case DOWN_LEFT:
    //         dy = 1, dx = -1;
    //         break;
    //     case DOWN_RIGHT:
    //         dy = 1, dx = 1;
    //         break;
    //     case UP_RIGHT:
    //         dy = -1, dx = 1;
    //         break;
    //     default:
    //         break;
    // }
    // // Draw a smaller shifted circle representing direction
    // al_draw_filled_circle(
    //     drawrec.midx() + dx * shift, drawrec.midy() + dy * shift, fmin(drawrec.w / 8, drawrec.h / 8),
    //     al_map_rgb(0, 0, 0)
    // );


    int idx;
    switch(facing) {
        case NONE 		:idx = 3; break;
        case UP 		:idx = 0; break;
        case LEFT 		:idx = 1; break;
        case RIGHT 		:idx = 2; break;
        case DOWN 		:idx = 3; break;
        case UP_DOWN 	:idx = 3; break;
        case LEFT_RIGHT :idx = 3; break;
        case UP_LEFT 	:idx = 0; break;
        case DOWN_LEFT 	:idx = 3; break;
        case DOWN_RIGHT :idx = 3; break;
        case UP_RIGHT 	:idx = 0; break;
    }

    ALLEGRO_BITMAP* sprite;

    if (hp > 0) 
        sprite = sprites[idx];
    else 
        sprite = die_sprite;

    float sp_w = al_get_bitmap_width(sprite);
    float sp_h = al_get_bitmap_height(sprite);

    al_draw_scaled_bitmap(
        sprite,
        0, 0,
        sp_w, sp_h,
        drawrec.x, drawrec.y,
        drawrec.w, drawrec.h, 
        0
    );

    RECTANGLE hp_bar(drawrec.x, drawrec.y - shift, drawrec.w * (float)(fmax(hp, 0.0) / 100), shift / 2);
    RECTANGLE mp_bar(drawrec.x, drawrec.y - shift - 20, drawrec.w * (float)(fmax(mp, 0.0) / 100), shift / 2);

    /* Draw blood bar */
    al_draw_filled_rectangle(
        hp_bar.x, hp_bar.y, hp_bar.x + hp_bar.w, hp_bar.y + hp_bar.h, al_map_rgb(255, 255, 255)
    );
    al_draw_filled_rectangle(
        mp_bar.x, mp_bar.y, mp_bar.x + mp_bar.w, mp_bar.y + mp_bar.h, al_map_rgb(220, 220, 255)
    );
}

void PLAYER::update() {
    /* Update the position */
    bool _movable = movable(grid_x, grid_y, grid_w, grid_h, map, next_facing);
    if (is_time_to_move(speed)) {
        moveCNT = 0;
        // If next grid point is legal, update speed, facing, and position
        if (_movable) {
            switch (next_facing) {
            case NONE:
                break;
            case UP:
                grid_y -= 1;
                break;
            case LEFT:
                grid_x -= 1;
                break;
            case RIGHT:
                grid_x += 1;
                break;
            case DOWN:
                grid_y += 1;
                break;
            case UP_DOWN:
                break;
            case LEFT_RIGHT:
                break;
            case UP_LEFT:
                grid_y -= 1; grid_x -= 1;
                break;
            case DOWN_LEFT:
                grid_y += 1; grid_x -= 1;
                break;
            case DOWN_RIGHT:
                grid_y += 1; grid_x += 1;
                break;
            case UP_RIGHT:
                grid_y -= 1; grid_x += 1;
                break;
            default:
                break;
            }
            speed = next_speed;
            facing = next_facing;
        }
    }
    else {
        if (_movable)
            moveCNT = min(moveCNT + speed, (int)GAME_TICK_CD);
    }
    // Update rectangle
    rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);

    // Update drawrec
    drawrec = getDrawArea(rec, moveCNT, facing, GAME_TICK_CD);

    // Check the status of player
    if (hp <= 0.0) {
        hp = 0.0;
        mp = 0;
        color = al_map_rgb(50, 50, 50);
    }
}