#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SHOW_MAILS 2
#define SEND_NEW_MAIL 4

#define SORT_BY_ADRESSES 11
#define SORT_BY_DATE 12

void print_menu(){
    printf("PROGRAM USAGE:\n");
    printf("<sender|date> | <email> <title> <content> \n");
}

struct command{
    char* str;
    int n;
};

struct command commands[] = {
    {"date", SORT_BY_DATE},
    {"sender", SORT_BY_ADRESSES}
};

int compare(const void *s1, const void *s2){
    const struct command *e1  = s1; 
    const struct command *e2  = s2;

    return strcmp(e1->str, e2->str); 
}

struct command* map_command_to_int(char * arg){
    struct command *result, key = {arg};
    result = bsearch(&key, commands, sizeof(commands)/sizeof(commands[0]),
                        sizeof commands[0], compare);

    return result;
}

void show_mails_handle(char* sort_by){
    struct command *result = map_command_to_int(sort_by);

    if(result == NULL){
        fprintf(stderr,"Unknown command!\n");
        print_menu();
        exit(EXIT_FAILURE);
    }

    FILE* print_mails;
    int c;
    char line[1024];

    switch (result->n) {
        case SORT_BY_ADRESSES:
            print_mails = popen("mail -H | sort -k 3", "r");
            while (fgets(line, sizeof(line), print_mails) != NULL) {
                printf("%s", line);
            }
        break;

        case SORT_BY_DATE:
            print_mails = popen("mail -H", "r");
            while (fgets(line, sizeof(line), print_mails) != NULL) {
                printf("%s", line);
            }
        break;

        default:
            print_menu();
            exit(EXIT_FAILURE);
            break;
    }
}

void send_mail(char* email_adress, char* title, char* content){
    FILE* sender;
    char line[1024];
    int cx = snprintf(line, sizeof(line),"echo \"%s\" | mail -s \"%s\" %s", content, title, email_adress);
    puts(line);
    
    if(cx != -1){
        sender = popen(line,"w");

        if(sender == NULL){
            printf("ERROR: Sender could not sent new mail!\n");
            exit(EXIT_FAILURE);
        }
   }
}


int main(int argc, char ** argv){
    switch (argc) {
        case SHOW_MAILS:
            printf("Showing mails...\n");
            show_mails_handle(argv[1]);
        break;

        case SEND_NEW_MAIL:
            send_mail(argv[1], argv[2], argv[3]);
            return 0;   
        break;

        default:
            fprintf(stderr, "Wrong number or arguments!");
            print_menu();
            exit(EXIT_FAILURE);
    }


    return  0;
}