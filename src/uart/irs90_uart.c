// irs90_uart.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>   // open()
#include <unistd.h>  // read(), write(), close()
#include <errno.h>

#define IRS_DEV "/dev/irs90_all"
#define UART_DEV "/dev/uart2"

int main()
{
    int fh_irs, fh_uart;
    char buf[16];              // 放讀出字串
    char last_state[4] = "000"; // 初始狀態

    printf("IRS90 UART driver\n");
    printf("Press Ctrl+C to exit\n");

    // 打開 UART
    fh_uart = open(UART_DEV, O_RDWR);
    if (fh_uart < 0) { perror("open uart2"); return -1; }

    while (1) {
        // 打開 IRS90
        fh_irs = open(IRS_DEV, O_RDONLY);
        if (fh_irs < 0) { perror("open irs90_all"); return -1; }

        // 讀出一行字串
        memset(buf, 0, sizeof(buf));
        ssize_t n = read(fh_irs, buf, sizeof(buf) - 1);
        if (n <= 0) {
            close(fh_irs);
            usleep(100000);
            continue;
        }
        buf[n] = '\0';

        // 取前三個字元作為狀態
        char curr_state[4];
        strncpy(curr_state, buf, 3);
        curr_state[3] = '\0';

        // 判斷條件：
        // prev -> curr
        // 000->100, 001->101, 010->110, 011->111
        int prev = strtol(last_state, NULL, 2);
        int curr = strtol(curr_state, NULL, 2);

        if ((prev & 0x3) == (curr & 0x3) && (prev & 0x4) == 0 && (curr & 0x4) == 4) {
            write(fh_uart, "Z", 1);
            printf("Triggered: %s -> %s, sent Z\n", last_state, curr_state);
        }

        // 更新狀態
        strncpy(last_state, curr_state, 3);
        last_state[3] = '\0';

        close(fh_irs);
        usleep(100000); // 100ms
    }

    close(fh_uart);
    return 0;
}
