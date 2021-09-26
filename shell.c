/*
Victor Lelis Soares
2019.1904.038-2
Compile usando: gcc shell.c -o shelll -Wall
Execute com: ./shelll
*/

//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

//arquivos
#include "commands.c"
#include "handlers.c"
#include "fila_cc.c"

#define MAX 300

/*apresenta uma mensagem na shell
caso MYPS1 não esteja definido
ou não tenho um conteudo válido
mostra uma mensagem padrão*/
void iniciaShell(){
    char* username = getenv("MYPS1");

    if(!username)
    	printf("tecii$ ");
    else
    	printf("%s$: ", username);
}

/*Redireciona saida/entrada padrão para arquivo
recebe o arquivo de entrada ou saida
e utiliza o descritor
void redirect(FILE *arquivo, int descriptor){
    int out = open(arquivo, O_RDWR|O_CREAT|O_APPEND, 0600);
    
    if (-1 == out) { 
    	perror("Impossivel abrir/criar arquivo\n"); 
    	exit(255); 
    }

    int save_out  = dup(fileno(stdout));    

    if ( dup2(out, fileno(stdout)) == -1) { 
    	perror("não pode redirecionar stdout\n"); 
    	exit(255); 
    }
    
    printf("zuei\n");

    fflush(stdout); 
    close(out);

    dup2(save_out, fileno(stdout));

    close(save_out);
    printf("Voltando a saida normal!\n");
}
*/

//separa a entrada em tokens
void parser_tokens_entrada(char *cmd, char *path, char *dir, char **argum){
	int i = 1;
	
	char cmdAux [MAX];

	strcpy(cmdAux, cmd);
	/*printf("Começo cmd->%s\n", cmd);*/
	char *s;
	strcat(path, "/");
	strcpy(dir, path);
	/*printf("Dir-->%s\n", dir);*/
	s = strtok(cmdAux, " ");
	/*printf("s->%s\n", s);*/
	strcat(dir, s);
	
	while (s) {
		s = strtok(NULL, " ");
		/*printf("%s ", s);*/
		argum[i++] = s;
	}

	argum[i] = NULL;
	
	/*printf("Dir-->%s\n", dir);
	while(*argum != 0){
		printf("argumentos:%s ", *argum++);
	}
	printf("\n");*/

}


int main(int argc, char *argv[], const char *envp[]){
	pid_t pid;
	int len;	
	//caso seja, 1, caso não, 0		
	int isBackGround = 0;
	char cmd[MAX], dir[MAX], *argum[MAX], caminhos[MAX][MAX];
	argum[0] = dir;

	//histórico implementado por meio de uma FIFO
	filacc* historico;
	historico = cria_filacc();


	setenv("MYPATH", getenv("PATH"), 1);
	char *caminho = getenv("MYPATH");

	signal(SIGINT, &sigintHandler);
	signal(SIGCHLD, &sigchldHandler);
	/*signal(SIGTSTP, &sigstpHandler); ainda está com erro*/
	
	/*printf("Dirs->%s\n", getenv("MYPATH"));*/

	//provisório
	/*printf("caminhos %s\n", caminho);
	Separa o conteudo da variavel MYPATH
	em tokens e joga na "matriz" caminhos
	*/
	char *separete = strtok(caminho, ":");
	int i = 0;
	while (separete) {
		/*printf("aqui->%s \n", separete);*/
		strcpy(caminhos[i], separete);
		separete = strtok(NULL, ": ");			
		i++;
	}


	while (1) {
		isBackGround = 0;
		iniciaShell();

		/* Le o comando do usuário 
		Caso fgets retorne NULL, significa que
		foi pressionado crtl+d, o que sairá do programa*/
		if(fgets(cmd, MAX, stdin) == NULL){
			printf("\n");
			free(historico);
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
		enfileira(historico, cmd);

		if(strncmp(cmd, "exit", 4) == 0){
			free(historico);
			exit(0);
		}
		else if(strncmp(cmd, "cd", 2) == 0){
			cd(cmd+3);
		}
		else if(strncmp(cmd, "echo", 4) == 0){
			echo((cmd+5));
		}
		else if(strncmp(cmd, "env", 3) == 0){
			/*No meu caso, utilizo arch linux
			então o comando env é equivalente
			a printenv*/
			printenv(envp);
		}else if(strncmp(cmd, "kill", 4) == 0){
			kill_(cmd+6);
		}else if(strncmp(cmd, "history", 7) == 0){
			imprime_fila(historico);
		}
		/*else if(strncmp(cmd, "export", 6) == 0){
			export(cmd+7);
		}*/
		else{
			/*cria novo processo*/
			pid = fork();
			/*memcpy(agrumAux,argum, MAX);*/
			if(pid == 0){
				int j = 0;
				/*memcpy(argum,agrumAux, MAX);*/
				while(caminhos[j] != 0){
					//estamos no filho
					//arrumar parser para os casos com pipe
				/*	printf("caminhos[%d]=%s\n", j, caminhos[j]);*/
					parser_tokens_entrada(cmd,caminhos[j++], dir, argum);
					if (execv(dir, argum) < 0) {
						;
					}
				}
			}
			else{
				
				/*caso não execute em background*/
				if(isBackGround == 0){
					wait(NULL);
				}

			}

		}
	}

	free(historico);
	return 1;
}



