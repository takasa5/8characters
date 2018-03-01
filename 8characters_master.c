#include "message.h"

int main() {
    int s;
    struct sockaddr_in my_addr;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 0;
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&my_addr, sizeof my_addr) < 0) {
        perror("bind");
        return 0;
    }
    listen(s, 5);
    printf("プレイヤー受付中...\n");
    int s2;
    struct sockaddr_in addr;
    socklen_t sktlen = sizeof addr;
    s2 = accept(s, (struct sockaddr *)&addr, &sktlen);
    printf("プレイヤーを発見しました. ゲームを開始します.\n");
    // 最初の一回
    printf("最初の8charactersを入力してください>>");
    char buf[DATA_SIZE];
    do {
        memset(buf, 0, sizeof buf);
        fgets(buf, DATA_SIZE-1, stdin);
    } while (check_input(buf) < 0);
    struct Message mymsg, urmsg;
    strcpy(mymsg.now, buf);
    send(s2, &mymsg, sizeof mymsg, 0);
    int score = 0;
    while (score < 8) {
        printf("相手の入力を待っています...\n");
        if (recv(s2, &urmsg, sizeof urmsg, 0) < 0) {
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
        if (send(s2, &mymsg, sizeof mymsg, 0) < 0) {
            perror("send");
            return 0;
        }
    }
}

