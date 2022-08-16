#include <fcntl.h>
off_t getfilelen(int fd);
void parselines(char *buff,off_t len,char ***lines,int *numlines);
void readreals(char *filename);
