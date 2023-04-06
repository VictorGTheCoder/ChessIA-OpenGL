#include "../includes/header.h"

t_game game;
t_gui *gui;




void	init_openGL(void)
{
	glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);

	 // Activer la transparence


}

void draw_chess_piece(GLuint texture, int x, int y, int case_size)
{
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(x, y);
    glTexCoord2f(1, 1); glVertex2i(x + case_size, y);
    glTexCoord2f(1, 0); glVertex2i(x + case_size, y + case_size);
    glTexCoord2f(0, 0); glVertex2i(x, y + case_size);
    glEnd();

    glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void draw_pieces_from_case_list(t_gui *gui)
{
    int case_size = BOARD_SIZE / 8;
    for (int i = 0; i < 64; i++) {
        t_case *square = &gui->case_list[i];

        if (square->status != EMPTY) {
            draw_chess_piece(square->square_img, square->startX, square->startY, case_size);
			printf("Draw on Square %d, at x:%d,y:%d status %d\n",i, square->startX, square->startY, square->status);
        }
    }
}

void display_board(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    printf("Create Board\n");
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
                glColor3f(0.3, 0.2, 0.1);
            }
            glBegin(GL_QUADS);
            glVertex2i(x, y);
            glVertex2i(x, y + 100);
            glVertex2i(x + 100, y + 100);
            glVertex2i(x + 100, y);
            glEnd();
			
        }
    }

	draw_pieces_from_case_list(gui);
	glutSwapBuffers();
}

void mouse_hook(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        int square_n = get_square_from_xy(x, y);
		t_case *square = &gui->case_list[square_n];
		printf("isSelected %d, isWhiteToPlay %d, square id %d, pieceIsWhite %d, square status %d\n", game.is_piece_selected, game.white_to_play, square_n, is_white_piece(square), square->status);
		if (game.is_piece_selected == 1)
		{
			printf("Square selected %d, x:%d, y%d\n", game.is_piece_selected, gui->square_selected->startX, gui->square_selected->startY);
			if (game.white_to_play == is_white_piece(square))
			{
				game.is_piece_selected = 0;
				deselect(gui, gui->square_selected);
				case_selected(gui, square);
			}
			else if (try_to_move(gui, gui->square_selected, square) == -1)
			{
				case_selected(gui, square);
			}
		}
		else
		{
			case_selected(gui, square);
		}
    }
}

void init_squares(t_gui *gui)
{
	t_case *square = &gui->case_list[0];

	square[0].status = ROOK | BLACK;
	square[1].status = KNIGHT | BLACK;
	square[2].status = BISHOP | BLACK;
	square[3].status = QUEEN | BLACK;
	square[4].status = KING | BLACK;
	square[5].status = BISHOP | BLACK;
	square[6].status = KNIGHT | BLACK;
	square[7].status = ROOK | BLACK;


	/*square[0].square_img = gui->pieces.black_rook;
	square[1].square_img = gui->pieces.black_knight;
	square[2].square_img = gui->pieces.black_bishop;
	square[3].square_img = gui->pieces.black_king;
	square[4].square_img = gui->pieces.black_queen;
	square[5].square_img = gui->pieces.black_bishop;
	square[6].square_img = gui->pieces.black_knight;
	square[7].square_img = gui->pieces.black_rook;*/



	square[56].status = ROOK | WHITE;
	square[57].status = KNIGHT | WHITE;
	square[58].status = BISHOP | WHITE;
	square[59].status = KING | WHITE;
	square[60].status = QUEEN | WHITE;
	square[61].status = BISHOP | WHITE;
	square[62].status = KNIGHT | WHITE;
	square[63].status = ROOK | WHITE;


	/*square[56].square_img = gui->pieces.white_rook;
	square[57].square_img = gui->pieces.white_knight;
	square[58].square_img = gui->pieces.white_bishop;
	square[59].square_img = gui->pieces.white_king;
	square[60].square_img = gui->pieces.white_queen;
	square[61].square_img = gui->pieces.white_bishop;
	square[62].square_img = gui->pieces.white_knight;
	square[63].square_img = get_pieces_image(gui, &square[63]);*/

		

	for (int i = 8; i < 16; i++)
    {
        square[i].status = PAWN | BLACK;
       // square[i].square_img = gui->pieces.black_pawn;
    }
    for (int i = 16; i < 48; i++)
    {
        square[i].status = EMPTY;
        //square[i].square_img = gui->pieces.empty;
    }
    for (int i = 48; i < 56; i++)
    {
        square[i].status = PAWN | WHITE;
       // square[i].square_img = gui->pieces.white_pawn;
    }

	for (int i = 0; i < 64; i++)
	{
		square[i].square_img = get_pieces_image(gui, &square[i]);
	}
	printf("status %d\n", square[7].status);
	square[7].square_img = gui->pieces.white_rook;
}

void key_hook(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		exit(0);
	}
	printf("key %c, x %d, y %d\n", key, x, y);
}

int main(int argc, char *argv[])
{

    gui = malloc(sizeof(t_gui)); 

    printf("INIT\n");
	
	init_game();
	init_gui(gui);

	printf("square statud %d\n", gui->case_list[2].status);
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(BOARD_SIZE, BOARD_SIZE);
	glutCreateWindow("CHESS-IA");
	init_openGL();
	load_textures(gui);
	init_squares(gui);
	glutDisplayFunc(display_board);
	glutMouseFunc(mouse_hook);
	glutKeyboardFunc(key_hook);
    glutMainLoop();
    free(gui);
    printf("FINISH\n");
    return 0;
}
