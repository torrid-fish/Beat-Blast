#include "player.h"
#include <math.h>

PLAYER::PLAYER(int grid_x, int grid_y, int grid_w, int grid_h, const char* _name, Map* map) {
    rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);
    printRecInfo(&rec);
    next_facing = facing = NONE;
    next_speed = speed = 8;
    hp = 100;
    mp = 100;
    moveCD = GAME_TICK_CD;
    element = 0;
    combo = 0;
    state = 1; // alive
    this->grid_x = grid_x;
    this->grid_y = grid_y;
    this->grid_w = grid_w;
    this->grid_h = grid_h;
    this->map = map;
    name = _name;
}

PLAYER::~PLAYER() {
    // destroy bitmap
}

void PLAYER::draw() {
    RECTANGLE drawrec = getDrawArea(rec, moveCD, facing, GAME_TICK_CD);

    // First draw a circle
    al_draw_filled_circle(
        drawrec.midx(), drawrec.midy(), fmin(drawrec.w / 2, drawrec.h / 2),
        strcmp(name, "p1") ? al_map_rgb(234, 38, 38) : al_map_rgb(38, 38, 234)
    );
    float dx = 0, dy = 0, shift = 10; // shift 10 px
    switch (facing) {
        case NONE:
            break;
        case UP:
            dy = -1, dx = 0;
            break;
        case LEFT:
            dy = 0, dx = -1;
            break;
        case RIGHT:
            dy = 0, dx = 1;
            break;
        case DOWN:
            dy = 1, dx = 0;
            break;
        case UP_DOWN:
            break;
        case LEFT_RIGHT:
            break;
        case UP_LEFT:
            dy = -1, dx = -1;
            break;
        case DOWN_LEFT:
            dy = 1, dx = -1;
            break;
        case DOWN_RIGHT:
            dy = 1, dx = 1;
            break;
        case UP_RIGHT:
            dy = -1, dx = 1;
            break;
        default:
            break;
    }
    // Draw a smaller shifted circle representing direction
    al_draw_filled_circle(
        drawrec.midx() + dx * shift, drawrec.midy() + dy * shift, fmin(drawrec.w / 8, drawrec.h / 8),
        al_map_rgb(0, 0, 0)
    );
}

void PLAYER::update() {
    bool _movable = movable(grid_x, grid_y, grid_w, grid_h, map, next_facing);
    if (is_time_to_move(speed)) {
        moveCD = 0;
        // If next grid point is legal, update speed, facing, and position
        if (_movable) {
            game_log("Check_movable done");
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
            moveCD = min(moveCD + speed, (int)GAME_TICK_CD);
    }
    rec = get_rec_by_pt_on_board(grid_x, grid_y, grid_w, grid_h);
    game_log("%d %d", grid_x, grid_y);
}