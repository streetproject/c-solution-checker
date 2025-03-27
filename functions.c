#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include "functions.h"
#include "printheader.h"

int autoOpenSubjects = 0;

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int processInput(char *input, const char *username) {
    // Remove newline from input
    input[strcspn(input, "\n")] = 0;  

    // Check input against possible commands
    if (strcmp(input, "start") == 0) {
        startExercises(username);
        return 1;
    } 
    else if (strcmp(input, "home") == 0) {
        return 0;
    } else if (strcmp(input, "about") == 0) {
        about();
        return 1;
    } else if (strcmp(input, "check") == 0) {
        checkAllExercises();
        return 1;
    } else if (strcmp(input, "list") == 0) {
        listExercises();
        return 1;
    } 
    else if (strcmp(input, "progress") == 0) {
        viewprogress();
        return 1;
    } 
    else if (strcmp(input, "reset") == 0) {
        resetprogress();
        return 1;
    } 
    else if (strcmp(input, "erase") == 0) {
        eraseprogress();
        return 1;
    } 
    else if (strcmp(input, "settings") == 0) {
        openSettings();
        return 1;
    } 
    else if (strcmp(input, "commands") == 0 || strcmp(input, "help") == 0) {
        commands();
        return 1;
    } 
    else if (strcmp(input, "exit") == 0) {
        clear_screen();
        printf("\n[::] Goodbye, %s! C you next time!\n", username);
        sleep(1);
        printf("Exiting program...\n");
        sleep(2);
        clear_screen();
        exit(0);
    }
    return (0);
}

void setUsername(char *username) {
    clear_screen();
    printHeader("Welcome to the C Exam System!");
    printf("    What should we call you?\n");
    printf("\n> ");

    while (1) {
        fgets(username, MAX_NAME_LEN, stdin);
        username[strcspn(username, "\n")] = 0;

        // Check if username is non-empty and contains only valid characters
        int valid = (*username != '\0');

        for (int i = 0; username[i] && valid; i++) {
            if (!((username[i] >= 'a' && username[i] <= 'z') ||
                  (username[i] >= 'A' && username[i] <= 'Z') ||
                  (username[i] >= '0' && username[i] <= '9'))) {
                valid = 0;
            }
        }

        if (valid) break;

        printf("Invalid username. Use only letters and numbers:\n");
        printf("\n> ");
    }
}

void openSettings() {
    while (1) {
        clear_screen();
        printf("\n========== SETTINGS ==========\n");
        printf("  [1] Auto-check all exercises\n");
        printf("  [2] Auto-open exercise text files [%s]\n", autoOpenSubjects ? "ON" : "OFF");
        printf("  [3] Show available commands\n");
        printf("  [4] Return to main menu\n");
        printf("==============================\n");

        char choice = getUserChoice("\n Enter your choice (1-4): ", "1234");

        switch (choice) {
            case '1': {
                checkAllExercises();
                break;
            }
            case '2': {
                autoOpenSubjects = !autoOpenSubjects;
                printf("\n[✔] Auto-open Subject File is now %s\n", autoOpenSubjects ? "ON" : "OFF");
                sleep(2);
                break;
            }
            case '3': {
                commands();
                break;
            }
            case '4':
                return;  // Exit the settings menu
        }
    }
}

void commands() {
    clear_screen();
    printf("\n========== Commands Menu ==========\n");
    printf(" home     - Return to the main menu\n");
    printf("\n start    - Begin new exercises\n");
    printf(" check    - Auto-check all exercises\n");
    printf(" list     - Show all available exercises\n");
    printf(" progress - Check your progress\n");
    printf(" reset    - Reset progress (keep solutions)\n");
    printf(" erase    - Erase all progress permanently\n");
    printf("\n help     - Show help information\n");
    printf(" about    - View program version and info\n");

    printf("\n commands - Show all available commands\n");
    printf(" settings - Open settings\n");
    printf(" exit     - Exit the program\n");
    printf("===================================\n");

    printf("\n[>] Press Enter to return...");
    getchar();
}

