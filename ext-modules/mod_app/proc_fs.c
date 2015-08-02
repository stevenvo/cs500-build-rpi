#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/shm.h>
#include  <fcntl.h>
#include  <unistd.h>

extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *d, const void *src, size_t n);

#define BUF_MAX_SIZE  4
char rd_buf[BUF_MAX_SIZE];

int main(int argc, char **argv)
{
    /* Our file descriptor */
    int fd;
    int rc = 0;
    int i4Loop;
    int i4LoopMax = 10;
    int i4Delay = 5;  /* Seconds. */
    int sid = 0;
    unsigned long u4Uptime;

    sid = shmget(ftok("/etc/inittab",42), 1024, IPC_CREAT);
    printf("Shared Memory ID %d\n", sid);
    /* Loop until end of /proc/assignment5 file. */
    for( i4Loop = 0; i4Loop < i4LoopMax; i4Loop++) {
      /* Open the device */
      fd = open("/proc/assignment5", O_RDWR);
      if ( fd == -1 ) {
         perror("open failed");
         rc = fd;
         exit(-1);
      }
      memset(rd_buf, 0, BUF_MAX_SIZE);
      /* Issue a read */
      rc = read(fd, rd_buf, BUF_MAX_SIZE);
      if ( rc == -1 ) {
         printf("%s: read: FAILED!\n", argv[0]);
         perror("read failed");
         close(fd);
         exit(-1);
      }
      printf("%s: Read: %d Bytes\n", argv[0], rc);
      if (rc == 0) {
           printf("%s: Exit Read /proc/assignment5 File\n", argv[0]);
           break;
      }
      memcpy((char *)&u4Uptime, rd_buf, rc);
      printf("%s: Read %d System Uptime %d Seconds\n",
             argv[0], (i4Loop + 1), (int)u4Uptime);
      printf("%s: rd_buf 0x%x:%x:%x:%x\n",
             argv[0],rd_buf[0],rd_buf[1],rd_buf[2],rd_buf[3]);
      /* */
      close(fd);
      sleep(i4Delay);
    }
    /* */

    close(fd);
    return 0;
}

