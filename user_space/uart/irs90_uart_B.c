// irs90_uart.c
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>  // memcpy()
#include <fcntl.h>   // open()
#include <unistd.h>  // read(), write(), close()
#include <errno.h>

#define IRS_DEV "/dev/irs90_all"
#define UART_DEV "/dev/uart2"

int main(int argc , char *argv[])
{
  int fh_irs, fh_uart;        // fh: UART裝置檔案描述子
  char buf[4];                // 暫存要用的字串
  char last_state[4] = "000"; // 初始狀態

  // 打開裝置
  printf("IRS90 UART driver \n");
  printf("Press Ctrl C to exit\n");

  fh_uart = open("/dev/uart2", O_RDWR);     // 打開uart2     driver, 允許讀寫
  if (fh_uart < 0) {perror("open uart2"); return -1;}
    
  fh_irs = open("/dev/irs90_all", O_RDWR); // 打開irs90_all driver, 允許讀
  if (fh_irs < 0) {perror("open irs90_all"); return -1;}
  
  while(1) {  // 主迴圈
    memset(buf, 0, sizeof(buf)); 
    if (read(fh_irs, buf, 3) != 3) continue; // 每次讀3個字元
    buf[3] = '\0'; // null terminate

    // 將字串轉為二進位整數
    int prev = strtol(last_state, NULL, 2);
    int curr = strtol(buf, NULL, 2);

    if ((prev & 0x3) == (curr & 0x3) && (prev & 0x4) == 0 && (curr & 0x4) == 4) {
      write(fh_uart, "Z", 1);
      printf("Triggered: %s -> %s, sent Z\n", last_state, buf);
    }

    // 更新上個狀態
    strncpy(last_state, buf, 3);
    last_state[3] = '\0';

    usleep(100000); // 100ms
  }

  close(fh_irs);
  close(fh_uart);
  
  return 0;
}
