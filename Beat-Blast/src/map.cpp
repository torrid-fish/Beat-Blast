#include "game_window.h"
#include "map.h"

extern uint32_t GAME_TICK;
extern uint32_t SLOWER_GAME_TICK;

float dx, dy;

const char* default_map[] = {
	"####################################",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#          m      m                #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#               P                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"####################################",
};


Map* create_map(const char * filepath) {
	Map* M = new Map;
	FILE* pFile = NULL;
	if (!M) {
		game_abort("Error when creating Map");
		return NULL;
	}
	if (filepath == NULL) {
		M->row_num = 30;
		M->col_num = 36;
		game_log("Creating from default map. row = %d col = %d", M->row_num, M->col_num);
	}
	else {
		game_log("%s\n", filepath);
		pFile = fopen(filepath,"r");
		if (!pFile) {
			game_abort("error to open map file\n");
			return NULL;
		}
		if(fscanf(pFile,"%d%d",&M->row_num,&M->col_num) != 2) {
			game_abort("Map format unmatched\n");
			return NULL;
		}
		getc(pFile); // get the '\n'
	}

	M->map = new char*[M->row_num];
	if (!M->map) {
		game_abort("map char array malloc error\n");
		return NULL;
	}
	for (int i = 0; i < M->row_num; i++) {
		M->map[i] = new char[M->col_num];
		if(!M->map[i]){
			game_abort("map char array malloc error\n");
			return NULL;
		}
	}

	M->wallnum = 0;

	for (int i = 0; i < M->row_num; i++) {
		for (int j = 0; j < M->col_num; j++) {
			if (filepath == NULL)
			M->map[i][j] = default_map[i][j]; 
			else
				fscanf(pFile,"%c", &(M->map[i][j]));
			switch(M->map[i][j]) {
			case '#':
				M->wallnum++;
				break;
			case '.':
				break;
			default:
				break;
			}
		}
		if(filepath != NULL)
			getc(pFile);
	}
	return M;
}

void delete_map(Map* M) {
	if (!M)
		return;
	if (M->map) for (int i = 0; i < M->row_num; i++) free(M->map[i]);		
	free(M->map);
	free(M);
}

void draw_map(Map* M) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	if (M == NULL) {
		game_abort("error map!\n");
		return;
	}
	for (int row = 0; row < M->row_num; row++) {
		for (int col = 0; col < M->col_num; col++) {
			switch (M->map[row][col]) {
				case '#':
					draw_wall_index(M, row, col);
					break;
				case 'X':
					draw_wall_index(M, row, col);
					break;
				case 'E':
					draw_exit_index(M, row, col);
					break;
				case '_':
					draw_board_index(M, row, col);
					break;
				default:
					draw_ground_index(M, row, col);
					break;
			}
		}
	}
}

static void draw_wall_index(Map* M, const int row, const int col) {
    int block_x = map_offset_x + block_width * col;
    int block_y = map_offset_y + block_height * row;
	
	float sp_w = al_get_bitmap_width(stone_brick);
    float sp_h = al_get_bitmap_height(stone_brick);

    al_draw_scaled_bitmap(
        stone_brick,
        0, 0,
        sp_w, sp_h,
        block_x, block_y,
        block_width, block_height,
        0
    );

    // al_draw_filled_rectangle(s_x, s_y, e_x, e_y, al_map_rgb(40, 40, 40));
}

extern bool finish;

static void draw_exit_index(Map* M, const int row, const int col) {
    // int s_x, s_y, e_x, e_y, dw;
    // int block_x = map_offset_x + block_width * col;
    // int block_y = map_offset_y + block_height * row;

    // dw = block_width;
    // s_x = block_x;
    // s_y = block_y;
    // e_x = s_x + dw;
    // e_y = s_y + dw;
	// if (finish)
    // 	al_draw_filled_rectangle(s_x, s_y, e_x, e_y, al_map_rgb(200, 200, 100));
	// else
    // 	al_draw_filled_rectangle(s_x, s_y, e_x, e_y, al_map_rgb(40, 40, 40));
	int block_x = map_offset_x + block_width * col;
	int block_y = map_offset_y + block_height * row;
	if (finish) {
		float sp_w = al_get_bitmap_width(exit_block);
		float sp_h = al_get_bitmap_height(exit_block);

		al_draw_scaled_bitmap(
			exit_block,
			0, 0,
			sp_w, sp_h,
			block_x, block_y,
			block_width, block_height,
			0
		);
	}
	else {
		float sp_w = al_get_bitmap_width(stone_brick);
		float sp_h = al_get_bitmap_height(stone_brick);

		al_draw_scaled_bitmap(
			stone_brick,
			0, 0,
			sp_w, sp_h,
			block_x, block_y,
			block_width, block_height,
			0
		);
	}
}

static void draw_ground_index(Map* M, const int row, const int col) {
    // int s_x, s_y, e_x, e_y, dw;
    // int block_x = map_offset_x + block_width * col;
    // int block_y = map_offset_y + block_height * row;

    // dw = block_width;
    // s_x = block_x;
    // s_y = block_y;
    // e_x = s_x + dw;
    // e_y = s_y + dw;

    // al_draw_filled_rectangle(s_x, s_y, e_x, e_y, al_map_rgb(70, 70, 70));
	int block_x = map_offset_x + block_width * col;
    int block_y = map_offset_y + block_height * row;
	
	float sp_w = al_get_bitmap_width(stone_ground);
    float sp_h = al_get_bitmap_height(stone_ground);

    al_draw_scaled_bitmap(
        stone_ground,
        0, 0,
        sp_w, sp_h,
        block_x, block_y,
        block_width, block_height,
        0
    );
}


static void draw_board_index(Map* M, const int row, const int col) {
    int s_x, s_y, e_x, e_y, dw;
    int block_x = map_offset_x + block_width * col;
    int block_y = map_offset_y + block_height * row;

    dw = block_width;
    s_x = block_x;
    s_y = block_y;
    e_x = s_x + dw;
    e_y = s_y + dw;

    al_draw_filled_rectangle(s_x, s_y, e_x, e_y, al_map_rgb(0, 0, 0));
}