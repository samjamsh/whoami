/*  whoami - Equivalent to 'id -un',  it prints actually user name associated with the uid version 1.1

    Copyright (C) 2023-2024 Free and Open Source Software "No Copyrights"

    This program is free software: you can redistribute it or modify it under the terms of the project that means you can do whatever you want with this program.

    Differences with Linux whoami:
       This is a different code what it means that both programs may do the samething but in different ways
       This is a not GNU Core utilities (coreutils) program, it means that I created this program from zero by myself

       By Yassir Daniel also known as Sam Jamsh

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VERSION "1.1"
#define OWNER "Sam Jamsh"
#define EMAIL "th3cyb3rguy@protonmail.com"
#define WEBPAGE "https://github.com/samjamsh/whoami"

#define SUCCESS 0
#define ERROR 1

#define true 1
#define false 0

typedef char bool;

extern char **environ;



unsigned short intlen(int number)
{

unsigned short lenght;

while (number != 0)
{
    number /= 10;
    lenght++;
}

return lenght;
}



char* int_tostrn(unsigned int number, unsigned short buflen)
{

short ld = 0;

char* string = malloc(sizeof(char) * (buflen + 1));

unsigned short i = 0;


while (number != 0)
{
    ld = number % 10; // last digit
    number = number / 10; // update number

    string[i] = ld + 48;

    i++;
}
string[i] = '\0';

return string;
}



unsigned short strlen_(char *string){

    unsigned short counter;
    unsigned short ushort_lim = 65535;

    for (counter = 0; counter < ushort_lim; counter++)
    {
        if (*string++ == 0)
        {
            return counter;
        }
    }
    return 0;
}



void invert(char *array, unsigned short limit)
{

unsigned short first_pos = 0; // first digit position
unsigned short last_pos = limit - 1; // last digit position
char temp_var;

unsigned short answer;
answer = (unsigned short) limit / 2; // quotient only

for (unsigned short i = 0; i < answer; i++)
{

    temp_var = array[last_pos];
    array[last_pos] = array[first_pos];
    array[first_pos] = temp_var;

    last_pos--;
    first_pos++;
}

return;
}



char* new_merge_strings(char *string1, char *string2, char *string3)
{
    if (string1 == NULL || string2 == NULL || string3 == NULL)
    {
        puts("err: string1/2/3 NULL");
        return NULL;
    }

    unsigned short string1_lenght = strlen_(string1);

    unsigned short string2_lenght = strlen_(string2);

    unsigned short string3_lenght = strlen_(string3);

    unsigned short allocate = string1_lenght + string2_lenght + string3_lenght;

    char *ptr = malloc(sizeof(char) * allocate + 1);

    unsigned short i;

    for (i = 0; i < string1_lenght; i++)
    {
        ptr[i] = string1[i];
    }

    for (i = i; i < string1_lenght + string2_lenght; i++)
    {
        ptr[i] = string2[i - string1_lenght];

    }

    for (i = i; i < allocate; i++)
    {
        ptr[i] = string3[i - (string1_lenght + string2_lenght)];
    }

    ptr[allocate] = '\0';

return ptr;
}



int envfile(void)
{
    char *user = "USER=";
    char *logname = "LOGNAME=";

    unsigned short i = 0;  // (user) found chrs
    unsigned short ii = 0; // (logname) found chars
    char c = 1;

    bool user_found = false;
    bool logname_found = false;
    bool found = false;


    pid_t pid = getpid();

    unsigned int pid_ = (unsigned int) pid;

    unsigned short pid_len = intlen(pid_);

    char *strpid = int_tostrn(pid_, pid_len);

    unsigned short strpid_len = strlen_(strpid);

    invert(strpid, strpid_len); // reverse strpid

    char *path = new_merge_strings("/proc/", strpid, "/environ");

    free(strpid);


    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("file error");
        return 0;
    }


    while (c != EOF)
    {
        c = fgetc(file);

        if (i < 4 || i != 5)
        {

            if (i > 0 && c != user[i])
            {
                i = 0;
            }

            if (c == user[i])
            {
                i++;
            }
        }
        else {

            if (c != 0)
            {
                printf("%c", c);
            }
            else {
                printf("\n");
                found = true;
                break;
            }
        }


        if (ii < 7 || ii != 8)
        {
            if (ii > 0 && c != logname[ii])
            {
                ii = 0;
            }

            if (c == logname[ii])
            {
                ii++;
            }

        }
        else {

            if (c != 0)
            {
                printf("%c", c);
            }
            else {
                printf("\n");
                found = true;
                break;
            }
        }
    }

    return found;
}


bool cmpstr(char *string1, char *string2)
{
    unsigned short i = 0;
    bool same = 0;

    while (string1[i] != '\0' && string2[i] != '\0')
    {

        if (string1[i] != string2[i])
        {
            same = 1; // different
            break;
        }

        i++;
    }
    return same;

}



bool cmpstrn(char *string1, char *string2, unsigned int number)
{
    unsigned short i = 0;
    bool same = 0;

    while ((string1[i] != '\0' && i < number) || (string2[i] != '\0' && i < number))
    {

        if (string1[i] != string2[i])
        {
            same = 1; // different
            break;
        }

        i++;
    }
    return same;

}



int environ_var(void){
    char** env = environ;

    while (1){

        if (cmpstrn(*env, "USER=", 5) == 0){
            *env+=5;
            printf("%s\n", *env);
            return true;
        }

        if (cmpstrn(*env, "LOGNAME=", 8) == 0){
            *env+=8;
            printf("%s\n", *env);
            return true;
        }

        env++;
    }

    return SUCCESS;
}



bool get_column(char* ptr, unsigned short column, char separator, unsigned short* ret){

    unsigned col = 0;
    char chr;
    unsigned short i = 0;
    unsigned short n = 0;

    unsigned short begin = 0;
    unsigned short end;

    ret[0] = 0;
    ret[1] = 0;


    while (i < strlen_(ptr)){

        chr = ptr[i];

        n++;

        if (chr == separator)
        {
            col++;

            end = n;
            if (col == column)
            {
                ret[0] = begin;
                ret[1] = --end;
                return SUCCESS;
            }
            begin = n;
        }

        i++;

    }
    return ERROR;

}



bool print_range(char *ptr, unsigned short begin, unsigned short end){
    for (unsigned short i = begin; i < end; i++)
    {
        printf("%c", ptr[i]);
    }
    return SUCCESS;

}



bool fill_range(char *ptr, char *buffer, unsigned short ptr_begin, unsigned short ptr_end){


    unsigned short lenght = ptr_end - ptr_begin;

    for (unsigned short i = 0; i < lenght; i++)
    {
        buffer[i] = ptr[ptr_begin + i];

    }

    return SUCCESS;
}



short decimal(char value)
{
    if (value >= 48 && value <= 57)
        return value - 48;

    else
        return -1;
}



unsigned short strn_to_int(char *string, unsigned short str_len)
{

unsigned short v = 1;
unsigned short total = 0;
unsigned short value;
short number;

int i = str_len - 2; // int


while (i >= 0)
{

    number = decimal(string[i]);

    if (number == -1)
        return 0; //-1;

    i--;

    v = v * 10;

    value = number * v;

    total += value;
}

number = decimal(string[str_len -1]);

if (number == -1)
    return 0; //-1;

total = total + number;

return total;

}



int uid_gid(void)
{

    FILE *file = fopen("/etc/passwd","r");

    if (file == NULL){
        perror("error opening passwd file");
       return 0;
    }

    uid_t uid = getuid();
    gid_t gid = getgid();

    bool uid_bool = false;
    bool gid_bool = false;

    bool found = false;

    unsigned short iuid;
    unsigned short igid;

    unsigned short column_range;

    unsigned short allocated_uid;
    unsigned short allocated_gid;


    unsigned short *ret = malloc(sizeof(unsigned short) * 2);

    if (ret == NULL)
    {
        puts("error alocating ret memory");
        return 0;
    }


    char *struid = malloc(sizeof(char));
    char *strgid = malloc(sizeof(char));

    if (struid == NULL || strgid == NULL)
    {
        puts("error alocating memory to str uid & gid");
        return 0;
    }

    char *line = NULL;
    size_t bufsize = 0;

    while(getline(&line, &bufsize, file) != -1)
    {


    // uid
    get_column(line, 3, ':', ret);

    column_range = ret[1] - ret[0];

     if (column_range > allocated_uid)
        {
            allocated_uid = column_range;

            struid = realloc(struid,
sizeof(char) * allocated_uid);

        }

    fill_range(line, struid, ret[0], ret[1]);

    iuid = strn_to_int(struid, column_range);



    // gid
    get_column(line, 4, ':', ret);

    column_range = ret[1] - ret[0];

    if (column_range > allocated_gid)
        {
            allocated_gid = column_range;

            strgid = realloc(strgid,
sizeof(char) * allocated_gid);

        }

    fill_range(line, strgid, ret[0], ret[1]);

    igid = strn_to_int(strgid, column_range);


    uid_bool = uid == (uid_t) iuid;
    gid_bool = gid == (gid_t) igid;

    if (uid_bool == true && gid_bool == true)
    {
        get_column(line, 1, ':', ret);

        column_range = ret[1] - ret[0];

        print_range(line, ret[0], ret[1]);

        printf("\n");

        found = true;
        break;
    }

    }

    free(ret);
    free(struid);
    free(strgid);
    free(line);
    fclose(file);
    return found;
}



int main(int argc, char *argv[]){
#if defined(__linux__)

if (argc > 1){
    if (cmpstr(argv[1], "--help\0") == 0)
    {
        printf("Usage: %s [OPTION]...\nDisplays the user name associated to actual user id (uid) same as id -un.\n\n      --help        display this help message\n      --version     display the program version information\n\nReporting Bugs and complete Documentation in <%s>\nCopyright: this is a free software, you are free to change and redistribute it.\nWritten by %s", argv[0], WEBPAGE, OWNER); // displays help message
    }
    else if (cmpstr(argv[1], "--version\0") == 0)
    {
        printf("Whoami (NGCU) version %s\nCopyright (C) 2024 Free and Open Source Software.\nThis is a free software: you are free to modify and redistribute it.\n\nWritten by %s aka Yassir, instagram> @sam.jamsh, email> %s\n ", VERSION, OWNER, EMAIL); // displays version information
    }
    else {
        printf("%s: unknown option '%s'\nTry '%s --help' for more info.", argv[0], argv[1], argv[0]);

    }
    return 0;
}
else {



    // passwd file
    if(!uid_gid()){

        // environ file
        if (!envfile()){ // unnecessary func

            // environ variable
            if (!environ_var()){ // unnecessary func

                puts("error: failed getting username");
                return ERROR;
            }
        }
    }

}
return SUCCESS;

#else
    puts("wrong operational system: you must compile this code on a linux distribution");
    return ERROR;
#endif
}
 
