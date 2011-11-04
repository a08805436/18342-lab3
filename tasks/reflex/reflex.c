/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include <bits/fileno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MILLIS_IN_SEC 1000
#define MAX_LETTERS 52 
#define FREQ_INTERVAL 10
#define MAX_WORD_LEN 6
#define MAX_TRIES 5
#define MAX_SLEEP_TIME 3
#define ASCII_ZERO 0x30

/*
 * globals
 */
char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
				  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
				  'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
				  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 
				  'W', 'X', 'Y', 'Z'};
unsigned long total_time = 0;

/*
 * helper functions
 */

void timer(unsigned long begin_time, unsigned long end_time)
{
	total_time += (end_time - begin_time);
}

unsigned int rand(unsigned int max)
{
	return (((unsigned int)time()/FREQ_INTERVAL) % max);
}

void generate_word(char *word, int difficulty)
{
	int i;
	for (i = 0; i < difficulty; i++) {
		sleep(rand(MAX_SLEEP_TIME * MILLIS_IN_SEC));	
		word[i] = letters[rand(MAX_LETTERS)];
	}
	word[i] = '\n';
	return;
}

int main(int argc, char** argv)
{
	/* Add code here -- put your names at the top. */
	char prompt[] = "> ";
	int difficulty = 0, i = 0;
	unsigned long begin_time = 0, end_time = 0;
	ssize_t read_cnt = 0;
	int avg_sec = 0, avg_msec = 0;
	char word[MAX_WORD_LEN], answer[MAX_WORD_LEN];
	char temp[1];

	while(1) {
		i = 0;
		printf("\n New game");
		printf("\n Enter Difficulty Level <1-5>: ");
	    read(STDIN_FILENO, temp, 1);
		difficulty = temp[0] - ASCII_ZERO;
		if((difficulty < 1) || (difficulty > 5)) {
			printf("\n choose a difficulty level between 1 and 5");
			continue;
		}
		printf("\n");
		while(i < MAX_TRIES) {
			write(STDOUT_FILENO, prompt, 2);

			sleep(rand(MAX_SLEEP_TIME) * MILLIS_IN_SEC);

			generate_word(word, difficulty);
			write(STDOUT_FILENO, word, difficulty + 1);

			begin_time = time();
			read_cnt = read(STDIN_FILENO, answer, MAX_WORD_LEN);
			end_time = time();
			timer(begin_time, end_time);

			if(strncmp(word, answer, read_cnt)) {
				printf("\n incorrect answer\n");
				continue;
			}
			i++;
		}
		total_time /= MAX_TRIES;
		avg_sec = total_time/MILLIS_IN_SEC;
		avg_msec = total_time % MILLIS_IN_SEC;
		
		printf("Average reflex time is %d.%ds", avg_sec, avg_msec);
	}
	return 0;
}
