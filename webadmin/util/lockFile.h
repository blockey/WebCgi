#ifndef __LOCKFILE_H_
#define __LOCKFILE_H_

#define FILE_BUSY -2
#define ERROR -1
#define lock_t int 
#define MAX_WAIT_TIME 3 //3 seconds 


/* open and lock file */
lock_t lck_open(const char *path,const char *mode,FILE **fp);

/* release file  */
void lck_close(lock_t lock,FILE *fp);

#endif 