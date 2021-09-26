#include <signal.h>
#include <errno.h>

//tratador de sinal crtl+c
void sigintHandler(int sig_num){
    printf("\n");
    fflush(stdout);
}

//tratador para evitar processos zumbis
void sigchldHandler(int signal){
	
	int status, savedErrno;
    pid_t childPid;

    savedErrno = errno;
    while((childPid = waitpid(0, &status, WNOHANG)) > 0){
        printf("filho %d terminou\n", childPid);
    }
    errno = savedErrno;	
}

/*
	tratador de sinal crtl+z, ainda nao funciona
	void sigstpHandler(int sig_num){
	    printf("Entrei");
	    kill(getpid(), SIGTSTP);
	    printf("Sai");
	}
*/