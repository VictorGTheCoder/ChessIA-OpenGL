#include "../includes/header.h"

t_gui *gui;
t_game *game;
void	init_openGL(void)
{
	glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);
}

void init_squares(t_gui *gui, t_bb *bitboards)
{
	t_case *square = &gui->case_list[0];

    for (int i = 0; i < 64; i++)
    {
        square[i].status = get_status_by_index(i, bitboards);
        //printf("square[%d].status = %d\n", i, square[i].status);
        square[i].square_img = get_pieces_image(gui, &(square[i]));
        //setImage(&(square[i]));
    }
}

int main(int argc, char *argv[])
{

    gui = malloc(sizeof(t_gui)); 

    // print_bitboard(0x00FF000000000000ULL);
    // printf("EXAMPLE\n");
    // uint64_t x = 0x0000000000000400ULL;
    // print_bitboard(x);
    // x = x << 9;
    // printf("--------\n");
    // print_bitboard(x);
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
	init_squares(gui, game->bitboards);
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
