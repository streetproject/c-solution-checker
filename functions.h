#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_NAME_LEN 50
#define BUFFER_SIZE 1024
#define PROGRESS_FILE "progress.txt"

void clear_screen();
int processInput(char *input, const char *username);
void openSettings();
void setUsername(char *username);
void welcomeUser(const char *username);
void about();
void startExercises(const char *username);
void loadExercise(int exerciseNumber);
void checkAllExercises();
void openExerciseFile(int exerciseNumber, int autoOpenSubjects);
int compileAndCheckSolution(int exerciseNumber);
int compileAndGenerateReferenceOutput(int exerciseNumber);
int endsWithNewline(const char *filename);
int countExercises();
void listExercises();
int getLastCompletedExercise();
void saveProgress(int lastCompleted);
void clearScreen();
char getUserChoice(const char *prompt, const char *validChoices);
void display_interface();
void viewprogress();
void resetprogress();
void eraseprogress();
void toLowerCase(char *str);
void commands();


#endif
