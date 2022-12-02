#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/
struct termios orig_termios;

/*** terminal ***/
void die(const char *s) {
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == 1)
    die("tcsetattr");
}

void enableRawMode()
{
    // Save the state of the terminal in to orig_termios.
    if (tcgetattr(STDIN_FILENO, &orig_termios) == 1) 
    die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    // raw.c_clafg = raw.c_clafg | CS8
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 1)
    die("tcsetattr");
}

/*** init ***/
int main()
{
    enableRawMode();

    while (1) {
    char c = '\0';
    // Read one byte at a time, until no bytes are left.
    if (read(STDIN_FILENO, &c, 1) == 1 && errno != EAGAIN) die("read");
        // ASCII 32-126 & 127 are control chars.
        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q')
            break;
        }
    return 0;
}