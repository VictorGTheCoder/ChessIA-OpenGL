
#include "../../includes/header.h"

void draw_transparent_square(int x, int y, int case_size)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 0.5, 0.3);

    glBegin(GL_QUADS);
    glVertex2i(x,BOARD_SIZE - y - 100);
    glVertex2i(x + case_size, BOARD_SIZE - y - 100);
    glVertex2i(x + case_size, BOARD_SIZE - y - 100 + case_size);
    glVertex2i(x, BOARD_SIZE - y - 100 + case_size);
    glEnd();

    glDisable(GL_BLEND);
}

GLuint get_pieces_image_selected(t_case *square)
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
        case EMPTY: return 0;
        default: return 0;
    }
}

GLuint get_pieces_image(t_case *square)
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
        case EMPTY: return 0;
        default: return 0;
    }
}

int get_status_by_index(int index, t_bb *bitboards)
{
    int square_status = 0;
    if (get_bit(bitboards->black_pieces | bitboards->white_pieces, index) == 0)
        square_status = EMPTY;
    else
    {
        if (get_bit(bitboards->black_pawns, index) == 1)
            square_status = PAWN | BLACK;
        else if (get_bit(bitboards->black_knights, index) == 1)
            square_status = KNIGHT | BLACK;
        else if (get_bit(bitboards->black_bishops, index) == 1)
            square_status = BISHOP | BLACK;
        else if (get_bit(bitboards->black_rooks, index) == 1)
            square_status = ROOK | BLACK;
        else if (get_bit(bitboards->black_queens, index) == 1)
            square_status = QUEEN | BLACK;
        else if (get_bit(bitboards->black_king, index) == 1)
            square_status = KING | BLACK;
        else if (get_bit(bitboards->white_pawns, index) == 1)
            square_status = PAWN | WHITE;
        else if (get_bit(bitboards->white_knights, index) == 1)
            square_status = KNIGHT | WHITE;
        else if (get_bit(bitboards->white_bishops, index) == 1)
            square_status = BISHOP | WHITE;
        else if (get_bit(bitboards->white_rooks, index) == 1)
            square_status = ROOK | WHITE;
        else if (get_bit(bitboards->white_queens, index) == 1)
            square_status = QUEEN | WHITE;
        else if (get_bit(bitboards->white_king, index) == 1)
            square_status = KING | WHITE;
    }

    return square_status;
}

int is_white_piece(t_case *square)
{
	if (square->status == 0)
		return (-1);
	return square->status >= WHITE ? 1 : 0;
}

void draw_highlighted_piece(GLuint texture, int x, int y, int case_size)
{
    draw_chess_piece(texture, x, y, case_size); // Dessinez la pièce normalement

    // Dessinez un carré blanc semi-transparent par-dessus
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f); // Couleur blanche avec une opacité de 30%

    glBegin(GL_QUADS);
    glVertex2i(x,BOARD_SIZE - y - 100);
    glVertex2i(x + case_size, BOARD_SIZE - y - 100);
    glVertex2i(x + case_size, BOARD_SIZE - y - 100 + case_size);
    glVertex2i(x, BOARD_SIZE - y - 100 + case_size);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Réinitialiser la couleur à blanc opaque
    glDisable(GL_BLEND);
}


void case_selected(t_case *square)
{
    if (game->is_piece_selected && is_white_piece(square) == game->white_to_play)
    {
        // Désélectionnez la pièce si la même couleur est cliquée
        game->is_piece_selected = 0;
        gui->square_selected = NULL;
    }
    else if (is_white_piece(square) == game->white_to_play)
    {
        // Sélectionnez la pièce si elle est de la couleur appropriée
        game->is_piece_selected = 1;
        gui->square_selected = square;
    }
    else if (game->is_piece_selected == 1)
    {
        // Effectuez le mouvement si une pièce est sélectionnée et si la case cliquée est vide ou a une pièce de couleur opposée
        gui->square_selected->square_img = 0; // Enlevez la pièce de la case d'origine
        square->square_img = get_pieces_image(gui->square_selected); // Placez la pièce sur la nouvelle case

        game->is_piece_selected = 0;
        gui->square_selected = NULL;
        // game->white_to_play = !game->white_to_play; // Changez le joueur actuel
    }
    glutPostRedisplay();
}

int		get_square_from_xy(int x, int y)
{
	return (8 * x / BOARD_SIZE + (8 * y / BOARD_SIZE) * 8);
}


void update_gui(t_bb *b) {
    for (int x = 0; x < 64; x++) {
        t_case *current_case = &(gui->case_list[x]);

        // Black pieces
        if (get_bit(b->black_pawns, x)) {
            current_case->status = PAWN | BLACK;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->black_knights, x)) {
            current_case->status = KNIGHT | BLACK;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->black_rooks, x)) {
            current_case->status = ROOK | BLACK;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->black_queens, x)) {
            current_case->status = QUEEN | BLACK;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->black_king, x)) {
            current_case->status = KING | BLACK;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->black_bishops, x)) {
            current_case->status = BISHOP | BLACK;
            current_case->square_img = get_pieces_image(current_case);

        // White pieces
        } else if (get_bit(b->white_pawns, x)) {
            current_case->status = PAWN | WHITE;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->white_knights, x)) {
            current_case->status = KNIGHT | WHITE;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->white_rooks, x)) {
            current_case->status = ROOK | WHITE;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->white_queens, x)) {
            current_case->status = QUEEN | WHITE;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->white_king, x)) {
            current_case->status = KING | WHITE;
            current_case->square_img = get_pieces_image(current_case);
        } else if (get_bit(b->white_bishops, x)) {
            current_case->status = BISHOP | WHITE;
            current_case->square_img = get_pieces_image(current_case);
        } else {
            current_case->status = EMPTY;
            current_case->square_img = 0;
        }
    }
    glutPostRedisplay();
}