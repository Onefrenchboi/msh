#include "filesystem.h"



int main() {
    printf(YELLOW("Welcome to msh ! This is a simple shell implementation, with a few built-in commands.\n"));
    printf(GREEN("Type \"help\" to get started !.\n"));

    loop();


}


void loop(){
    char *line;
    char *command[MAX_COMMAND_SIZE];
    int status;
    while(1){
        printf("|-> ");
        line = read_line();
        create_command_from_line(line, command);
        //status = run(command);
        free(line);
    }
}

/*
* Reads a line (duh)
* Returns the line read, else 0 if error while reading
*/
char *read_line() {
    char *buffer = malloc(MAX_LINE_SIZE);
    if (!buffer){
        return NULL;
    }
    if (fgets(buffer, MAX_LINE_SIZE, stdin)) {
        return buffer;
    }
    free(buffer);
    return NULL;
}


/*
* Creates a command from a line
* Returns the args read, else 0 if error while reading
*/
 void create_command_from_line(char *line, char *command[]){
    int i = 0;
    while(*line){

        //Replace spaces with null terminator, to cut up words.
        while(*line == ' '){
            *line ='\0';
            line++;
        }

        //If EOF reached, break
        if (*line == '\0') {
            break;
        }

        command[i] = line;
        while (*line && *line != ' ') {
            line++;
        }
        i++;
    }
    command[i] = NULL; //close with null
    return;
}
    
