#include "small_monster.h"

SMALL_MONSTER::SMALL_MONSTER(int grid_x, int grid_y, const char* name, Map* map) {
    drawrec = rec = get_rec_by_pt_on_board(grid_x, grid_y, 1, 1);
    next_facing = facing = UP;
    next_speed = speed = 8;
    hp = 15.0;
    mp = 100.0;
    moveCNT = 0;
    element = 0;
    state = 1; // alive
    this->grid_x = grid_x;
    this->grid_y = grid_y;
    this->grid_w = 1;
    this->grid_h = 1;
    this->map = map;
    this->name = name;
    color = al_map_rgb(38, 234, 38);

    sprite = load_bitmap("Assets/small_monster.png");
}

SMALL_MONSTER::SMALL_MONSTER(const char* _name, Map* map) {
    pair<int, int> spawn_grid_pt = generateRandomSpawnGridPoint(map, 1, 1);
    drawrec = rec = get_rec_by_pt_on_board(spawn_grid_pt.first, spawn_grid_pt.second, 1, 1);
    next_facing = facing = UP;
    next_speed = speed = 8;
    hp = 100.0;
    mp = 100.0;
    moveCNT = 0;
    element = 0;
    state = 1; // alive
    this->grid_x = spawn_grid_pt.first;
    this->grid_y = spawn_grid_pt.second;
    this->grid_w = 1;
    this->grid_h = 1;
    this->map = map;
    name = _name;
    color = al_map_rgb(38, 234, 38);

    sprite = load_bitmap("Assets/big_monster.png");
}

SMALL_MONSTER::~SMALL_MONSTER() {
    al_destroy_bitmap(sprite);
}

void SMALL_MONSTER::draw() {
    /* Draw body */
    // al_draw_filled_circle(
    //     drawrec.midx(), drawrec.midy(), fmin(drawrec.w / 2, drawrec.h / 2),
    //     color
    // );

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

    float shift = 10;

    RECTANGLE hp_bar(drawrec.x, drawrec.y - shift, drawrec.w * (float)(fmax(hp, 0.0) / 100), shift / 2);

    /* Draw blood bar */
    al_draw_filled_rectangle(
        hp_bar.x, hp_bar.y, hp_bar.x + hp_bar.w, hp_bar.y + hp_bar.h, al_map_rgb(255, 255, 255)
    );
}

void SMALL_MONSTER::update() {
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
            next_facing = facing = bernoulliTrail(0.04) ? generatRandomDirection() : next_facing;
        }
    }
    else {
        if (_movable)
            moveCNT = min(moveCNT + speed, (int)GAME_TICK_CD);
        else {
            switch (facing) {
            case NONE:
                next_facing = NONE;
                break;
            case UP:
                next_facing = DOWN;
                break;
            case LEFT:
                next_facing = RIGHT;
                break;
            case RIGHT:
                next_facing = LEFT;
                break;
            case DOWN:
                next_facing = UP;
                break;
            case UP_DOWN:
                next_facing = UP_DOWN;
                break;
            case LEFT_RIGHT:
                next_facing = LEFT_RIGHT;
                break;
            case UP_LEFT:
                next_facing = DOWN_RIGHT;
                break;
            case DOWN_LEFT:
                next_facing = UP_RIGHT;
                break;
            case DOWN_RIGHT:
                next_facing = UP_LEFT;
                break;
            case UP_RIGHT:
                next_facing = DOWN_LEFT;
                break;
            default:
                next_facing = NONE;
                break;
            }
            speed = next_speed;
            facing = next_facing;
        }
    }
    // Update rectangle
    rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);

    // Update drawrec
    drawrec = getDrawArea(rec, moveCNT, facing, GAME_TICK_CD);

    // Check the status of player
    if (hp <= 0.0) {
        hp = 0.0;
        color = al_map_rgb(50, 50, 50);
    }
}