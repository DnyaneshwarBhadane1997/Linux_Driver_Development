#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define CHAR_DEVICE "toto.txt"



int main(int argc, char **argv)
{
int fd= 0;
char buf[20];

if ((fd = open(CHAR_DEVICE, O_RDONLY)) < -1){

	return 1;
}


/* Read 20 bytes */
if (read(fd, buf, 20) != 20)
	return 1;
printf("%s\n", buf);
/* Move the cursor to 10 time, relative to its actual position */
if (lseek(fd, 10, SEEK_CUR) < 0)
	return 1;
if (read(fd, buf, 20) != 20)
	return 1;
printf("%s\n",buf);
/* Move the cursor ten time, relative from the beginning of the file */
if (lseek(fd, 7, SEEK_SET) < 0)
	return 1;
if (read(fd, buf, 20) != 20)
	return 1;
printf("%s\n",buf);
close(fd);
return 0;
}





