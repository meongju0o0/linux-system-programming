#define MY_ID 30

#define SERV_TCP_PORT (7000 + MY_ID)
#define SERV_HOST_ADDR "163.180.140.216"

#define MSG_REQUEST 1
#define MSG_REPLY 2

typedef struct {
	int count;
	char buf[100];
} packet;
