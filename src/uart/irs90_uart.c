// irs90_uart.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define IRS_DEV "/dev/irs90_all"
#define UART_DEV "/dev/uart2"

int main()
{
    int fh_irs, fh_uart;
    char buf[16];
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

        memset(buf, 0, sizeof(buf));
        ssize_t n = read(fh_irs, buf, sizeof(buf) - 1);
        if (n <= 0) {
            close(fh_irs);
            usleep(100000);
            continue;
        }
        buf[n] = '\0';

        char curr_state[4];
        strncpy(curr_state, buf, 3);
        curr_state[3] = '\0';

        // 檢查每一位從 0 -> 1
        for (int i = 0; i < 3; i++) {
            if (last_state[i] == '0' && curr_state[i] == '1') {
                char out = (i == 0) ? 'Z' : (i == 1) ? 'Y' : 'X';
                write(fh_uart, &out, 1);
                printf("Triggered bit %d: %c\n", i+1, out);
            }
        }

        strncpy(last_state, curr_state, 3);
        last_state[3] = '\0';

        close(fh_irs);
        usleep(100000);
    }

    close(fh_uart);
    return 0;
}
