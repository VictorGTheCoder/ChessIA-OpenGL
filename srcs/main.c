#include "../includes/header.h"

t_game *game;
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


/*int count_possible_moves(t_gui *gui, int depth) {
    if (depth == 0) {
        return 1;
    }

    int count = 0;

    for (int x_start = 0; x_start < 8; x_start++) {
        for (int y_start = 0; y_start < 8; y_start++) {
            t_case *start_square = &gui->case_list[y_start * 8 + x_start];

            // Vérifier si la case contient une pièce de l'équipe actuelle
            if (is_white_piece(start_square) == game->white_to_play) {
                continue;
            }

            for (int x_end = 0; x_end < 8; x_end++) {
                for (int y_end = 0; y_end < 8; y_end++) {
                    t_case *end_square = &gui->case_list[y_end * 8 + x_end];

                    if (move_is_valid(gui, start_square, end_square)) {
                        // Créer une copie profonde de la structure t_gui
                        t_gui *cloned_gui = clone_t_gui(gui);

                        // Simuler le coup
                        move_piece(cloned_gui, &cloned_gui->case_list[y_start * 8 + x_start], &cloned_gui->case_list[y_end * 8 + x_end]);

                        // Passer au joueur suivant
                        game->white_to_play = !game->white_to_play;

                        // Appeler récursivement avec une profondeur réduite
                        count += count_possible_moves(cloned_gui, depth - 1);

                        // Restaurer l'état du jeu
                        game->white_to_play = !game->white_to_play;

                        // Libérer la mémoire allouée pour la copie profonde
                        free(cloned_gui);
                    }
                }
            }
        }
    }

    return count;
}*/

int main(int argc, char *argv[])
{
    gui = malloc(sizeof(t_gui)); 


    printf("INIT\n");
	init_game();
	init_gui(gui);
    initialize_bitboards(game);
    print_bitboard(game->bitboards->white_pieces);
    printf("---------------\n");

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
	//printf("Moves %d\n", count_possible_moves(gui, 3));
    glutMainLoop();
    free(gui);
	free(gui->case_list);
    printf("FINISH\n");
    return 0;
}
