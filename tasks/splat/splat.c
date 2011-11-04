/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */
#include <bits/fileno.h>
#include <unistd.h>

#define MAX_STATES 4
#define INTERVAL 200

int main(int argc, char** argv)
{
	/* Add code here -- put your names at the top. */
	char states[] = {'|', '/', '-', '\\'};
	int i = 0;
	while(1) {
		write(STDOUT_FILENO, &states[i], 1);
		i = (i + 1) % MAX_STATES;
		sleep(INTERVAL);
		write(STDOUT_FILENO, "\b", 1);
	}
	return 0;
}
