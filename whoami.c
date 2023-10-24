#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern char **environ;

int environ_var(void){  // environ global and extern variable
    char** env = environ;

    while (1){

        if (strncmp(*env, "LOGNAME=", 8) == 0){
            char *username = *env;
            username+=8;
            printf("%s\n", username);
            return 1;
        }
        env++;
    }

    return 1;
}



//////////////////////////////////////////////////////////////////


int set_value(char target[], char value, size_t size){
    int elements = size;
    for (int i = 0; i < elements; i++){
        target[i] = value;

    }
    return 0;
}


int environ_file(void){ // environ file in proc and pid dir

    pid_t pid = getpid(); // gets current program pid
    char env_path[30]; // store environ file path '/proc/pid/environ'

    snprintf(env_path, sizeof(env_path), "/proc/%d/environ", pid); // stores environ file path '/proc/pid/environ' in env_path var

    FILE *file = fopen(env_path, "r"); // opens environ file on read mode

    char character; // stores each character read in file
    char var[9];
    int tracker = 0;  // works as indice to var array var, to store characters in var array var
    int position;
    char username[128]; // stores username when found, with max buffer as 128 characters 
    int logname_found = 0;


    while (character != EOF) {
        character = (char) fgetc(file);   // stores char returned by fgetc func in character var
        var[tracker] = character;  //stores character var value in var 'var' array using tracker as indice

        if (tracker == 4) { // if there's (from 0) 7 values including 0 itself, 0 to 6 = 7 values counting from zero

            if (strncmp("USER=", var, 5) == 0){ // when array var lenght is 7 and if array var elements characters is same to "logname" string
                logname_found = 1;

                for (int i = 0; 1; i++){
                    character = fgetc(file);

                    if ((int) character == 0){
                        username[i] = '\0';
                        break;
                    }
                    username[i] = (char) character;
                }
                printf("%s\n", username);
                return 0;
            }

            tracker++;

        }

        else if (tracker == 7) { // if there's (from 0) 7 values including 0 itself, 0 to 6 = 7 values counting from zero
            var[-1] = '\0';

            if (strncmp("LOGNAME=", var, 8) == 0){ // when array var lenght is 7 and if array var elements characters is same to "logname" string
                logname_found = 1;

                for (int i = 0; 1; i++){
                    character = fgetc(file);

                    if ((int) character == 0){
                        username[i] = '\0';
                        break;
                    }
                    username[i] = (char) character;
                }
                printf("%s\n", username);
                return 0;
            }

            set_value(var, '\0', sizeof(var));

            tracker = 0; // changes var indice to start putting characters in var variable starting from 0 or element zero

            position++; // current cursor location plus one or current cursor position +1

            fseek(file, position, SEEK_SET); // move cursor position to next byte or  (+1) location

        }

        else {
            tracker++;
        }
    }

    return 1;
}


///////////////////////////////////////////////////////////



int str_len(char p[]){
    int i;
    for (i = 0; 1; i++){
        if (p[i] != '\0'){
       } else{
            break;
       }
    } return i;
}


int uid_gid(void){ // passwd file in etc dir

    FILE *file = fopen("/etc/passwd","r");

    if (file == NULL){
        perror("erro ao abrir o arquivo");
    }

    int c; // character returnd by fgetc function
    int n = 0; // how many char on each point or characters between a point
    int l = 0;
    int lchrs = 0;
    int pts = 0; // points or how many points
    uid_t uid = getuid();
    gid_t gid = getgid();

    int uid_found = 0;
    int gid_found = 0;
    char rd[128]; // characters readed or chars readed between a point
    char username[128]; //(char*) malloc(sizeof(char)*128);
    int count = 0; // rd array indice, indice to use to store returned char by fgetc in rd array

    while (c != EOF){
        c = fgetc(file);
        n++;
        lchrs++;

        if (n <= 128){
            if (c != 58){
                rd[count] = c;
                count++;
            }
        }

        if (c == 58){ // if character is : decimal 58
            pts++;

            if (pts == 1){ // if its first point :
                snprintf(username, sizeof(username), "%s\0", rd);
            }
            else if (pts == 3){ // if its third point :
                char uidstr[11]; // store uid got in getuid func converted to string
                snprintf(uidstr, sizeof(uidstr), "%d\0", uid); // convertes uid got by getuid function in string
                int uid_len = str_len(uidstr); //count; // tamanho de uid ou uid lenght

                // compares if got from getuid function and uid got from passwd file are same
                if (strncmp(uidstr, rd, uid_len) == 0){
                    uid_found = 1;
                } else {
                }
            }
            else if (pts == 4){ // if its fourth point :
                char gidstr[11]; // store gid got in getgid func converted to string
                snprintf(gidstr, sizeof(gidstr), "%d\0", gid); // convertes gid got by getgid function in string
                int gid_len = str_len(gidstr); //count; // tamanho de gid ou gid lenght

                // compares if got from getgid function and gid got from passwd file are same
                if (strncmp(gidstr, rd, gid_len) == 0){
                    gid_found = 1;
                    if (uid_found == 1 && gid_found == 1){ // if uid and gid was found
                        printf("%s\n", username);
                        return 0;
                    } else {
                        //printf("error: was not possible to get the current username!\n");
                        return 1;
                    }
                }
            }
            count = 0;
            n = 0;
            memset(rd, '\0', sizeof(rd));
        }
        else if (c == 10){ // if character is \n decimal 10
            n = 0;
            count = 0;
            memset(rd, '\0', sizeof(rd));
            l++;
            lchrs = 0;
            pts = 0;
        }
        else{
        }
    }
    fclose(file);
    return 1;
}


//////////////////////////////////////////////////////////////////


int main(void){
#if defined(__linux__)

    if (uid_gid() != 0){
        if (environ_var() != 0){
            environ_file();
        } else{
            printf("error getting username\n");
            return 1;
        }
    }
    return 0;
#else
    printf("os error: operating system must be linux\n");
    return 1;
#endif
}

