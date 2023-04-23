#include "../../includes/header.h"

void key_hook(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		exit(0);
	}
	printf("key %c, x %d, y %d\n", key, x, y);
}

void mouse_hook(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        int square_n = get_square_from_xy(x, y);
		t_case *square = &gui->case_list[square_n];
		//printf("isSelected %d, isWhiteToPlay %d, square id %d, pieceIsWhite %d, square status %d, image_status %d\n", game.is_piece_selected, game.white_to_play, square_n, is_white_piece(square), square->status, square->square_img);
		if (game.is_piece_selected == 1)
		{
			//printf("Square selected %d, x:%d, y%d\n", game.is_piece_selected, gui->square_selected->startX, gui->square_selected->startY);
			if (game.white_to_play == is_white_piece(square))
			{
				game.is_piece_selected = 0;
				deselect_piece(gui, gui->square_selected);
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

