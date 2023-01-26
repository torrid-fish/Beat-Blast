#include"ammo.h"

AMMO::AMMO(float mid_x, float mid_y, float angle, PATTERN pattern) {
	timeCNT = 0;
	RECTANGLE temp;
	switch (pattern) {
		case FIREBALL:
			temp.x = mid_x - 5;
			temp.y = mid_y - 5;
			temp.h = temp.w = 10;
			color = al_map_rgb(255, 50, 50);
			break;
		default:
			temp.x = mid_x - 3;
			temp.y = mid_y - 3;
			temp.h = temp.w = 6;
			color = al_map_rgb(255, 255, 255);
			break;
	}
	drawrec = temp;
	this->angle = angle;
	fix_x = mid_x, fix_y = mid_y;
	this->pattern = pattern;

	sprites[LINEAR] = load_bitmap("Assets/small_ammo.png");
	sprites[SINWAVE] = load_bitmap("Assets/small_ammo.png");
	sprites[FIREBALL] = load_bitmap("Assets/fire_ball.png");
}


AMMO::~AMMO() {
	for (auto sprite: sprites)
		al_destroy_bitmap(sprite);
}


AMMO* AMMO::create(float mid_x, float mid_y, float angle, PATTERN pattern) {
	AMMO* temp = new AMMO(mid_x, mid_y, angle, pattern);
	return temp;
}


void AMMO::draw() {
	// Draw the ammo with damping shift
	float damping_x = map_offset_x - fix_map_offset_x, damping_y = map_offset_y - fix_map_offset_y;
	float sp_w = al_get_bitmap_width(sprites[pattern]);
	float sp_h = al_get_bitmap_height(sprites[pattern]);

	al_draw_scaled_bitmap(
		sprites[pattern],
		0, 0,
		sp_w, sp_h,
		drawrec.x, drawrec.y,
		drawrec.w, drawrec.h, 
		ALLEGRO_ALIGN_CENTER
	);
}


void AMMO::update() {
	// Update time counter
	timeCNT++;

	// Use specific pattern to utilize tragetory function update shift
	pair <float, float> shift;

	switch (pattern) {
		case LINEAR:
			shift = LINEAR_tragetory(timeCNT);
			break;
		case SINWAVE:
			shift = SINWAVE_tragetory(timeCNT);
			break;
		case FIREBALL:
			if (timeCNT >= 64) {
				if (timeCNT == 64) {
					float midx = drawrec.midx(), midy = drawrec.midy();
					drawrec.h = drawrec.w = 30;
					drawrec.x = drawrec.midx() - drawrec.w / 2;
					drawrec.y = drawrec.midy() - drawrec.h / 2;
				}
				shift = SINWAVE_tragetory(64);
			}
			else 
				shift = SINWAVE_tragetory(timeCNT);
			break;
		// case LINEAR:
		// 	mid = LINEAR_
		// 	break;
		// case LINEAR:
		// 	mid = LINEAR_
		// 	break;
		default:
			shift = make_pair(0, 0);
			break;
	}

	// Rotation after applying tragetory function
	shift = Rotation2D(shift.first, shift.second, angle);

	// Change mid point back to left up point to update rectangle
	drawrec.x = shift.first + fix_x - drawrec.w / 2;
	drawrec.y = shift.second + fix_y - drawrec.h / 2;
}

pair<float, float> LINEAR_tragetory(int t) {
	return make_pair((float)t, 0);
}

pair<float, float> SINWAVE_tragetory(int t) {
	return make_pair((float)t, 10 * sin((float)t * 0.62 / pi));
}

pair<float, float> FIREBALL_tragetory(int t) {
	return make_pair((float)t / 2, 0);
}