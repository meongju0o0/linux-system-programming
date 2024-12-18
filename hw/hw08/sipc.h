#define	MY_ID 30

#define	SHM_KEY (0x9000 + MY_ID)
#define	SHM_SIZE 1024
#define	SHM_MODE (IPC_CREAT | 0600)

#define REQUEST_SEM_KEY (0x6000 + MY_ID)
#define REPLY_SEM_KEY (0x7000 + MY_ID)
