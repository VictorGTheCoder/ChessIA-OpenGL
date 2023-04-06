#include "../../includes/header.h"

extern t_game game;

static void create_case(t_gui *gui)
{
	gui->case_list = malloc(64 * sizeof(t_case));
	t_case  *current_case;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			current_case = &gui->case_list[y * 8 + x];

			current_case->startX = (int) x * (BOARD_SIZE / 8);
			current_case->startY = (int) y * (BOARD_SIZE / 8);
			current_case->endX = (int) x * (BOARD_SIZE / 8 + 1);
			current_case->endY = (int) y * (BOARD_SIZE / 8 + 1);

			current_case->case_n = (y > 0) ? &gui->case_list[(y - 1) * 8 +  x] : NULL;
			current_case->case_s = (x < 7) ? &gui->case_list[(y + 1) * 8 + x] : NULL;
			current_case->case_w = (x > 0) ? &gui->case_list[(y - 1) + 8 * x] : NULL;
			current_case->case_e = (x < 7) ? &gui->case_list[(y + 1) + 8 * x] : NULL; 
		}
	}
}

GLuint	load_texture(const char* file)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);

    if (image == NULL)
    {
        printf("Failed to load image: %s\n", SOIL_last_result());
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    printf("Loaded texture %s with ID %u\n", file, textureID);
    return textureID;
}

void	load_textures(t_gui *gui)
{
    gui->pieces.white_rook = load_texture("images/white_rook.png");
	gui->pieces.white_knight = load_texture("images/white_knight.png");
	gui->pieces.white_bishop = load_texture("images/white_bishop.png");
	gui->pieces.white_queen = load_texture("images/white_queen.png");
	gui->pieces.white_king = load_texture("images/white_king.png");
	gui->pieces.white_pawn = load_texture("images/white_pawn.png");

	gui->pieces.black_rook = load_texture("images/black_rook.png");
	gui->pieces.black_knight = load_texture("images/black_knight.png");
	gui->pieces.black_bishop = load_texture("images/black_bishop.png");
	gui->pieces.black_queen = load_texture("images/black_queen.png");
	gui->pieces.black_king = load_texture("images/black_king.png");
	gui->pieces.black_pawn = load_texture("images/black_pawn.png");
}

void	init_game(void)
{
	game.black_can_castle_king_side = 1;
	game.black_can_castle_queen_side = 1;
	game.white_can_castle_king_side = 1;
	game.white_can_castle_queen_side = 1;
	game.white_is_check = 0;
	game.black_is_check = 0;
	game.white_to_play = 1;
	game.en_passant_target = NULL;
}

void	init_gui(t_gui *gui)
{
	gui->case_list = NULL;
	gui->img = 0;
	gui->win = NULL;
	gui->mlx = NULL;
	gui->bits_per_pixel = 0;
	gui->addr = NULL;
	gui->square_selected = NULL;
	create_case(gui);

}

