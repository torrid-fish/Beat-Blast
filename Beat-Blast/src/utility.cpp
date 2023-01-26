#include "utility.h"
#include "game_window.h"
int GridSize = 22;
int block_width = 21,  block_height = 21;			// the pixel size of a "block"
int fix_map_offset_x = 25, fix_map_offset_y = 50;	// fix value
int map_offset_x = 25, map_offset_y = 50;			// this value might be changed by damping
float VOLUME = 1.0;
ALLEGRO_SAMPLE* load_audio(const char* filename) {
	ALLEGRO_SAMPLE* sample = al_load_sample(filename);
	if (!sample)
		game_abort("failed to load audio: %s", filename);
	else
		game_log("loaded audio: %s", filename);
	return sample;
}

ALLEGRO_SAMPLE_ID play_audio(ALLEGRO_SAMPLE* sample, float volume) {
	ALLEGRO_SAMPLE_ID id;
	if (!al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &id))
		game_abort("failed to play audio (once)");
	return id;
}

ALLEGRO_SAMPLE_ID play_bgm(ALLEGRO_SAMPLE* sample, float volume) {
	ALLEGRO_SAMPLE_ID id;
	if (!al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id))
		game_abort("failed to play audio (bgm)");
	return id;
}

void stop_bgm(ALLEGRO_SAMPLE_ID sample) {
	al_stop_sample(&sample);
}

ALLEGRO_FONT* load_font(const char* filename, int size) {
	ALLEGRO_FONT* font = al_load_font(filename, size, 0);
	if (!font)
		game_abort("failed to load font: %s with size %d", filename, size);
	else
		game_log("loaded font: %s with size %d", filename, size);
	return font;
}

ALLEGRO_BITMAP* load_bitmap(const char* filename) {
	ALLEGRO_BITMAP* bmp = al_load_bitmap(filename);
	if (!bmp)
		game_abort("failed to load image: %s", filename);
	else
		game_log("loaded image: %s", filename);
	return bmp;
}

ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = load_bitmap(filename);
	ALLEGRO_BITMAP* resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
    al_get_bitmap_width(loaded_bmp),
    al_get_bitmap_height(loaded_bmp),
    0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	game_log("resized image: %s", filename);

	return resized_bmp;
}

bool pt_in_rect(int px, int py, RECTANGLE field) {
	if (field.x <= px && field.x + field.w >= px && field.y <= py && field.y + field.h >= py) return 1;
	else return 0;
}

bool RecOverlap(const RECTANGLE RA,const RECTANGLE RB) {
	float RA_x2 = RA.x + RA.w;
	float RA_y2 = RA.y + RA.h;
	float RB_x2 = RB.x + RB.w;
	float RB_y2 = RB.y + RB.h;
	if ((fmin(RA_x2, RB_x2) > fmax(RA.x, RB.x)) && (fmin(RA_y2, RB_y2) > fmax(RA.y, RB.y)))
		return true;
	return false;
}
RECTANGLE get_rec_by_pt_on_board(int grid_x, int grid_y, int grid_w, int grid_h) {
	float x = map_offset_x + grid_x * block_width;
	float y = map_offset_y + grid_y * block_height;
	float w = block_width * grid_w;
	float h = block_height * grid_h;
	RECTANGLE generator(x, y, w, h);
	return generator;
}

RECTANGLE getDrawArea(RECTANGLE rec, int moveCD, Directions facing, uint32_t TOTAL_TICK) {
	RECTANGLE target = rec;

	switch (facing) {
		case UP:
			target.y -= moveCD * block_width / TOTAL_TICK;
			break;
		case DOWN:
			target.y += moveCD * block_width / TOTAL_TICK;
			break;
		case LEFT:
			target.x -= moveCD * block_width / TOTAL_TICK;
			break;
		case RIGHT:
			target.x += moveCD * block_width / TOTAL_TICK;
			break;
		case UP_DOWN:
			break;
		case LEFT_RIGHT:
			break;
		case UP_LEFT:
			target.y -= moveCD * block_width / TOTAL_TICK;
			target.x -= moveCD * block_width / TOTAL_TICK;
			break;
		case DOWN_LEFT:
			target.y += moveCD * block_width / TOTAL_TICK;
			target.x -= moveCD * block_width / TOTAL_TICK;
			break;
		case DOWN_RIGHT:
			target.y += moveCD * block_width / TOTAL_TICK;
			target.x += moveCD * block_width / TOTAL_TICK;
			break;
		case UP_RIGHT:
			target.y -= moveCD * block_width / TOTAL_TICK;
			target.x += moveCD * block_width / TOTAL_TICK;
			break;
		case NONE:
			break;
		default:
			break;
	}

	return target;	
}

