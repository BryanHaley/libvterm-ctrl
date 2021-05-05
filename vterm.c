/* COPYRIGHT 2018 BRYAN HALEY UNDER THE MIT LICENSE *
 * SEE LICENSE.MD FOR MORE INFORMATION              */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* TODO: roll my own simple string-handling functions to remove these dependencies. 
 * Not all MCUs provide string.h or other UNIX-like headers. */

#include "vterm.h"

#ifndef LVTC_EMBEDDED
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

#define ASCII_ESC  0x1B
#define ASCII_ZERO 0x30

FILE *inp, *out;

void vt_init (FILE *input, FILE *output) 
{ 
    inp = input;
    out = output;
}

void vt_send (char* seq)
{
    int len = strlen(seq);
    vt_nsend(seq, len);
}

void vt_nsend (char* seq, int len)
{
    int i = 0;

    /* Print the escape sequence to the terminal file stream */
    putc(ASCII_ESC, out);

    for (i = 0; i < len; i++)
    { 
        if (seq[i] == '\0') { break; }
        putc(seq[i], out);
    }
}


void vt_move_cursor_up (char* units)
{   
    /* A note on the usage of char* here as opposed to int:
    * Ultimately since communication with the terminal is done
    * entirely through ASCII data, any integer value must eventually 
    * be converted to one or more ASCII characters before
    * being sent. Therefore, rather than convert integers to 
    * characters within these functions, it is left up to the user
    * to decide when and where to convert them, if necessary at all. */
    char cursor_command[10] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(units) <= 0) { return; }
#endif

    strncat(cursor_command, "[", 1);
    strncat(cursor_command, units, 7);
    strncat(cursor_command, "A", 1);

    vt_nsend(cursor_command, 10);
}

void vt_move_cursor_down (char* units)
{
    char cursor_command[10] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(units) <= 0) { return; }
#endif

    strncat(cursor_command, "[", 1);
    strncat(cursor_command, units, 7);
    strncat(cursor_command, "B", 1);

    vt_nsend(cursor_command, 10);
}

void vt_move_cursor_left (char* units)
{
    char cursor_command[10] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(units) <= 0) { return; }
#endif

    strncat(cursor_command, "[", 1);
    strncat(cursor_command, units, 7);
    strncat(cursor_command, "C", 1);

    vt_nsend(cursor_command, 10);
}

void vt_move_cursor_right (char* units)
{
    char cursor_command[10] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(units) <= 0) { return; }
#endif

    strncat(cursor_command, "[", 1);
    strncat(cursor_command, units, 7);
    strncat(cursor_command, "D", 1);

    vt_nsend(cursor_command, 10);
}

void vt_move_cursor_xy (char* x, char* y)
{
    char cursor_command[18] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(x) <= 0 || strlen(y) <= 0) { return; }
#endif

    strncat(cursor_command, "[", 1);
    strncat(cursor_command, y, 7);
    strncat(cursor_command, ";", 1);
    strncat(cursor_command, x, 7);
    strncat(cursor_command, "f", 1);

    vt_nsend(cursor_command, 18);
}

void vt_scroll_up (void)
{
    putc(ASCII_ESC, out);
    putc('D', out);
}

void vt_scroll_down (void)
{
    putc(ASCII_ESC, out);
    putc('M', out);
}

void vt_clear_line (void)
{
    vt_nsend("[2K", 3);
}

void vt_clear_screen (void)
{
    vt_nsend("[2J", 3);
}

void vt_hide_cursor (void)
{
    vt_nsend("[?25l", 5);
}

void vt_show_cursor (void)
{
    vt_nsend("[?25h", 5);
}

void vt_setcolor_4bit_fg (char* fg_code)
{
    char color_command[6] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(fg_code) <= 0) { return; }
#endif

    color_command[0] = '[';
    strncat(color_command, fg_code, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 6);
}

/* This is redundant since fg and bg codes don't overlap */
void vt_setcolor_4bit_bg (char* bg_code)
{
    char color_command[6] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(bg_code) <= 0) { return; }
#endif

    color_command[0] = '[';
    strncat(color_command, bg_code, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 6);
}

void vt_setcolor_4bit (char* fg_code, char* bg_code)
{
    char color_command[10] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(fg_code) <= 0 || strlen(bg_code) <= 0) { return; }
#endif

    color_command[0] = '[';
    strncat(color_command, fg_code, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, bg_code, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 10);
}

void vt_setcolor_8bit_fg (char* fg_code)
{
    char color_command[11] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(fg_code) <= 0) { return; }
#endif

    strncat(color_command, "[38;5;", 6);
    strncat(color_command, fg_code, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 11);
}

void vt_setcolor_8bit_bg (char* bg_code)
{
    char color_command[11] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(bg_code) <= 0) { return; }
#endif

    strncat(color_command, "[48;5;", 6);
    strncat(color_command, bg_code, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 11);
}

void vt_setcolor_8bit (char* fg_code, char* bg_code)
{ 
    char color_command[20] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(fg_code) <= 0 || strlen(bg_code)) { return; }
#endif

    strncat(color_command, "[38;5;", 6);
    strncat(color_command, fg_code, 3);
    strncat(color_command, ";48;5;", 6);
    strncat(color_command, bg_code, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 20);
}

