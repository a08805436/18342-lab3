/*
 * kread.c:  bare implementation of read syscall using the Exports API
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 *
 */

#include <exports.h>
#include <bits/errno.h>
#include <bits/fileno.h>

#define MAX_BUF_SIZE 0x4000000   // 64 MB max buffer size
#define SDRAM_RANGE_END 0xa4000000

#define EOT 0x4

/*
 * implementation of the read syscall
 * @param: fd - file descriptor
 * @param: buf - user task buffer pointer
 * @param: count - number of bytes the user task wants to read
 * @return ssize_t - number of bytes read on success, < 0 on failure
 */
ssize_t kread(int fd, void *buf, size_t count)
{
	char ch;
	ssize_t read_cnt = -1;
	char *ubuf = buf;
	/*
	 * validate the fd argument
	 */
	if(fd != STDIN_FILENO) {
	 	return -EBADF;
	}

	/*
	 * validate the user supplied count
	 */
	if(count > MAX_BUF_SIZE) {
		return -EFAULT;
	}
	  
	/*
	 * validate the range of buffer memory addresses
	 */
	if(((char *)ubuf + count) >= (char *)SDRAM_RANGE_END) {
	    return -EFAULT;
	}

	/*
	 * all set, read the bytes from STDIN and handle different cases
	 */
	do {
		ch = (char)getc();
		switch (ch) 
		{
			case EOT:
				/*
				 * end of text
				 */
				return (read_cnt + 1);
			case '\b':
				/*
				 * backspace or delete key was pressed
				 */
				if(read_cnt > -1) {
					ubuf[read_cnt] = 0;
					read_cnt--;
					putc('\b');
					putc(' ');
					putc('\b');
				}
				break;
			case '\n':
			case '\r':
				/*
				 * carriage return/line feed was pressed
				 */
				if((read_cnt + 1) < (ssize_t)count) {
			    	read_cnt++;
					ubuf[read_cnt] = '\n';
					putc('\n');
				} 
				return (read_cnt + 1);
			default:
				/*
				 * for all other inputs
				 */
				if((read_cnt + 1) < (ssize_t)count) {
			    	read_cnt++;
					ubuf[read_cnt] = ch;
					putc(ch);
				} else {
					return (read_cnt + 1);
				}
		}
	} while(1);
}
