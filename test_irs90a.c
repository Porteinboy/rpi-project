// test_irs90a.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main() {

    int fd = open("/dev/irs_90a", O_RDONLY);

    if(fd < 0){
    	perror("open");
	return -1;
    }

    while (1) {
        unsigned char buf;
	int ret = read(fd, &buf, sizeof(buf));
	if (ret > 0) {
	   printf("IRS_90A value = %d\n", buf);
	   fflush(stdout);
	}
	else if (ret == 0) {
		printf("blocking at read ret:%d, buf:%x\n", ret, buf);
	}
	else if (ret < 0 && errno != EAGAIN) {
	    perror("read");
	}
	usleep(500000);
    }

    close(fd);
    return 0;
}

