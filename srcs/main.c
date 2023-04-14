#include "../includes/header.h"

t_game game;
t_gui *gui;

void	init_openGL(void)
{
	glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);
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
	square[59].status = QUEEN | WHITE;
	square[60].status = KING | WHITE;
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
//        printf("square[%d].img = %d\n", i, square[i].square_img);
    }

}

int main(int argc, char *argv[])
{

    gui = malloc(sizeof(t_gui)); 

    printf("INIT\n");
	init_game();
	init_gui(gui);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
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
	free(gui->case_list);
    printf("FINISH\n");
    return 0;
}