void welcomeUser(const char *username) {
    int maxExercises = countExercises();
    int lastCompleted = getLastCompletedExercise();

    clear_screen();
    printHeader("Welcome to the C Exam System!");
    
    printf("  Hello, %s!\n", username);
    printf("  Progress: %d/%d exercises completed\n", lastCompleted, maxExercises);
}

void display_interface() {
    printHeader("        Main menu");
    printf("Type 'start' to begin, 'settings', 'help' or 'exit' to quit.\n");

    printf("\n What would you like to do?\n");
    printf("  [1] Start/Resume Exercises\n");
    printf("  [2] View Progress\n");
    printf("  [3] Reset Progress (Keep your solutions)\n");
    printf("  [4] Start over (! Erase all solutions and progress !)\n");
    printf("  [5] Settings\n");
    printf("  [6] Exit program\n");
    printf(" Enter your choice (1-6):\n");

    printf("\n--8<----------------------------------------------\n");
}

void about() {
    clear_screen();
    printf("\n========== About ==========\n");
    printf(" Program: C Solution Checker\n");
    printf(" Version: 1.0\n");
    printf(" Author: MQ\n");
    printf("\n This program is designed to help users improve their coding skills\n");
    printf(" through interactive exercises, automated checks, and progress tracking.\n");
    printf("\n Features include:\n");
    printf(" - Guided exercises with automated verification\n");
    printf(" - Progress tracking to monitor your learning journey\n");
    printf(" - Options to reset or erase progress as needed\n");
    printf("\n Use 'help' to view detailed information on available commands.\n");
    printf("===================================\n");

    printf("\n[>] Press Enter to return...");
    getchar();
}