void vt_setcolor_tru_fg (char* r, char* g, char* b)
{
    char color_command[19] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(r) <= 0 || strlen(g) <= 0 || strlen(b) <= 0) { return; }
#endif

    strncat(color_command, "[38;2;", 6);
    strncat(color_command, r, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, g, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, b, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 19);
}

void vt_setcolor_tru_bg (char* r, char* g, char* b)
{
    char color_command[19] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(r) <= 0 || strlen(g) <= 0 || strlen(b) <= 0) { return; }
#endif

    strncat(color_command, "[48;2;", 6);
    strncat(color_command, r, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, g, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, b, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 19);
}

void vt_setcolor_tru (char* r_fg, char* r_bg, char* g_fg, char* g_bg, char* b_fg, char* b_bg)
{
    char color_command[36] = {0};

#ifndef DISABLE_SAFETY_CHECK
    if (strlen(r_fg) <= 0 || strlen(r_bg) <= 0 || 
        strlen(g_fg) <= 0 || strlen(g_bg) <= 0 ||
        strlen(b_fg) <= 0 || strlen(b_bg) <= 0) 
    { return; }
#endif

    strncat(color_command, "[38;2;", 6);
    strncat(color_command, r_fg, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, g_fg, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, b_fg, 3);
    strncat(color_command, ";48;2;", 6);
    strncat(color_command, r_bg, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, g_bg, 3);
    strncat(color_command, ";", 1);
    strncat(color_command, b_bg, 3);
    strncat(color_command, "m", 1);

    vt_nsend(color_command, 36);
}

void vt_bold_text (void)
{
    vt_nsend("[1m", 3);
}

void vt_underline_text (void)
{
    vt_nsend("[4m", 3);
}

void vt_invert_colors (void)
{
    vt_nsend("[7m", 3);
}

void vt_blink_text (void)
{
    vt_nsend("[5m", 3);
}

void vt_reset_text_attributes (void)
{
    vt_nsend("[0m", 3);
}

void vt_get_cursor_pos (vt_vec2 *pos)
{
    char esc = 0;
    int x_pos = 0, y_pos = 0;
 
    /* send the request to get the screen size */
    vt_nsend("[6n", 3);
    esc = getc(inp);

    /* read the response out of inp */
    if (esc == ASCII_ESC && getc(inp) == '[')
    {
        char in = 0;

#define INPUT_BUFF_LEN 10

        char x_str[INPUT_BUFF_LEN] = {'\0'};
        char y_str[INPUT_BUFF_LEN] = {'\0'};
        int index = 0;

        while (in != ';' && index < INPUT_BUFF_LEN)
        {
            in = getc(inp);

            /* sanity check */
            if (in < '0' || in > '9') { continue; }

            if (in == ';') { break; }

            y_str[index] = in;
            index++;
        }

        y_pos = atoi(y_str);
        index = 0;

        while (in != 'R' && index < INPUT_BUFF_LEN)
        {
            in = getc(inp);

            /* sanity check */
            if (in < '0' || in > '9') { continue; }

            if (in == 'R') { break; }

            x_str[index] = in;
            index++;
        }

        x_pos = atoi(x_str);
    }

#undef INPUT_BUFF_LEN

    else
    {
        pos->x = -1;
        pos->y = -1;
        return;
    }
    
    pos->x = x_pos;
    pos->y = y_pos;
}

void vt_get_screen_size (vt_vec2 *pos)
{
#ifdef LVTC_EMBEDDED

    /* There is no actual ANSI/ISO/VT-100 escape sequence to get the size of the terminal 
     * screen, and this library can't rely on an OS to exist to provide ioctl, so this
     * function just cheats the old fashioned way by telling the cursor to go to position
     * (999,999) then asking where the cursor actually ended up. */

    char old_x[15] = {0};
    char old_y[15] = {0};
    vt_vec2 old_pos, new_pos;

    vt_get_cursor_pos(&old_pos);
    vt_move_cursor_xy("999", "999");
    vt_get_cursor_pos(&new_pos);

    /*itoa(old_pos->x, old_x, 10);
    itoa(old_pos->y, old_y, 10);*/

    sprintf(old_x, "%d", old_pos.x);
    sprintf(old_y, "%d", old_pos.y);

    vt_move_cursor_xy(old_x, old_y);

    pos->x = new_pos.x;
    pos->y = new_pos.y;

#else

    /* When running on a UNIX-based system, use ioctl to get the terminal size. */

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    pos->x = w.ws_col;
    pos->y = w.ws_row;

#endif
}

void vt_backspace (void)
{
    /* Simulate a backspace by setting the cursor back one space, sending a blank space to the
     * output file stream, then sending the cursor back a space again. */

    vt_nsend("[1D", 3);
    putc(' ', out);
    vt_nsend("[1D", 3);
}

void vt_clear_space (void)
{
    /* Cheat by sending a space then sending the cursor back one */
    putc(' ', out);
    vt_nsend("[1D", 3);
}

void vt_fill_screen (char c)
{
    int i = 0;
    vt_vec2 screen_size;
    int num_spaces;

    vt_get_screen_size(&screen_size);
    num_spaces = screen_size.x * screen_size.y;

    putc('\n', out);
    
    for (i = 0; i < num_spaces; i++)
    { putc(c, out); }
}

void vt_bell (void)
{
    putc('\a', out);
}
