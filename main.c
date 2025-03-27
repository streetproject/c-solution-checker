#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "functions.h"
#include "printheader.h"

int main() {
    char username[MAX_NAME_LEN];

    setUsername(username);
    welcomeUser(username);
    sleep(1);


    char input[10];
    while (1) {
        welcomeUser(username);
        display_interface();
        printf(" Type 'commands' to view all available options.\n");
        printf("\n> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline

        toLowerCase(input);

        if (processInput(input, username)) {
            continue;
        }
        if (strcmp(input, "1") == 0) {
            startExercises(username);
        } else if (strcmp(input, "2") == 0) {
            viewprogress();
        } else if (strcmp(input, "3") == 0) {
            resetprogress();
        } else if (strcmp(input, "4") == 0) {
            eraseprogress();
        } else if (strcmp(input, "5") == 0) {
            openSettings();
        } else if (strcmp(input, "6") == 0) {
            clear_screen();
            printf("\n[::] Goodbye, %s! C you next time!\n", username);
            sleep(1);

            printf("Exiting program...\n");
            sleep(2);
            clear_screen();
            break;
        } else {
            printf("\n[!] Unknown command. Type 'help' or 'exit'.\n");
            sleep(1);
        }
    }
    return 0;
}
