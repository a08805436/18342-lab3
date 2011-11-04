/*
 * kwrite.c:  bare implementation of write syscall using the Exports API
 *
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 */

#include <exports.h>
#include <bits/fileno.h>
#include <bits/errno.h>

#define STDOUT_FILENO 1 
#define MAX_BUF_SIZE 0x4000000   // 64 MB max buffer size
#define SDRAM_RANGE_END 0xa4000000

ssize_t kwrite(int fd, const void *buf, size_t count)
{
	ssize_t write_cnt = 0;
	const char *ubuf = buf;
	/*
	 * validate the fd argument
	 */
	 if(fd != STDOUT_FILENO) {
	 	 return -EBADF;
	 }

	/*
	 * validate the user supplied pointer
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
	 * all set, write out the buffer to STDOUT
	 */
	 while((write_cnt < (ssize_t)count) && (ubuf[write_cnt] != '\0')) {
	 	putc(ubuf[write_cnt++]);
	}
	return write_cnt;
}
