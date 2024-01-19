        /* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

        /* Function Declarations */
void welcome();
void prompt();
void displayHistory();
void outputReDir(char **left, char **right, int leftSize);
void append(char **left, char **right, int leftSize);
void mypipe(char **left, char **right, int leftSize); 
void systemcommand(char **argv);
void myor(char **left, char **right, int leftSize);
void myand(char **left, char **right, int leftSize);
void mysemicolon(char **left, char **right, int leftSize);

        /* Defines */
#define TRUE 1
#define FALSE 0
#define READ 0
#define WRITE 1
#define BUF_SIZE 50
#define buf      

       /*Declarations*/

char history[20][BUF_SIZE];
int count = 0;

        /* Main Function */
	int main(int c, char **v) 
	{
		int i, j;
		int special;
		int specialpos;
		char **argv;
		char **left;
		char **right;
		int special_c = FALSE;
		int argc = 0; 
		char lineInput[80];
		argv = malloc(sizeof(char)* 200);  
		left = malloc(sizeof(char)* 100);
		right = malloc(sizeof(char)* 100);
                	
		
		if (c <2) 
		{	system("clear");
			fprintf(stderr,"\t\t\t  Batch file not provided\n\t\t\tRunning on interactive mode\n");
		} 
                else {
		        if (c>2) 
		        {
		                fprintf(stderr,"Inadequate number of arguments in command prompt\n");
				exit(1);
		        }                        
		        else 
		        {	
			     	char command[100];
				snprintf(command, sizeof(command), "sh %s", v[1]);
				int ret = system(command);
				if (ret != 0) 
				{
					fprintf(stderr, "Error executing batch file: %s\n", v[1]);
					return 1;			
				}
				exit(1);
			}
		}	
        	welcome();  
                while(1)
                { 
                        prompt();
			 fgets(lineInput, 80 ,stdin);
                        char *token;
                        char flags[]=" \n\t";
                        if(strcmp(lineInput, "exit\n")==0)
                        {break;}
                        token = strtok(lineInput, flags);
                        argv[argc] = token;
                        while (token != NULL)
                        {
                                token = strtok(NULL, flags);
                                argc++;
                                argv[argc] = token;
                        }
                        
			if(strcmp(argv[0],"history")==0)
        		{		
               			if(count>0)
				{
			
                			displayHistory();
                			continue;
				}
				else
				{
				        fprintf(stderr,"\nNo Commands in the history\n");
				}
				return -1;
        		}
        		for (i = 19;i>0; i--) 
       			{
                                strcpy(history[i], history[i-1]);
                        	}
    			strcpy(history[0],lineInput); 
    			count++;
			if(count>20)
			{ 
                                count=20;	
			}	
			
                        specialpos = 0;
                        special = -1;
                        for (i = 0; i < argc ; i++)
                        {  
                                if (strcmp(argv[i], ">>") == 0)
                                {
                                        special = 2;
                                        specialpos = i;
                                        special_c = TRUE;
                                }
                                else if (strcmp(argv[i], ">") == 0)
                                {
                                        special = 3;
                                        specialpos = i;
                                        special_c = TRUE;
                                }
                                else if (strcmp(argv[i], "|") == 0)
                                {
                                        special = 4;
                                        specialpos = i;
                                        special_c = TRUE;
                                }
                                else if (strcmp(argv[i], "||") == 0)
                                {
					 
					  special = 6;
					  specialpos = i;
					  special_c = TRUE;
				}
				else if (strcmp(argv[i], "&&") == 0)
                                {
					  special = 7;
					  specialpos = i;
					  special_c = TRUE;
				}
				else if (strcmp(argv[i], ";") == 0)
                                {
					  special = 8;
					  specialpos = i;
					  special_c = TRUE;
				}
                        }
                        
                        if (special_c == TRUE)
                        {
                                if (argc - 1 == specialpos)
                                {  
                                        special = 5;  
                                }
                                if (specialpos < 1)
                                { 
                                        special = 5;
                                }
                                else
                                {
                                        for(i=0; i< specialpos; i++)
                                        { 
                                                left[i] = argv[i];
                                        }
                                        i=0; 
                                        for(j = specialpos + 1; j < argc; j++)
                                        { 
                                                right[i] = argv[j];
                                                i++;
                                        }
                                }
                        }

                        switch(special) {  
                                case 2:         
                                        append(left, right, specialpos);
                                        break;
                                case 3:         
                                        outputReDir(left, right, specialpos);
                                        break;
                                case 4:         
                                        mypipe(left, right, specialpos);
                                        break;
                                case 5: 
                                        fprintf(stderr,"Command can not be executed\n");
                                        break;
                                case 6 :
   					  myor(left, right, specialpos);
   					  break;
   				 case 7 :
   					  myand(left, right, specialpos);
   					  break;
   				 case 8: 
   				          mysemicolon(left, right, specialpos); 
   				          break;
                                default:  
                                         systemcommand(argv);
                                }
                        
                        argc = 0;
                        special_c = FALSE;
                        for (i = 0; i < argc; i++)
                        {
                                argv[i] = NULL;
                        }

                }
                free(argv);
                free(left);
                free(right);
        }

        
