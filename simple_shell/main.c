#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include <time.h>
#include<sys/wait.h>

FILE *logFile;

void hand(int sig)
{
    int status;
    pid_t pidc;
    /* kills background process if it is termenated*/
    if(waitpid(pidc, &status,0) != -1) {

        if (status == 0)  // Verify child process terminated without error.
        {
            time_t mytime = time(NULL);
            char * time_str = ctime(&mytime);
            time_str[strlen(time_str)-1] = '\0';
            logFile= fopen("logfile.txt", "a");
            fprintf(logFile,"child process was termenated ,Time:%s\n",time_str);
            fclose(logFile);
                    }
        if (status == 1)
        {
            // error
            printf("\nBackground job exited\n");
        }
    }
    return;
}

void executing(char **res,char *last_string){
char temp[5]="";
pid_t pid=fork();
int  status;

        if (pid  < 0) {
            printf("ERROR: forking child process failed\n");
            exit(1);
        }
        //in child
        else if (pid == 0) {
            if (execvp(res[0], res) < 0) {
                printf("ERROR: exec failed\n");
                exit(1);
            }
        }
         else {
            if(strcmp(last_string,"&" )!= 0)
                wait(NULL) ;
            //background process
            else{
                signal(SIGCHLD, hand);
            }
        }
        free (res);
        strcpy(last_string,temp);
}

void split_strings(char* str){

    char ** res  = NULL;
    char *  spaceString= strtok (str, " ");
    int i,size = 0;
    char last_string[5];
    /* split string to 'res' */
    while (spaceString) {
      res = realloc (res, sizeof (char*) * ++size);
      if (res == NULL)
        exit (-1); /* memory allocation failed */

        if(strcmp(spaceString,"&")!=0)
            res[size-1] = spaceString;
        else
            strcpy(last_string,spaceString);

      spaceString = strtok (NULL, " ");
    }
    /* realloc one extra element for the last NULL */
    res = realloc (res, sizeof (char*) * (size+1));
    res[size] = 0;
    //the execute
    executing(res,last_string);
}

int main()
{
    char user_input[100];
    fclose(fopen("logfile.txt", "w"));  // clear the file
    while(1){
    printf("shell> ");
    //taking the input, and if it is null exit
    if (fgets (user_input, sizeof (user_input), stdin) == NULL || strcmp (user_input, "\n") == 0 )
    {
       user_input[ strcspn( user_input, "\n" ) ] = '\0';
    }
    else {
            user_input[ strcspn( user_input, "\n" ) ] = '\0';
            if(strcmp(user_input,"exit")==0)
                exit(0);
            else{
            split_strings(user_input);}
        }
    }
    return 0;
}
