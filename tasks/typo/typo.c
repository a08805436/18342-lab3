/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include <bits/fileno.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_BUF_LEN 100
#define MILLIS_IN_SEC 1000
#define PRECISION_FACTOR 100

void calc_time(unsigned long begin_time, unsigned long end_time, int *second_decimal, int *second_fraction)
{
	*second_decimal = (end_time - begin_time) / MILLIS_IN_SEC;
	*second_fraction = ((end_time - begin_time) % MILLIS_IN_SEC) / 
	                     PRECISION_FACTOR;
}


int main(int argc, char** argv)
{
	/* Add code here -- put your names at the top. */
	char buf[MAX_BUF_LEN];
	char prompt[] = "> ";
	unsigned long begin_time = 0, end_time = 0;
	ssize_t read_cnt = 0;
	int second_decimal = 0, second_fraction = 0;

	while(1) {
		write(STDOUT_FILENO, prompt, 2);
		begin_time = time();
		read_cnt = read(STDIN_FILENO, buf, MAX_BUF_LEN);
		end_time = time();
		write(STDOUT_FILENO, buf, read_cnt);
		calc_time(begin_time, end_time, &second_decimal, &second_fraction);
		printf("%d.%ds\n", second_decimal, second_fraction);
	}
	return 0;
}
