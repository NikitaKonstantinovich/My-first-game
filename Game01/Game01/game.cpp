#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

float playerPositionX = 0.f;
float playerPositionY = 0.f;

internalVariable void simulateGame(Input* input) {
	clearScreen(0xff9900);
	if (pressed(BUTTON_UP)) playerPositionY += 1.f;
	if (pressed(BUTTON_DOWN)) playerPositionY -= 1.f;
	if (pressed(BUTTON_LEFT)) playerPositionX -= 1.f;
	if (pressed(BUTTON_RIGHT)) playerPositionX += 1.f;

	drawRectangle(playerPositionX, playerPositionY, 1, 1, 0x00ff22);
	drawRectangle(30 + playerPositionX, 30 + playerPositionY, 5, 5, 0xff0022);
	drawRectangle(-20 + playerPositionX, 20 + playerPositionY, 8, 3, 0x5500ff);
	drawRectangle(-10 + playerPositionX, -15 + playerPositionY, 8, 8, 0xff00ff);
	drawRectangle(10 + playerPositionX, -20 + playerPositionY, 8, 7, 0x00ff55);

	//E
	drawRectangle(-26 + playerPositionX, -30 + playerPositionY, 1, 5, 0xffff22);
	drawRectangle(-28 + playerPositionX, -30 + playerPositionY, 3, 1, 0xffff22);
	drawRectangle(-28 + playerPositionX, -26 + playerPositionY, 3, 1, 0xffff22);
	drawRectangle(-28 + playerPositionX, -34 + playerPositionY, 3, 1, 0xffff22);

	//oO
	drawRectangle(40 + playerPositionX, 40 + playerPositionY, 1, 5, 0x55ff22);
	drawRectangle(42 + playerPositionX, 36 + playerPositionY, 3, 1, 0x55ff22);
	drawRectangle(44 + playerPositionX, 40 + playerPositionY, 1, 5, 0x55ff22);
	drawRectangle(42 + playerPositionX, 44 + playerPositionY, 3, 1, 0x55ff22);
}