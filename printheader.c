#include <stdio.h>
#include "printheader.h"

void printHeader(const char *title) {
    printf("====================================\n");
    printf("    %s\n", title);
    printf("====================================\n");
}

void printHeaderx(const char *title) {
    printf("==============================================\n");
    printf("  %s\n", title);
    printf("==============================================\n");
}

void printexHeader(const char *title, int number) {
    printf("====================================\n");
    printf("          %s%02d\n", title, number);
    printf("====================================\n");
}

void printCong(const char *username, int lastCompleted, int totalExercises) {
    printf("\n====================================\n");
    printf("      CONGRATULATIONS, %s!\n", username);
    printf("    %d / %-2d exercises completed\n", lastCompleted, totalExercises);
    printf("====================================\n");
}
