#include "message.h"

int main() {
    int port = PORT;
    int s;
    struct sockaddr_in addr;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 0;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(s, (struct sockaddr *)&addr, sizeof addr) < 0) {
        perror("connect");
        return 0;
    }
    printf("ゲームマスターと接続しました.\n");
    printf("相手の入力を待っています...\n");
    struct Message mymsg, urmsg;
    recv(s, &urmsg, sizeof urmsg, 0);
    printf("相手の入力を受け付けました.\n");
    printf("対抗する8charactersを入力してください.>>");
    char buf[DATA_SIZE];
    do {
        memset(buf, 0, sizeof buf);
        fgets(buf, DATA_SIZE-1, stdin);
    } while (check_input(buf) < 0);
    int score = 0;
    calc_score(buf, urmsg.now, &score);
    printf("%d 点を獲得しました！\n", score);
    strcpy(mymsg.now, buf);
    mymsg.score = score;
    send(s, &mymsg, sizeof mymsg, 0);
    while (score < 8) {
        printf("相手の入力を待っています...\n");
        if (recv(s, &urmsg, sizeof urmsg, 0) < 0) {
            perror("recv");
            return 0;
        }
        printf("相手の入力を受け付けました.相手は %d 点獲得しました.\n", urmsg.score);
        if (urmsg.score >= 8) {
            printf("あなたの負けです！\n");
            return 0;
        }
        printf("前の入力から4characters採用し,8charactersを入力してください.>>");
        int tmp = 0;
        do {
            tmp = 0;
            memset(buf, 0, sizeof buf);
            fgets(buf, DATA_SIZE-1, stdin);
            calc_score(mymsg.now, buf, &tmp);
            if (tmp < 4)
                printf("前の入力から4characters採用してください.>>");
        } while (check_input(buf) < 0 || tmp < 4);
        calc_score(buf, urmsg.now, &score);
        printf("%d 点を獲得しました！\n", score);
        if (score >= 8) {
            printf("あなたの勝ちです！\n");
        }
        strcpy(mymsg.now, buf);
        mymsg.score = score;
        if (send(s, &mymsg, sizeof mymsg, 0) < 0) {
            perror("send");
            return 0;
        }
    }
}