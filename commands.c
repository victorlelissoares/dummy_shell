#include <string.h>
#include <unistd.h>

//comando cd com atualização da variavel PWD
void cd(char *cmd){
	/*printf("PWD antes -->%s\n", getenv("PWD"));*/
	
	chdir((cmd));
	/*como getcwd(), retorna o caminho absoluto
	eu não preciso fazer alguma concatenação
	com o retorno do diretório atual*/
	setenv("PWD", getcwd(NULL, 0), 1);
	/*printf("PWD depois -->%s\n", getenv("PWD"));*/
	//falta atualizar a variavel de ambiente PWD
}

//comando echo, impressão de variaveis de ambiente e string
void echo(char *cmd){
	/*
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

/*comando env, que lista todas
as variaveis de ambiente*/
void printenv(const char *envp[]){
	while(*envp != 0){
		printf("%s\n", *envp++);
	}
}

void kill_(char *cmd){
	/*printf("cmd->%s\n", cmd);*/
	pid_t pid = atoi(cmd+2);
	int signal = atoi(cmd);
	/*printf("PID = %d, sinal = %d\n", pid, signal);*/
	kill(pid, signal);
}

//comando para (re)definição  de variaveis de ambiente
/*
void export(char *cmd){
	int i = 0;

	procura o sinal de atribuição
	e armazena a variavel que
	vai receber o conteudo da direita
	da atribuição.
	while(cmd[i] != '=')
		i++;

	char variavel[i+1];
	strncpy(variavel, cmd, i);
	variavel[i] = '\0';
	i++;

	if(cmd[i] != '$'){
		setenv(variavel, (cmd+i), 1);
	}else{
		i++;
		int j = i;
		while(cmd[j] != '\0'){
			
			if(cmd[j] != ':'){
				break;
			}
			
			j++;
		}


		strncpy(variavel, cmd+i, j);
		variavel[j-3] = '\0';
		printf("s->%s\n", variavel);
		printf("c->%s\n", cmd+i+j-2);

	}



}*/

