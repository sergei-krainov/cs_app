extern int ls;
extern int ns;
extern int fd;
extern struct stat file_stat;

void sigchld_handler(int signo);
void sendfile_fork(void);
