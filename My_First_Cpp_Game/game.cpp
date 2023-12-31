#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_x = 0.f;
float player_pos_y = 0.f;

internal void
simulate_game(Input* input, float dt) {
	clear_screen(0xff5500);

	float speed = 50.f;
	if (is_down(BUTTON_UP)) player_pos_y += speed * dt;
	if (is_down(BUTTON_DOWN)) player_pos_y -= speed * dt;
	if (is_down(BUTTON_LEFT)) player_pos_x -= speed * dt;
	if (is_down(BUTTON_RIGHT)) player_pos_x += speed * dt;
	
	draw_rect(player_pos_x, player_pos_y, 1, 1, 0x2E8B57);
	draw_rect(30, 30, 5, 5, 0x008f39);
	draw_rect(-20, 20, 8, 3, 0x32CD32);
}