float dampedOscillation(float A0, float time, float angular_freq, float alpha) {
	return A0 * exp(-alpha * time) * cos(angular_freq * time);
}

void printRecInfo(const RECTANGLE* RA) {
	game_log("RecArea info: \nx: %f, y: %f, w: %f, h: %f\n",
		RA->x, RA->y, RA->w, RA->h);
}
void printDirection(const Directions a) {
	switch (a)
	{
	case NONE:
		game_log("NONE");
		break;
	case UP:
		game_log("UP");
		break;
	case DOWN:
		game_log("DOWN");
		break;
	case LEFT:
		game_log("LEFT");
		break;
	case RIGHT:
		game_log("RIGHT");
		break;
	default:
		game_log("direction error");
		break;
	}
}

bool is_time_to_move(int speed) {
	return GAME_TICK % (GAME_TICK_CD / speed) == 0;
}

uint32_t generateRandomNumber(uint32_t a, uint32_t b) {
	if (b < a) 
		game_abort("Error in RandomNumber, b is less than a");
	return rand() % (b - a + 1) + a;
}

double generateRandomFloat() {
	return (double)rand() / RAND_MAX;
}

bool bernoulliTrail(double p) {
	if (p >= 1 || p <= 0) 
		game_abort("Error range of p = %lf in BernoulliTrail func\n But p should be between 0.0 and 1.0", p);
	return generateRandomFloat() < p;
}

pair<float, float> Rotation2D(float x, float y, float angle) {
	return make_pair(cos(angle) * x - sin(angle) * y, sin(angle) * x + cos(angle) * y);
}

int ChebyshevDistance(int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end) {
	return 
		max(
			abs(grid_x_start - grid_x_end),
			abs(grid_y_start - grid_y_end)
		);
}

int EuclideanDistanceSquare(int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end) {
	return (grid_x_start - grid_x_end) * (grid_x_start - grid_x_end) + (grid_y_start - grid_y_end) * (grid_y_start - grid_y_end);
}

Directions A_star(Map* M, int grid_w, int grid_h, int grid_x_start, int grid_y_start, int grid_x_end, int grid_y_end) {
	using T = tuple<int, int, int>; // T: (f(n), x, y)
	int row = M->row_num, col = M->col_num;
	int dir[8][2] = {
	{ 0,-1},	// UP
	{-1, 0},	// LEFT
	{-1, -1},	// UP_LEFT
	{1, -1},	// UP_RIGHT
	{-1, 1},	// DOWN_LEFT
	{1, 1},		// DOWN_RIGHT
	{ 1, 0},	// RIGHT
	{ 0, 1},	// DOWN 
	};
	Directions direction[8] = {
		UP,
		LEFT,
		UP_LEFT,
		UP_RIGHT,
		DOWN_LEFT,
		DOWN_RIGHT,
		RIGHT,
		DOWN
	};
	priority_queue<T, vector<T>, greater<T>> todo;
	vector<vector<bool>> vis(row, vector<bool>(col, false));
	vector<vector<int>> dis(row, vector<int>(col, 0));
	// Search from end to start then return direction
	vis[grid_y_end][grid_x_end] = true;
	todo.emplace(0, grid_x_end, grid_y_end);
	// A* algorithm
	Directions ans;
	while (!todo.empty()) {
		int f, x, y;
		tie(f, x, y) = todo.top(); todo.pop();
		// Search
		for (int i = 0; i < 8; i++) {
			if (movable(x, y, grid_w, grid_h, M, direction[i])) {
				int next_x = x + dir[i][0];
				int next_y = y + dir[i][1];
				if (!vis[next_y][next_x]) {
					if (next_x == grid_x_start && next_y == grid_y_start) {
						// Find first dirction from end to start, we reverse it
						return direction[7 - i];
					}
					dis[next_y][next_x] = dis[y][x] + 1;
					vis[next_y][next_x] = true;
					// f(n) = g(n) + h(n)
					int new_f = dis[next_y][next_x];//ChebyshevDistance(next_x, next_y, grid_x_start, grid_y_start);
					todo.emplace(new_f, next_x, next_y);
				}
			}
		}
	}
	// There is no such path
	return generatRandomDirection();
}

