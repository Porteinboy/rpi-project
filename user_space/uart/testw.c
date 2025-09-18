// testw.c
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>  // memcpy()
#include <fcntl.h>   // open()
#include <unistd.h>  // read(), write(), close()
#include <termios.h> // termois 結構與 tcsetatrr()

int main(int argc , char *argv[])
{
  int fh, cnt=0; // fh: UART裝置檔案描述子, cnt: 寫入的字數
  char bfr[64];  // 暫存要用的字串
  struct termios original_t, new_t; // 保存與修改終端模式用

#ifndef __LINE_BUF__
  // Disable line buffering
  tcgetattr(fileno(stdin), &original_t);       // 取得原本 stdin 設定
  memcpy(&new_t, &original_t, sizeof(new_t));
  new_t.c_lflag &= ~(ICANON | ECHO);           // 關閉 canonical mode 與 echo
  tcsetattr(fileno(stdin), TCSAFLUSH, &new_t); // 套用新設定
  fflush(stdin);
#endif

  printf("I/O test for UART driver \n");
  printf("Press Ctrl C to exit\n");
  fh = open("/dev/uart2", O_RDWR); // 打開uart2 driver, 允許讀寫
  while(1) {  // 主迴圈
    printf("Input char:"); // 提示輸入
    memset(bfr, 0, sizeof(bfr) );
#ifndef __LINE_BUF__
    bfr[0]=getchar(); // 讀單一字元
    cnt=1;
#else
    scanf("%s", bfr); // 輸入字串 (遇空白結束)
    cnt=strnlen(bfr, 64);
#endif
    cnt = write(fh, bfr, cnt); // 把資料寫入 UART driver
    if(cnt > 0) {
      printf("write %d characters: %s\n", cnt, bfr); 
      // 成功後印出 實際寫入幾個字元 + 寫入內容
    }
  }
  close(fh); // 關閉 UART device

#ifndef __LINE_BUF__
  // Restore line buffering
  tcsetattr(fileno(stdin), TCSANOW, &original_t); // 還原終端輸入模式
#endif

  return 0;
}
