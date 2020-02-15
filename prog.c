#include <windows.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SOFTVERS 1.01
#define INITFILEPATH "initfile.txt"
#define LISTFILEPATH "listfile.txt"
#define SCANINTERVAL 1
#define RANDOMDATALENGTH 1000
#define RANDOMNAMELENGTH 6
#define VERBOSE 4               /*0 = Nothing displayed except errors, 1 = Only few things like the state of the program, 2 = Message at every scan interval, 3 = A lot of messages, 4 = Every messages*/



FILE* open_file(const char* filepath, const char* mode) {
    /*Opens a file, test if the file exist and the pointer is correctly created then return it.*/

    FILE* file = (FILE*) fopen(filepath, mode);

    if (file == NULL) {
        printf("Error: this file doesn't seem to exist or cannot be created: %s\n", filepath);
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return file;

}


int test_if_file_exist(const char* filepath) {
    /*Return 1 if the file exists, 0 if not.*/

    FILE* fp = (FILE*) fopen(filepath, "r");
    if (fp == NULL){
        return 0;
    }
    fclose(fp);
    return 1;

}


int hash_file(const char* filepath) {
    /*Hash function using some DIY methods, please don't hate me...*/

    if (test_if_file_exist(filepath) == 0) {
        printf("Error: this file doesn't seem to exist: %s\n", filepath);
        perror("");
        exit(EXIT_FAILURE);
    }

    FILE* file = (FILE*) fopen(filepath, "rb");

    int e = 0;
    int result = 0;

    for (unsigned int i=0; i<RANDOMDATALENGTH; i++) {
        fread(&e, sizeof(int), 1, file);
        result += (e + i) % 9999999999;
    }

    fclose(file);

    return result;

}


int main () {

    printf("\n");
    if (VERBOSE > 0) {
        printf("  _____                              _______                  \n");
        printf(" |  __ \\                            |__   __|                 \n");
        printf(" | |__) |__ _ _ __  ___  ___  _ __ ___ | |_ __ __ _ _ __  ___ \n");
        printf(" |  _  // _` | '_ \\/ __|/ _ \\| '_ ` _ \\| | '__/ _` | '_ \\/ __|\n");
        printf(" | | \\ \\ (_| | | | \\__ \\ (_) | | | | | | | | | (_| | |_) \\__ \\\n");
        printf(" |_|  \\_\\__,_|_| |_|___/\\___/|_| |_| |_|_|_|  \\__,_| .__/|___/\n");
        printf("                                                   | |        \n");
        printf("                                                   |_|    %g\n\n\n", SOFTVERS);
    }

    /*Check if it's the program first launch.*/
    if (test_if_file_exist(LISTFILEPATH) == 0) {

        if (VERBOSE > 0) {printf("Welcome! This is the first time this program is been launched.\n\n");}
        if (VERBOSE > 0) {printf("RansomTraps is a free anti-ransomware program. It is variant resistant, it doesn't rely on the ransomware's signature to detect an attack. ");}
        if (VERBOSE > 0) {printf("Here's how this works: The program generates random 'dummy files' in locations specified by the 'initfile' inside the program folder. ");}
        if (VERBOSE > 0) {printf("A dummy file is a file that is only used by that program; it contains a random string. The program check on that file from time to time to make sure nothing has been encrypted. ");}
        if (VERBOSE > 0) {printf("If a dummy file has been altered or deleted, the program will alert the user and quickly shutdown the computer. It is highly recommended to NOT restart the computer after this, but instead to properly copy your data to another disk if you don't already have a backup system.\n\n");}
        if (VERBOSE > 2) {printf("Here is the current configuration:\nThe initfile is located here: %s\nThe listfile will be created here: %s\nRandom string of %i characters will be place inside files.\nThe program will scan if files have been altered every %is.\n\n", INITFILEPATH, LISTFILEPATH, RANDOMDATALENGTH,SCANINTERVAL);}
        if (VERBOSE > 0) {printf("Furthermore, here is the list of folders where files will be placed. They will have random names and will be of the DOCX, JPG, MP3 and ODT.\n");}

        if (test_if_file_exist(INITFILEPATH) == 0) {
            printf("Error: initfile doesn't exist, the program cannot start without one.");
            perror("Error");
            exit(EXIT_FAILURE);
        }

        FILE* initfile = open_file(INITFILEPATH, "r");

        int maximum_line_length = 256;
        char* line = (char*) malloc(sizeof(char) * maximum_line_length);
        char* directory_path = (char*) malloc(sizeof(char) * maximum_line_length);
        char* randomdata = (char*) malloc(sizeof(char) * (RANDOMDATALENGTH + 1));
        char* randomname = (char*) malloc(sizeof(char) * (RANDOMNAMELENGTH + 1));

        if (line == NULL) {
            perror("Error while allocating memory for line buffer");
            exit(EXIT_FAILURE);
        }

        if (directory_path == NULL) {
            perror("Error while allocating memory for directory_path");
            exit(EXIT_FAILURE);
        }

        if (randomdata == NULL) {
            perror("Error while allocating memory for random data");
            exit(EXIT_FAILURE);
        }

        if (randomname == NULL) {
            perror("Error while allocating memory for random name");
            exit(EXIT_FAILURE);
        }

        int r;

        while ((r = fscanf(initfile, "%s\n", line)) != EOF) {
            printf("%s\n", line);
        }

        fseek(initfile, 0, SEEK_SET);
        printf("\nPLEASE MAKE SURE TO NOT RENAME, MOVE OR MODIFY THOSE FILES IN ANYWAY OR THE PROGRAM WILL SEE IT HAS A POTENTIAL THREAT.\n\n");
        printf("\nDo you agree with the above? (Yes, NO)\n\n");

        char response[4];
        if ( scanf("%3s", response) == 0 || strcmp(response,"n") == 0 || strcmp(response,"N") == 0 || strcmp(response,"no") == 0 || strcmp(response,"No") == 0 || strcmp(response,"NO") == 0) {
            printf("\nVery well, the program will now close itself. If you wish to change the directories where dummy files will be install, please change the initfile and start the program again.\n\n");
            exit(EXIT_SUCCESS);
        }


        /*END OF THE INTRODUCTION LET'S INITIALISE EVERYTHING.*/



        FILE* newfile;
        FILE* listfile;
        srand((unsigned) time(NULL));

        /*For as long as there are lines inside the initfile, read the next line.*/
        while ((r = fscanf(initfile, "%s\n", line)) != EOF) {

            /*There are 4 different types of files we want to generate.*/
            for (unsigned j = 0; j<4; j++) {

                strcpy(directory_path, line);
                /*Generate a random filename of length RANDOMNAMELENGTH*/
                for (unsigned i = 0; i<RANDOMNAMELENGTH; i++) {
                    randomname[i] = 100 + (rand() % 20);
                }

                randomname[RANDOMNAMELENGTH] = atoi("");

                strcat(directory_path, "\\");
                strcat(directory_path, randomname);
                if (j==0) {
                    strcat(directory_path, ".docx");
                } else if (j==1) {
                    strcat(directory_path, ".txt");
                } else if (j==2) {
                    strcat(directory_path, ".jpg");
                } else {
                    strcat(directory_path, ".mp3");
                }



                /*Create a file at the path given by the current line*/
                newfile = open_file(directory_path, "w");
                if (VERBOSE > 3) {printf("%s has been created successfully.\n", directory_path);}

                /*Create random string.*/
                for (unsigned i = 0; i<RANDOMDATALENGTH; i++) {
                    randomdata[i] = 32 + (rand() % 90);
                }

                randomdata[RANDOMDATALENGTH] = atoi(""); /*Make sure that the last character is a \0 "end of the string" character*/

                /*Write it inside the file.*/
                fprintf(newfile, "%s", randomdata);
                if (VERBOSE > 3) {printf("This is the random data now written inside: %s\n", randomdata);}
                fclose(newfile);
                if (VERBOSE > 3) {printf("newfile (%s) has been closed.\n\n", directory_path);}

                /*Write the file path and hash inside the listfile.*/
                if (VERBOSE > 3) {printf("Trying to open listfile path: %s\n", LISTFILEPATH);}
                listfile = open_file(LISTFILEPATH, "a");
                if (VERBOSE > 3) {printf("listfile has been opened successfully.\n\n", LISTFILEPATH);}
                fprintf(listfile, "%s %i\n", directory_path, hash_file(directory_path));
                fclose(listfile);

            }

        }

        if (VERBOSE > 3) {printf("\nEvery file has successfully been created with data inside. Now let's clean the memory.\n");}
        free(line);
        if (VERBOSE > 3) {printf("line has been freed.\n");}
        free(randomdata);
        if (VERBOSE > 3) {printf("randomdata has been freed.\n");}
        free(randomname);
        if (VERBOSE > 3) {printf("randomname has been freed.\n");}
        fclose(initfile);
        if (VERBOSE > 3) {printf("initfile has been closed.\n\n");}

    }

    /*The program has successfully places files, now we can check on them*/
    if (VERBOSE > 0) {printf("Everything seems ready now, let's start the scan.\n\n");}
    if (VERBOSE > 2) {printf("This program pid is %d\n\n", getpid());}

    FILE* listfile = open_file(LISTFILEPATH, "r");

    int maximum_line_length = 256;
    char* line = (char*) malloc(sizeof(char) * maximum_line_length);

    if (line == NULL) {
        perror("Error while allocating memory for line buffer");
        exit(EXIT_FAILURE);
    }

    int r;
    int expected_hash;
    int count = 0;

    while (1) {

        /*For as long as there are lines inside the listfile, read the next line.*/
        while ((r = fscanf(listfile, "%s %i\n", line, &expected_hash)) != EOF) {

            if (test_if_file_exist(line) == 0) {
                printf("It seems that this filename has been altered or deleted: %s\n", line);
                return EXIT_SUCCESS;
            }

            if (expected_hash != hash_file(line)) {
                printf("It seems that this file has been altered: %s\n", line);
                return EXIT_SUCCESS;
            }

        }

        /*Go back to the beginning of the file*/
        fseek(listfile, 0, SEEK_SET);

        if (VERBOSE > 1) {printf("[%i] Everything normal, waiting %is before next scan.\n", count, SCANINTERVAL);}
        count++;

        Sleep(SCANINTERVAL*1000);

    }

    return EXIT_SUCCESS;
    fclose(listfile);

}