Directions generatRandomDirection(void) {
	switch (generateRandomNumber(0, 8)) {
	case 0:		return UP;
	case 1:		return LEFT;
	case 2:		return RIGHT;
	case 3:		return DOWN;
	case 4:		return UP_LEFT;
	case 5:		return DOWN_LEFT;
	case 6:		return DOWN_RIGHT;
	case 7:		return UP_RIGHT;
	}
}

pair<int, int> generateRandomSpawnGridPoint(Map* map, int grid_w, int grid_h) {
	int grid_x, grid_y;
	while (true) {
		grid_x = generateRandomNumber(0, map->col_num);
		grid_y = generateRandomNumber(0, map->row_num);
		if (movable(grid_x, grid_y, grid_w, grid_h, map, NONE) &&
			!(is_wall(map, grid_x, grid_y) ||
			is_exit(map, grid_x, grid_y) ||
			is_outside(map, grid_x, grid_y) ||
			is_boss(map, grid_x, grid_y) ||
			is_small_monster(map, grid_x, grid_y) ||
			is_big_monster(map, grid_x, grid_y) ||
			is_crystal(map, grid_x, grid_y) ||
			is_p1(map, grid_x, grid_y) ||
			is_p2(map, grid_x, grid_y)))
			break;
	}
	return make_pair(grid_x, grid_y);
}

extern bool finish;

bool movable(int grid_x, int grid_y, int grid_w, int grid_h, Map* M, Directions facing) {
	switch (facing) {
		case UP:
			grid_y--;
			break;
		case DOWN:
			grid_y++;
			break;
		case LEFT:
			grid_x--;
			break;
		case RIGHT:
			grid_x++;
			break;
		case UP_DOWN:
			break;
		case LEFT_RIGHT:
			break;
		case UP_LEFT:
			grid_y--, grid_x--;
			break;
		case DOWN_LEFT:
			grid_y++, grid_x--;
			break;
		case DOWN_RIGHT:
			grid_y++, grid_x++;
			break;
		case UP_RIGHT:
			grid_y--, grid_x++;
			break;
		default:
			break;
	}
	bool res = true;
	int row = M->row_num, col = M->col_num;
	for (int i = 0; i < grid_w; i++)
		for (int j = 0; j < grid_h; j++)
			res &= !(is_wall(M, grid_x + i, grid_y + j) || is_outside(M, grid_x + i, grid_y + j) || is_exit(M, grid_x + i, grid_y + j) && !finish || grid_x + i < 0 || grid_x + i >= col || grid_y + j < 0 || grid_y + j >= row);
	return res;
}

bool is_wall(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return true;
	return M->map[index_y][index_x] == '#' || M->map[index_y][index_x] == 'X'; // X is inside wall, # is margin
}
bool is_exit(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'E';
}
bool is_boss(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'B';
}
bool is_outside(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == '_';
}
bool is_small_monster(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'm';
}
bool is_big_monster(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'M';
}
bool is_crystal(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'O';
}
bool is_p1(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'P';
}
bool is_p2(Map* M, int index_x, int index_y) {
	if (index_x < 0 || index_x >= M->col_num || index_y < 0 || index_y >= M->row_num)
		return false;
	return M->map[index_y][index_x] == 'p';
}

bool key_down(int allegro_key) {
	return al_key_down(&key_state, allegro_key);
}

// Log using va_list.
static void game_vlog(const char* format, va_list arg);

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

static void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}