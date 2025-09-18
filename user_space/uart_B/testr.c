// testr.c
#include <stdio.h>
#include <stdlib.h> // exit() 等等
#include <fcntl.h>  // open() flags (O_RDWR)
#include <unistd.h> // read(), write(), close()
#include <sys/ioctl.h> // ioctl()

int main(int argc , char *argv[])
{
  int fh, cnt=0; // fh: 檔案描述子, cnt: 表示讀取多少字元
  char bfr; // 單一字元緩衝區
 
  printf("I/O test for UART driver \n");
  printf("Press Ctrl C to exit\n");
  fh = open("/dev/uart3", O_RDWR); // 嘗試開啟 /dev/uart3, 允許讀寫, 失敗時回傳-1
  while(1) {  // 無限迴圈
    cnt = read(fh, &bfr, 1); // 嘗試從 /dev/uart3 讀取 1 byte 放到 bfr
    if(cnt > 0) { // 成功讀到的位元組數
      printf("read %d characters: %c\n", cnt, bfr); // 印出 幾個字元 字元內容
    }
  }
  close(fh);

  return 0;
}