void startExercises(const char *username) {
        int maxExercises = countExercises();
        int lastCompleted = getLastCompletedExercise();
    
        if (maxExercises == 0) {
            clear_screen();
            printf("Error: No exercises found! Exiting...\n");
            sleep(2);
            return;
        }

        // Ask if they want to resume only if progress is greater than 0
        if (lastCompleted > 0) {
            char input[10];  // Buffer for user input
            while (1) { 
                clear_screen();
                
                printf("\n****************************************\n");
                printf("*         PROGRESS CHECKPOINT          *\n");
                printf("****************************************\n");
                printf("  You have completed up to Exercise %02d\n", lastCompleted);
                printf("****************************************\n");
                printf("*  Do you want to continue?            *\n");
                printf("*  [Y] Yes, continue                   *\n");
                printf("*  [N] No, go back to menu             *\n");
                printf("*  [R] Restart from beginning          *\n");
                printf("****************************************\n");
                printf("\n> ");
                
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; 
                
                toLowerCase(input); 
        
                if (strcmp(input, "r") == 0) {
                    lastCompleted = 0;  // Reset progress
                    saveProgress(0);  // Overwrite progress.txt
                    printf("\n[↻] Restarting from the beginning...\n");
                    sleep(2);
                    break;
                } 
                else if (strcmp(input, "y") == 0) {
                    printf("\n[✔] Resuming from Exercise %d...\n", lastCompleted);
                    sleep(2);
                    break;
                } 
                else if (strcmp(input, "n") == 0) {
                    return;  // Go back to the home menu
                } 
                else {
                    printf("\n[!] Invalid input. Please enter 'y', 'n', or 'r'.\n");
                    sleep(1);
                }
            }
        }
        
        while (1) {
            int exerciseNumber = lastCompleted;
        
            while (exerciseNumber < maxExercises) {
                if (autoOpenSubjects) {
                    openExerciseFile(exerciseNumber, autoOpenSubjects);
                }
                compileAndGenerateReferenceOutput(exerciseNumber);
                clear_screen();
                int correct = 0;  // Track correctness
                while (!correct) {
                    clear_screen();
                    printexHeader("Coding Task ex", exerciseNumber);
                    loadExercise(exerciseNumber);
            
                    printf("\n\n[!] Write your solution in: ex%02d/ex%02d.c\n", exerciseNumber, exerciseNumber);
                    printf("\n******************************\n");
                    printf("\n[>] Press Enter when ready to submit...");
                    getchar();
                    
                    while (1) {  // Keep prompting until valid input
                        clear_screen();
                        printexHeader("Coding Task ex", exerciseNumber);
                        printf("|   Submit Your Solution? (y/n/h)   |\n");
                        printf("+-----------------------------------+\n");
                        printf("| [Y] Yes, submit now               |\n");
                        printf("| [N] No, cancel                    |\n");
                        printf("| [H] Main Menu                     |\n");
                        printf("+-----------------------------------+\n");
                        printf("\n> ");
            
                        char input[20];
                        fgets(input, sizeof(input), stdin);
                        input[strcspn(input, "\n")] = 0;

                        toLowerCase(input);

                        if (processInput(input, username)) {
                            continue;
                        }
                        if (strcmp(input, "h") == 0 || strcmp(input, "home") == 0) {
                            return; // Exit to home
                        } 
                        if (strcmp(input, "y") == 0) {
                            clear_screen();
                            printexHeader("Coding Task ex", exerciseNumber);
                            printf("\n[~] Grading your solution, please wait...\n");
                            sleep(2);
                            clear_screen();
                            printexHeader("Coding Task ex", exerciseNumber);
                            correct = compileAndCheckSolution(exerciseNumber);
            
                            if (correct) {
                                printf("\n[✔] Moving to the next exercise.\n");
                                saveProgress(exerciseNumber + 1);
                            } else {
                                printf("\n[✘] Incorrect or compilation failed. Try again.\n");
                                printf("\n******************************\n");
                                printf("[>] Press Enter to try again...");
                                getchar();
                            }
                            break;  // Exit prompt loop on valid submission
                        } 
                        if (strcmp(input, "n") == 0)  {
                            clear_screen();
                            printexHeader("Coding Task ex", exerciseNumber);
                            printf("\n[✘] Submission canceled.\nEdit your solution and try again when you're ready.\n");
                            printf("\n******************************\n");
                            printf("[>] Press Enter to return...");
                            getchar();
                            break;
                        } 
            
                        // If input is invalid, show error and reprompt
                        printf("\n[!] Unknown command. Type 'y', 'n', or 'h'.\n");
                        sleep(1);
                    }
                }
                printf("\n--8<-----------------------------------------------------\n");
                printf("\n[!] Press Enter to continue...");
                getchar();
                exerciseNumber++;
            }

            // Reset progress to 0
            saveProgress(0);

            char input[10];
            lastCompleted = getLastCompletedExercise();
            while (1) {
                clear_screen();
                printCong(username, exerciseNumber, maxExercises);
                printf("\n What would you like to do next?\n");
                printf(" - [1] Return to main menu\n");
                printf(" - [2] Restart from the beginning (Keep your solutions)\n");
                printf(" - [3] Start over (!~ Erase all solutions and progress ~!)\n");
                printf(" - [4] Settings\n");
                printf(" - [5] Exit program\n");
                printf("\n [H] Main Menu\n");
                printf("--8<-----------------------------------------------------\n");
                printf("\n> ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; // Remove newline

                toLowerCase(input);

                if (processInput(input, username)) {
                    continue;
                }
        if (strcmp(input, "1") == 0 || strcmp(input, "h") == 0 || strcmp(input, "home") == 0) {
            return;
        } else if (strcmp(input, "2") == 0) {
            printf("\n[↻] Restarting the exercises...\n");
                sleep(2);
                startExercises(username);
                return;
        } else if (strcmp(input, "3") == 0) {
            eraseprogress();
            return;
        } else if (strcmp(input, "4") == 0) {
            openSettings();
        } else if (strcmp(input, "5") == 0) {
            clear_screen();
            printf("\n[::] Take care %s! C you next time!\n", username);
            sleep(2);

            printf("Exiting program...\n");
            sleep(2);
            clear_screen();
            exit (0);
        } else {
            printf("\n[!] Unknown command. Type 'help' or 'exit'.\n");
            sleep(1);
        }
        }
    }
}

void loadExercise(int exerciseNumber) {
char filename[20];
snprintf(filename, sizeof(filename), "ex%02d/subject.txt", exerciseNumber);
FILE *file = fopen(filename, "r");
if (!file) {
    printf("[!] Error: Exercise description not found!\n");
    return;
}

char line[BUFFER_SIZE];
while (fgets(line, BUFFER_SIZE, file)) {
    printf("%s", line);
}
fclose(file);
}

int compileAndCheckSolution(int exerciseNumber) {
    char studentFile[50], mainFile[50], outputFile[50], command[256];
    int requiresMain = 0;  // Default: Assume user submits a full program

    snprintf(studentFile, sizeof(studentFile), "ex%02d/ex%02d.c", exerciseNumber, exerciseNumber);
    snprintf(mainFile, sizeof(mainFile), "ex%02d/solutions/main.c", exerciseNumber);
    snprintf(outputFile, sizeof(outputFile), "tmp/exercise.out");

    // If main.c exists, it means user submits only the function
    if (access(mainFile, F_OK) == 0) {
        requiresMain = 1;
    }

    // Compile accordingly
    if (requiresMain) {
        snprintf(command, sizeof(command), "gcc -Wall -Wextra -Werror %s %s -o %s", studentFile, mainFile, outputFile);
    } else {
        snprintf(command, sizeof(command), "gcc -Wall -Wextra -Werror %s -o %s", studentFile, outputFile);
    }

    if (system(command) != 0) {
        printf("\n[✘] Compilation failed! Fix errors before proceeding.\n");
        return 0;
    }

    // Run the compiled program and save output
    snprintf(command, sizeof(command), "%s > tmp/user_output.txt", outputFile);
    system(command);

    // Compare output with expected_output.txt
    snprintf(command, sizeof(command), "cmp -s tmp/user_output.txt ex%02d/expected_output.txt", exerciseNumber);
    if (system(command) == 0) {
        printf("\n[✔] Success!\n");
        printf("\n====================\n");
        printf(":: Your Output ::\n");
        system("cat -e tmp/user_output.txt");
        printf("\n\n:: Expected Output ::\n");
        snprintf(command, sizeof(command), "cat -e ex%02d/expected_output.txt", exerciseNumber);
        system(command);
        printf("\n====================\n");
        printf("$ = newline\n");
        system("rm -f tmp/exercise.out tmp/user_output.txt");
        printf("\n[✔] Solution is correct!\n");
        return 1;
    }

    printf("\n[✘] Output does not match reference. Try again!\n");

    printf("\n====================\n");
    printf(":: Your Output ::\n");
    system("cat -e tmp/user_output.txt");

    printf("\n\n:: Expected Output ::\n");
    snprintf(command, sizeof(command), "cat -e ex%02d/expected_output.txt", exerciseNumber);
    system(command);
    printf("\n====================\n");
    system("rm -f tmp/exercise.out tmp/user_output.txt");
    return 0;
}

int endsWithNewline(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    fseek(file, -1, SEEK_END);
    int lastChar = fgetc(file);
    fclose(file);

    return (lastChar == '\n');
}

int countExercises() {
    int count = 0;
    char dirName[20];

    while (1) {
        snprintf(dirName, sizeof(dirName), "ex%02d", count);
        struct stat sb;
        if (stat(dirName, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            count++;
        } else {
            break;
        }
    }
    return count;
}

void listExercises() {
    int totalExercises = countExercises();
    clear_screen();

    printHeader("Available Exercises");
    if (totalExercises == 0) {
        printf("\n[!] No exercises available.\n");
    } else {
        printf("\n Total available: %d\n\n", totalExercises);
        for (int i = 0; i < totalExercises; i++) {
            printf("   [ Exercise %02d ]\n", i);
        }
    }

    printf("\n--8<------------------------------------\n");
    printf("\n[>] Press Enter to return...");
    getchar();
}

char getUserChoice(const char *prompt, const char *validChoices) {
    char choice;
    do {
        printf("%s", prompt);
        scanf(" %c", &choice);
        while (getchar() != '\n');
    } while (!strchr(validChoices, choice));
    clear_screen();
    return choice;
}

void saveProgress(int lastCompleted) {
    FILE *file = fopen(PROGRESS_FILE, "w");
    if (file) {
        fprintf(file, "%d", lastCompleted);
        fclose(file);
    }
}

int getLastCompletedExercise() {
    FILE *file = fopen(PROGRESS_FILE, "r");
    if (!file) return 0;

    int lastCompleted = 0;
    fscanf(file, "%d", &lastCompleted);
    fclose(file);
    return lastCompleted;
}

void viewprogress() {
    int lastCompleted = getLastCompletedExercise();
    int totalExercises = countExercises();

    clear_screen();
    
    printf("****************************************\n");
    printf("*           PROGRESS OVERVIEW          *\n");
    printf("****************************************\n");
    printf("*  Completed:  %2d / %-2d exercises       *\n", lastCompleted, totalExercises);
    printf("****************************************\n");
    
    sleep(1);
    printf("\n[>] Press Enter to return to the menu...");
    getchar();
}

void resetprogress()
{
    int lastCompleted = getLastCompletedExercise();
    int totalExercises = countExercises();
    char input[10];  // Buffer for user input

    while (1) {
        clear_screen();
        printf("+-----------------------------------------+\n");
        printf("|           PROGRESS RESET MENU           |\n");
        printf("+-----------------------------------------+\n");

        printf("\n[i] You have completed %d out of %d exercises.\n", lastCompleted, totalExercises);

        if (lastCompleted == 0) {
            printf("\n[!] No progress to reset.\n");
            sleep(1);
            printf("\n[>] Press Enter to return to the menu...");
            fgets(input, sizeof(input), stdin);  // Wait for Enter key
            return; // Exit early if there's nothing to reset
        }

        printf("\n[!!] Are you sure you want to reset all progress? (y/n):\n");
        printf("\n> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline character

        toLowerCase(input);

        if (strcmp(input, "y") == 0) {
            // Reset progress if confirmed
            saveProgress(0);
            printf("\n[↻] Resetting progress...\n");
            sleep(1);
            printf("\n[✔] All progress has been reset successfully!\n");
            break;
        } 
        else if (strcmp(input, "n") == 0) {
            printf("\n[✔] Action canceled. Your progress is safe.\n");
            sleep(1);
            return;
        } 
        else {
            printf("\n[!] Invalid input. Please enter 'y' or 'n'.\n");
            sleep(1);
        }
    }

    sleep(2);
    printf("\n--8<------------------------------------\n");
    printf("\n[>] Press Enter to return to the menu...");
    fgets(input, sizeof(input), stdin);  // Wait for Enter key before returning
}

void toLowerCase(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

void eraseprogress() {
    int maxExercises = countExercises();
    int i = 0;
    clear_screen();
    
    printHeader("   [!]  WARNING  [!]");
    printf("  This will PERMANENTLY erase all your work!\n");
    printf("  [✘] This action CANNOT be undone.\n");
    printf("====================================\n");

    if (maxExercises > 0) {
        printf("\nThe following files will be erased:\n");
        while (i < maxExercises) {
            printf("  - ex%02d.c\n", i);
            i++;
        }
    } else {
        printf("No exercise files detected.\n\n");
    }
    printf("\n--8<------------------------------------\n");

    printf("\n[!!] Type 'ERASE' or 'OK' to confirm\n");
    printf("     or press Enter to CANCEL: \n");

    char confirm[10];
    printf("\n> ");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = 0; // Remove newline
    toLowerCase(confirm); 

    if (strcmp(confirm, "erase") != 0 && strcmp(confirm, "ok") != 0) {
        printHeaderx("[✘] Action Canceled. No files were deleted.");
        sleep(1);
        printf("\n[>] Press Enter to return to the menu...");
        getchar();
        return;
    }

    printHeader("[↻] Resetting all exercises...");
    sleep(2);
    system("make -s again");  // Calls Makefile's reset rule
    sleep(2);

    printHeaderx("[✔] All exercises have been reset.");
    printf("\n--8<------------------------------------\n");
    printf("\n[>] Press Enter to return to the menu...\n");
    getchar();
    return;
}

void checkAllExercises() {
    clear_screen();
    printf("******************************\n");
    printf("\n[✔] Checking all exercises...\n");
    sleep(2);

    int totalExercises = countExercises();
    char results[totalExercises][10];  // Store results

    for (int i = 0; i < totalExercises; i++) {
        printf("\n******************************\n");
        printf("\nChecking Exercise %02d...\n", i);

        if (compileAndCheckSolution(i)) {
            strcpy(results[i], "SUCCESS");
        } else {
            printf("[✘] Output does not match reference. Try again!\n");
            printf("ex%02d FAIL\n\n", i);
            strcpy(results[i], "FAIL");
        }
        sleep(2);
    }

    printf("\n******************************\n");
    printf("\n======== RESULTS ========\n");
    for (int i = 0; i < totalExercises; i++) {
        printf("Exercise %02d: %s\n", i, results[i]);
    }
    printf("=========================\n");

    printf("\n[✔] Auto-check completed!\n");
    printf("\n--8<-----------------------------------------------------\n");
    printf("\n[>] Press Enter to continue...");
    getchar();
}

void openExerciseFile(int exerciseNumber, int autoOpenSubjects) {
    if (!autoOpenSubjects) return;  // Exit if auto-open is disabled

    char subjectPath[100], targetFile[100], command[256]; // Increased command buffer size

    // Define source and target file paths
    snprintf(subjectPath, sizeof(subjectPath), "ex%02d/subject.txt", exerciseNumber);
    snprintf(targetFile, sizeof(targetFile), "ex%02d/ex%02d_subject.txt", exerciseNumber, exerciseNumber);
    struct stat buffer;
    if (stat(targetFile, &buffer) != 0) {  // If file does not exist, copy it
    #ifdef _WIN32
        snprintf(command, sizeof(command), "copy \"%s\" \"%s\" > nul 2>&1", subjectPath, targetFile);
    #else
    snprintf(command, sizeof(command), "cp ex%02d/subject.txt ex%02d/ex%02d_subject.txt", exerciseNumber, exerciseNumber, exerciseNumber);
    system(command);
    #endif
        system(command);  // Execute the copy command
    }

    // Open the copied file
    #ifdef _WIN32
    snprintf(command, sizeof(command), "start \"\" \"%s\"", targetFile);
    #elif __APPLE__
    snprintf(command, sizeof(command), "open \"%s\"", targetFile);
    #else
    snprintf(command, sizeof(command), "xdg-open \"%s\" >/dev/null 2>&1", targetFile);
    #endif

    system(command);  // Open the file
}

int compileAndGenerateReferenceOutput(int exerciseNumber) {
    char sourceFile[50], outputFile[50], refOutputFile[50], command[256];

    snprintf(sourceFile, sizeof(sourceFile), "ex%02d/solutions/solution_ex%02d.c", exerciseNumber, exerciseNumber);
    snprintf(outputFile, sizeof(outputFile), "tmp/ex%02d.out", exerciseNumber);
    snprintf(refOutputFile, sizeof(refOutputFile), "ex%02d/expected_output.txt", exerciseNumber);

    // Keep prompting until the correct file exists
    while (access(sourceFile, F_OK) == -1) {
		clear_screen();
        printexHeader("Coding Task ex", exerciseNumber);
        printf("\n*****\n[!] Error: Missing reference solution file: %s\n", sourceFile);
        printf("[!] Please provide the correct solution file before proceeding.\n*****\n\n");
        printf("\nPress Enter to check again...");
        getchar();
		clear_screen();
    }

    if (access(refOutputFile, F_OK) == 0) {
        return 1;  // Already exists, no need to regenerate
    }

    // Compile the reference solution
    snprintf(command, sizeof(command), "gcc %s -o %s", sourceFile, outputFile);
    if (system(command) != 0) {
        printf("\n[!] Error: Compilation failed for %s\n", sourceFile);
        return 0;
    }

    // Run the compiled program and save output
    snprintf(command, sizeof(command), "%s > %s", outputFile, refOutputFile);
    system(command);

    // Remove the compiled output file
    snprintf(command, sizeof(command), "rm -f %s", outputFile);
    system(command);

    return 1;
}
