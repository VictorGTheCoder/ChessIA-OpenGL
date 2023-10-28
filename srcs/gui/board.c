#include "../../includes/header.h"

void draw_chess_piece(GLuint texture, int x, int y, int case_size)
{
	

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(x, BOARD_SIZE - y - 100);
    glTexCoord2f(1, 1); glVertex2i(x + case_size, BOARD_SIZE - y - 100);
    glTexCoord2f(1, 0); glVertex2i(x + case_size, BOARD_SIZE - y - 100 + case_size);
    glTexCoord2f(0, 0); glVertex2i(x, BOARD_SIZE - y - 100 + case_size);
    glEnd();

    glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void draw_pieces_from_case_list(t_gui *gui)
{
    int case_size = BOARD_SIZE / 8;
    for (int i = 0; i < 64; i++)
    {
        t_case *square = &gui->case_list[i];

        if (square->status != EMPTY)
        {
            draw_chess_piece(square->square_img, square->startX, square->startY, case_size);

            if (game->is_piece_selected && gui->square_selected == square)
            {
                draw_transparent_square(square->startX, square->startY, case_size);
            }
        }
    }
}

void display_board(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //printf("Create Board\n");
    int case_size = BOARD_SIZE / 8;
	int f = 0;

    for (int x = 0; x <= BOARD_SIZE; x += case_size)
    {
        for (int y = 0; y <= BOARD_SIZE; y += case_size)
        {
            if (f)
            {
				f = 0;
                glColor3f(0.5, 0.4, 0.3);
            }
            else
            {
				f = 1;
                glColor3f(0.8, 0.7, 0.6);
            }
            glBegin(GL_QUADS);
            glVertex2i(x,  BOARD_SIZE - y - 100);
            glVertex2i(x, BOARD_SIZE - y - 100 + 100);
            glVertex2i(x + 100, BOARD_SIZE - y - 100 + 100);
            glVertex2i(x + 100, BOARD_SIZE - y - 100);
            glEnd();
			
        }
    }

	draw_pieces_from_case_list(gui);
	glutSwapBuffers();
}
