#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define MAX 100


void iniciaShell(){
    char* username = getenv("MYPS1");
    if(!username)
    	printf("tecii$ ");
    else
    	printf("%s$: ", username);
}

void sigintHandler(int sig_num){
    printf("\n");
    fflush(stdout);
}

void sigchldHandler(int signal){
	
	int status, savedErrno;
    pid_t childPid;

    savedErrno = errno;
    while((childPid = waitpid(0, &status, WNOHANG)) > 0){
        printf("filho %d terminou\n", childPid);
    }
    errno = savedErrno;
	
}

/*void sigstpHandler(int sig_num){
    printf("Entrei");
    kill(getpid(), SIGTSTP);
    printf("Sai");
}*/

void cd(char *cmd){
	

	printf("PWD antes -->%s\n", getenv("PWD"));
	//procura caminho do comando

	chdir((cmd));
	setenv("PWD", getcwd(NULL, 0), 1);
	printf("PWD depois -->%s\n", getenv("PWD"));
	//falta atualizar a variavel de ambiente PWD
}

void echo(char *cmd){/*
	printf("Echo-->%s\n", cmd);
	printf("%c\n", *(cmd+(strlen(cmd)-1)));*/

	if(*cmd == 39 || *cmd == 34){
		*(cmd+(strlen(cmd)-1)) = '\0';
		cmd = cmd+1;
	}
	else if(*cmd == '$'){
		/*printf("cheguei aqui\n");*/
		cmd = getenv(cmd+1);
		/*printf("aqui ó\n");*/
		if((cmd) == NULL){
			/*printf("entrei\n");*/
			printf("\n");
			return;
		}
	}
	printf("%s\n", cmd);
}

void printenv(const char *envp[]){
	int i = 0;
	while(*envp != 0){
		printf("%s\n", *envp++);
		i++;
	}
}

void parser_tokens_entrada(char *cmd, char *dir, char **argum, char *s){
	int i = 1;
	strcpy(dir, "/bin/");
	s = strtok(cmd, " ");
	
	strcat(dir, s);
	
	while (s) {
		s = strtok(NULL, " ");
		argum[i++] = s;
	}

	argum[i] = NULL;
}


int main(int argc, char *argv[], const char *envp[]){
	pid_t pid;
	int len;	
	int isBackGround = 0;//caso seja, 1, caso não, 0		
	char cmd[MAX], dir[MAX], *s = NULL, *argum[MAX];
	argum[0] = dir;

	signal(SIGINT, &sigintHandler);
	signal(SIGCHLD, &sigchldHandler);
	/*signal(SIGTSTP, &sigstpHandler);*/
	
	while (1) {
		isBackGround = 0;
		iniciaShell();
		/* le o comando do usuário */

		if(fgets(cmd, 100, stdin) == NULL){
			printf("\n");
			exit(0);
		}

		
		len = strlen(cmd);
		if(cmd[len-2] == '&'){
			isBackGround = 1;
			cmd[len-2] = '\0';
		}

		
		if (len < 2)
			continue;
		cmd[len - 1] = 0;

		if(strncmp(cmd, "exit", 4) == 0)
			exit(0);
		else if(strncmp(cmd, "cd", 2) == 0)
			cd(cmd+3);
		else if(strncmp(cmd, "echo", 4) == 0){
			echo((cmd+5));
		}
		else if(strncmp(cmd, "env", 3) == 0){
			printenv(envp);
		}
		else{
			/*cria novo processo*/
			pid = fork();
			
			if(pid == 0){
				//estamos no filho
				parser_tokens_entrada(cmd, dir, argum, s);
				if (execv(dir, argum) < 0) {
					printf("Comando não encontrado!\n");
					exit(1); 
				}
			}
			else{
				
				/*caso não execute em background*/
				if(isBackGround == 0){
					wait(NULL);
				}
				else{
				}

			}

		}
	}
	return 1;
}