/*---------------------------------------------------------Functions---------------------------------------------------------*/
        void welcome ()
        {
                printf("\n\t\t*****************"
                "***********************");
                printf(" \n\t\t\t     WELCOME ");
                printf(" %s" , getenv("USER")); 
                printf("\n\n\t\t\t-TO OUR ADVANCED SHELL-");
                printf("\n\t\t*******************"
                "*********************");
                printf("\n\n");
        }
        
        void prompt ()
        {
		char c[2048];
		getcwd(c, sizeof(c));
		printf("%s%%",c );
        }
        
        void displayHistory()
        {
		printf("\n Shell command history :\n");		
		int i;
		int j = 0;
		int histCount = count;	
		for (i = 0; i<20;i++)
		{
		        printf("%d.  ", histCount);
		        while (history[i][j] != '\n' && history[i][j] != '\0')
		        {	
				printf("%c", history[i][j]);
				j++;
		        }
		        printf("\n");
		        j = 0;
		        histCount--;
		        if (histCount ==  0)
		        break;
		}
		printf("\n");
        } 
        
        void outputReDir(char **left, char **right, int leftSize)
        {  
                char *str;                
                str = right[0];
                int file = open(str, O_WRONLY | O_TRUNC | O_CREAT, 0777);
                int saved_stdout; 
                saved_stdout = dup (1);
                dup2(file, 1);
                systemcommand(left);    
                dup2(saved_stdout, 1);  
                close(saved_stdout);

        }

        void append(char **left, char **right, int leftSize)
        {   
                char *str;
                str = right[0];                       
                int file = open(str, O_WRONLY|O_APPEND | O_CREAT, 0777);
                int saved_stdout;
                saved_stdout = dup (1);
                dup2(file, 1);
                systemcommand(left);  
                dup2(saved_stdout, 1);  
                close(saved_stdout);

        }

        void mypipe(char **left, char **right, int leftSize)
        { 
                pid_t pid;
                int fd[2];
                pid_t pid1;
                pid1=fork();
                if (pid1==0)
        	{

                	if(pipe(fd)==-1)
                	{
                                        fprintf(stderr,"piper error\n");
                                        return;
                        }

                        if((pid=fork()) == -1)
                	{
                                        fprintf(stderr,"fork Error\n");
                                        return;
                        }
                        else
                		if (pid==0)
                		{
                                        close(1);
                                        dup2(fd[WRITE], 1);
                                        close(fd[READ]);
                                        execvp(left[0], left);
                                }
                                else 
                                {
                                        close(0);
                                        dup2(fd[READ], STDIN_FILENO);
                                        close(fd[WRITE]);
                                        execvp(right[0], right);
                                }
                }
                else
        	{ 
        		waitpid(pid,NULL, 0);
                }
        }
        
        void myor(char **left, char **right, int leftSize) 
        {
		int status;
		pid_t pid;
		pid = fork();
		if (pid == 0) 
		{
		        execvp(left[0], left);
		        exit(1);
		} 
		else 
		{
		        wait(&status);
		        if (WEXITSTATUS(status) != 0) 
		        {
				pid = fork();
				if (pid == 0) 
				{
				        execvp(right[0], right);
				        exit(1);
				} 
				else 
				{
				        wait(&status);
				}
		        }
		}
        }
        
        void myand(char **left, char **right, int leftSize) 
        {
		int status;
		pid_t pid;
		pid = fork();
		if (pid == 0) 
		{
		        execvp(left[0], left);
		        exit(1);
		} 
		else 
		{
		        wait(&status);
		        if (WEXITSTATUS(status) == 0) 
		        {
				pid = fork();
				if (pid == 0) 
				{
				        execvp(right[0], right);
				        exit(1);
				} 
				else 
				{
				        wait(&status);
				}
		        }
		}
        }

        void mysemicolon(char **left, char **right, int leftSize) 
        {
		int status;
		pid_t pid;
		pid = fork();
		if (pid == 0) 
		{
		        execvp(left[0], left);
		        exit(1);
		} 
		else 
		{
		        wait(&status);
		        pid = fork();
		        if (pid == 0) 
		        {
				execvp(right[0], right);
				exit(1);
		        } 
		        else 
		        {
		        	wait(&status);
		        }
		}
        }

        void systemcommand(char **argv)
        {
		int pid = fork();
		if(-1 == pid)
		{
		        fprintf(stderr,"Failed to create a child\n");
		}
		else
		{   
			if(0 == pid)
		        {
				if(execvp(argv[0], argv)==-1)
				{
					fprintf(stderr,"Command not found\n");
				}
				else 
				{
					execvp(argv[0], argv);
				}
			}				
		        else
		        {
		                waitpid(pid, NULL, 0);
		        }
		}
        }
//EOF