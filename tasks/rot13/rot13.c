/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Sridhar Srinivasan <sridhar1>
 *          Ramya Bolla <rbolla>
 *          Vinay Prasad <vinayp1>
 * Date:    10/11/2011  5:56 pm 
 */
#include <unistd.h>
#include <stdlib.h>

#define EXIT_STATUS 0xdeadbeef
#define BUF_LEN 100 
#define MAX_ARG_LEN 4096

extern int errno;

/*
 * prototype for the rot13 implementation function
 */
void performROT13(char arr[], int read_cnt);

int main(int argc, char *argv[])
{
	char arr[BUF_LEN];
	ssize_t read_cnt, write_cnt, temp_cnt;
	int i;
	/*
	 * write out the argument list, there is no need to 
	 * print argv[0] coz its always a3000000, we start from
	 * argv[1]. 
	 */
	for (i = 1; i < argc; i++) {
	    write (1, "Argument: ", 10);
		write(1, argv[i], MAX_ARG_LEN);
		write(1, "\n", 1);
	}

	while(1) {
	    /*
		 * reset all control variables to 0
		 */
	    read_cnt = 0;
		write_cnt = 0;
		temp_cnt = 0;

		/*
		 * read the user input into our buffer
		 */
		read_cnt = read(0, arr, BUF_LEN);
		if(arr[read_cnt - 1] != '\n') {
			/*
			 * buffer overflow... 
			 */
			 exit(0);
		}
		if(read_cnt < 0) {
			exit(-1);
		} else if(read_cnt == 0) {
			exit(-1);
		}
		else {
		    /*
			 * all good, lets rorate.... 
			 */
			performROT13(arr, read_cnt-1);
			/*
			 * finished rotating, now write into stdout
			 * taking care of short counts
			 */
			while(write_cnt < read_cnt) {
				temp_cnt=write(1, arr + write_cnt, read_cnt - write_cnt);
				if(temp_cnt < 0) {
					exit(1);
				}
				else {
					write_cnt += temp_cnt;
				}	
			}
		}
	}	
	return EXIT_STATUS;
}

/*
 * Implementation of the ROT13 cipher algorithm
 * @param: arr - buffer in the block to be encrypted is stored
 * @param: number of bytes present in the buffer for encryption
 * @return: void
 */
void performROT13(char arr[], int read_cnt)
{
	char b;
	int i;
	for (i = (read_cnt-1); i >= 0; i--) {
		b = arr[i];
		if ((b >= 'A' && b<='M') || 
			(b >= 'a' && b <= 'm')) {
			b += 13;
		} else if((b >= 'N' && b <= 'Z') || 
			(b >= 'n' && b <= 'z')) {
			b -= 13;
		}
		/*
		 * non alphabetic characters will be left unchanged
		 */
		arr[i] = b;
	}
}
