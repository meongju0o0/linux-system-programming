#define MY_ID 30

#define SERV_TCP_PORT (7000 + MY_ID)
#define SERV_HOST_ADDR "127.0.0.1"

#define MSG_REQUEST 1
#define MSG_REPLY 2

#define BUF_SIZE 100

typedef struct {
	int count;
	char buf[BUF_SIZE];
} PacketType;