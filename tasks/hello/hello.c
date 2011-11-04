/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	printf("\n current_time is %lu", time());
	sleep(5000);
	printf("\n current_time is %lu", time());
	sleep(5000);
	printf("\n current_time is %lu", time());
	sleep(5000);
	printf("\n current_time is %lu", time());
	sleep(5000);
	printf("\n current_time is %lu", time());
	sleep(5000);
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	printf("\n current_time is %lu", time());
	return 100;
}
