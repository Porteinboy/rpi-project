// simple_irs90_publish.c
#inlcude <stdio.h>
#inlcude <stdlib.h>
#inlcude <string.h>
#inlcude <unistd.h>
#inlcude <mosquitto.h>
#inlcude <signal.h>

static int running = 1;

void signal_handler(int signal) {
    running = 0;
}

int main() {
    struct mosquitto *mosq;
    FILE *fp;
    char buffer[256];
    int rc;

    signal(SIGINT, signal_handler);

    mosquitto_lab_init();
    mosq = mosquitto_new("irs90_simple", true, NULL);

    if (!mosq) {
        printf("無法創建客戶端\n");
	return 1;
    }

    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        printf("連接失敗\n");
	return 1;
    }

    mosquitto_loop_start(mosq);

    while (runnuing) {
    	fp = fopen("/dev/irs90_all", "r");
    	if (fp) {
    	    if (fgets(buffer, sizeof(buffer), fp)) {
	        // 移除換行符
		buffer[strcspn(buffer, "\n")] = 0;

		// 發布數據
                mosquitto_publish(mosq, NULL, "/sensor/irs90/raw", 
				strlen(buffer), buffer, 0, false);
		printf("發布: %s\n", buffer);
	    }
	    fclose(fp);
    	}
	sleep(2);
    }
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
