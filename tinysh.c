#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//Function declarations
char *readUserInput(FILE* fp, size_t size);
char **parseUserInput(char *input, int *num);
int chd(char *dirPath){
   return chdir(dirPath);
}

//Main Function
int main() {
   pid_t child_pid;
   int stat_loc;
   char **usrCmd = NULL;
   

   while (1){
      char *input = NULL;
         int num = 0;

         printf("tinysh>");
      input = readUserInput(stdin, 10);

      //IF user enter exit, exit the tinysh shell or ctrl + c works as well  
         if (!strncmp(input, "exit ", 5) || !strcmp(input, "exit")){
            free(input);
            break;
         }

      usrCmd = (char **) parseUserInput(input, &num);
         if (num == 0){
            free(input);
            continue;
         }
//When users enter cd ( change directory )
         if (!strcmp(usrCmd[0], "cd")){
            if (chd(usrCmd[1]) < 0){
               printf("No such directory exists\n");
            }
         free(input);
         free(usrCmd);
         continue;
         }
//library function, you can check what does it for
      child_pid = fork();
      if (child_pid == 0){
         //xecutes the user cmd's
         if (execvp(usrCmd[0], usrCmd) < 0){
            printf("unknown command %s\n", usrCmd[0]);
         }
            //exit the process
            exit(0);
      }
      else{
         wait(NULL);
      }

      free(input);
      free(usrCmd);
   }

   return 0;
}

//Function definitions
// This function for to take user's input
char *readUserInput(FILE* fp, size_t size){
   char *str;
   int ch;
   size_t len = 0;
   str = realloc(NULL, sizeof(char)*size); // initial size
   if(!str)return str;
   while('\n' != (ch = fgetc(fp))){
      str[len++]=ch;
         if(len == size){
            str = realloc(str, sizeof(char)*(size += 16));
            if(!str)return str;
         }
   }

   str[len++] = '\0';
   return realloc(str, sizeof(char)*len);
}

// This function for to parse user's input
char **parseUserInput(char *input, int *num){
   char *tokens = NULL;
   char **cmd = malloc(1024 * sizeof(char *));
   int idx = 0;
   char *delimiter = " ";
   char *savepointer = NULL;


   tokens = strtok_r(input, delimiter, &savepointer);
   while (tokens != NULL){
      cmd[idx++] = tokens;
      tokens = strtok_r(NULL, delimiter, &savepointer);
   }
   cmd[idx] = NULL;
   *num = idx;
   return cmd;
}
