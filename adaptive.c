#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int count=0;
    float prev_download_speed = 0.0;
    for (int i = 0; i < 6; i++) {
        char command[100];
        sprintf(command, "speedtest --bytes --simple | grep Download");
        char result[1000];
        FILE* fp = popen(command, "r");
        if (fp == NULL) {
            printf("Failed to run command\n");
            exit(1);
        }
        printf("%d cycle\n",++count);
        while (fgets(result, sizeof(result)-1, fp) != NULL) {
            char *p = strchr(result, ' ');
        float download_speed;
        sscanf(p, "%f", &download_speed);
            printf("Download speed: %.2f\n", download_speed);
            if (i > 0) {
                float variation = download_speed - prev_download_speed;
        if(variation>0)
        {
            printf("Speed increased by %.2f\n", variation);
        }
        else
        {
            printf("speed reduced by%.2f\n", variation);
        }

            }
            prev_download_speed = download_speed;
        }
        pclose(fp);
        sleep(10);
    }
    return 0;
}
