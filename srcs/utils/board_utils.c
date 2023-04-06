
#include "../../includes/header.h"

extern t_game game;

GLuint get_pieces_image_selected(t_gui *gui, t_case *square)
{
		switch (square->status) {
			case WHITE | ROOK: return gui->pieces.white_rook_s;
			case WHITE | QUEEN: return gui->pieces.white_queen_s;
			case WHITE | BISHOP: return gui->pieces.white_bishop_s;
			case WHITE | PAWN: return gui->pieces.white_pawn_s;
			case WHITE | KNIGHT: return gui->pieces.white_knight_s;
			case WHITE | KING: return gui->pieces.white_king_s;
			case BLACK | ROOK: return gui->pieces.black_rook_s;
			case BLACK | QUEEN: return gui->pieces.black_queen_s;
			case BLACK | BISHOP: return gui->pieces.black_bishop_s;
			case BLACK | PAWN: return gui->pieces.black_pawn_s;
			case BLACK | KNIGHT: return gui->pieces.black_knight_s;
			case BLACK | KING: return gui->pieces.black_king_s;
			default: return 0;
		}
}

GLuint get_pieces_image(t_gui *gui, t_case *square)
{
		switch (square->status) {
			case WHITE | ROOK: return gui->pieces.white_rook;
			case WHITE | QUEEN: return gui->pieces.white_queen;
			case WHITE | BISHOP: return gui->pieces.white_bishop;
			case WHITE | PAWN: return gui->pieces.white_pawn;
			case WHITE | KNIGHT: return gui->pieces.white_knight;
			case WHITE | KING: return gui->pieces.white_king;
			case BLACK | ROOK: return gui->pieces.black_rook;
			case BLACK | QUEEN: return gui->pieces.black_queen;
			case BLACK | BISHOP: return gui->pieces.black_bishop;
			case BLACK | PAWN: return gui->pieces.black_pawn;
			case BLACK | KNIGHT: return gui->pieces.black_knight;
			case BLACK | KING: return gui->pieces.black_king;
			default: return 0;
		}
}

int is_white_piece(t_case *square)
{
	if (square->status == 0)
		return (-1);
	return square->status >= WHITE ? 1 : 0;
}

void    case_selected(t_gui *gui, t_case *square)
{
	if (is_white_piece(square) != game.white_to_play)
		return ;

	GLuint imgs = get_pieces_image_selected(gui, square);

	if (imgs != 0)
	{
		if (square->square_img != 0)
		{
			////mlx_destroy_image(gui->//mlx, square->square_img);
		}
		game.is_piece_selected = 1;
		printf("piece is selected\n");
		if (gui->square_selected != 0)
		{
			GLuint img = get_pieces_image(gui, gui->square_selected);
			square->square_img = img;
			//mlx_put_image_to_window(gui->//mlx, gui->win, img, gui->square_selected->startX, gui->square_selected->startY);
		}
		square->square_img = imgs;
		gui->square_selected = square;
		//mlx_put_image_to_window(gui->//mlx, gui->win, imgs, square->startX, square->startY);
	}
	else
		perror("put image");
}

int		get_square_from_xy(int x, int y)
{
	return (8 * x / BOARD_SIZE + (8 * y / BOARD_SIZE) * 8);
}