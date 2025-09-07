#include "filesystem.h"



int main() {
    printf(YELLOW("Welcome to msh ! This is a simple shell implementation, with a few built-in commands.\n"));
    printf(GREEN("Type \"help\" to get started !.\n"));

    //Run the main loop
    loop();


}


/*
* The main loop, that reads each input
*/
void loop(){
    char *line;
    char *command[MAX_COMMAND_SIZE];
    int status;
    while (1){
        printf("|-> ");
        line = read_line();
        create_command_from_line(line, command);
        status = run(command);
        free(line);

        // exit ? not sure yet
        if (status==2){
            return;
        }
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
    //remove trailing newline
    line[strcspn(line, "\n")] = 0;
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




/*
* Runs a command
* Returns the status (i.e. if the command failed, succeeded)
*/

int run(char *command[]){
    pid_t child_pid, wpid;
    int status;


    //Test if alrd built-in, and, if so, execute and return
    if (execute_built_in_function(command[0])){
        return 1;
    }



    return 0;
    
}

