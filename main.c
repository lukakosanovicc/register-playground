#include <stdio.h>
#include <stdbool.h>

/*
TODO:
- CLEANUP!!!
- also saving WP status and clearing it when clearing Config.dat
- convert registers to numbers (1 MSB means negative)
*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void ClearScreen()
{
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count)) return;
    if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;

    SetConsoleCursorPosition(hStdOut, homeCoords);
}
#else
#include <unistd.h>
#include <term.h>

void ClearScreen()
{
    if (!cur_term)
    {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if (result <= 0) return;
    }

    putp(tigetstr("clear"));
}
#endif

/*-------------------------------------------------*/

unsigned int reg1[8] = {0};
bool WP1 = false;

unsigned int reg2[8] = {0};
bool WP2 = false;

unsigned int reg3[8] = {0};
bool WP3 = false;

unsigned int reg4[8] = {0};
bool WP4 = false;

unsigned int reg5[8] = {0};
bool WP5 = false;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void R() {
    printf("\nunsigned int *ar[5] = {reg1, reg2, reg3, reg4, reg5};");
    printf("\n\n\t");
    for (unsigned int i = 0; i < 8; ++i) {
        printf("%d", reg1[i]);
    }
    printf("\tWP = %d", WP1);
    printf("\n\t");
    for (unsigned int i = 0; i < 8; ++i) {
        printf("%d", reg2[i]);
    }
    printf("\tWP = %d", WP2);
    printf("\n\t");
    for (unsigned int i = 0; i < 8; ++i) {
        printf("%d", reg3[i]);
    }
    printf("\tWP = %d", WP3);
    printf("\n\t");
    for (unsigned int i = 0; i < 8; ++i) {
        printf("%d", reg4[i]);
    }
    printf("\tWP = %d", WP4);
    printf("\n\t");
    for (unsigned int i = 0; i < 8; ++i) {
        printf("%d", reg5[i]);
    }
    printf("\tWP = %d", WP5);
    printf("\n\n");
}

bool W(unsigned int *pok, bool *WP) {
    if (*WP) {
        printf("Write protection is enabled for this register. Modifications are not allowed.\n");
        return true;
    }

    unsigned int temp[8] = {0};

    printf("Enter 8 binary digits (0 or 1) separated by spaces:\n");
    if (scanf("%u %u %u %u %u %u %u %u",
              &temp[0], &temp[1], &temp[2], &temp[3],
              &temp[4], &temp[5], &temp[6], &temp[7]) != 8) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return true;
    }

    for (int i = 0; i < 8; ++i) {
        if (temp[i] != 0 && temp[i] != 1) {
            printf("Invalid input: Only 0 or 1 allowed.\n");
            return true;
        }
        pok[i] = temp[i];
    }

    return false;
}

void toggleWP(unsigned int regIndex) {
    bool* wp = NULL;

    switch (regIndex) {
        case 0: wp = &WP1; break;
        case 1: wp = &WP2; break;
        case 2: wp = &WP3; break;
        case 3: wp = &WP4; break;
        case 4: wp = &WP5; break;
        default: return;
    }

    *wp = !(*wp);
    printf("\nWrite protection for register %u is now %s.\n", regIndex+1, *wp ? "enabled" : "disabled");
}

bool saveConfig()
{
    FILE *f = fopen("Config.dat", "wb");
    if (!f)
    {
        printf("Error while saving the config.\n");
        return true;
    }

    fwrite(reg1, sizeof(reg1), 1, f);
    fwrite(reg2, sizeof(reg2), 1, f);
    fwrite(reg3, sizeof(reg3), 1, f);
    fwrite(reg4, sizeof(reg4), 1, f);
    fwrite(reg5, sizeof(reg5), 1, f);

    fclose(f);
    return false;
}

bool loadConfig()
{
    FILE *f = fopen("Config.dat", "rb");
    if (!f) return true;

    if (fread(&reg1, sizeof(reg1), 1, f) != 1) return true;
    if (fread(&reg2, sizeof(reg2), 1, f) != 1) return true;
    if (fread(&reg3, sizeof(reg3), 1, f) != 1) return true;
    if (fread(&reg4, sizeof(reg4), 1, f) != 1) return true;
    if (fread(&reg5, sizeof(reg5), 1, f) != 1) return true;

    fclose(f);
    return false;
}

int main() {
    unsigned int *ar[5] = {reg1, reg2, reg3, reg4, reg5};
    bool *wp[5] = {&WP1, &WP2, &WP3, &WP4, &WP5};

    loadConfig();

    unsigned int choice;
    do {
        R();

        printf("Welcome to Memory Playground! Options:\n");
        printf("0. Clear console\n1. Modify a register\n2. Toggle write protect for a register\n3. Clear config file\n4. Save to a config file and exit\n\n>>> ");
        scanf("%u", &choice);

        switch(choice) {
            case 0:
            {
                ClearScreen();
                break;
            }
            case 1:
            {
                unsigned int regChoice;
                printf("-> Which register (1-5) do you want to modify (0 to go back)? ");
                scanf("%u", &regChoice);

                if (regChoice == 0) break;

                if (regChoice >= 1 && regChoice <= 5) {
                    if (!W(ar[regChoice-1], wp[regChoice-1])) {
                        printf("Register updated successfully.\n");
                    } else {
                        printf("Failed to update register.\n");
                    }
                } else {
                    printf("Invalid register number.\n");
                }
                break;
            }
            case 2:
            {
                unsigned int regToToggle;
                printf("-> Enter the register number (1-5) to toggle write protection (0 to go back): ");
                scanf("%u", &regToToggle);

                if (regToToggle == 0) break;

                if (regToToggle >= 1 && regToToggle <= 5) {
                    toggleWP(regToToggle-1);
                } else {
                    printf("Invalid register number.\n");
                }
                break;
            }
            case 3:
            {
                FILE *f = fopen("Config.dat", "wb");
                    if (!f)
                {
                    printf("Error while clearing the config.\n");
                    break;
                }
                fclose(f);
                break;
            }
            case 4:
            {
                if (!saveConfig()) {
                    printf("Successfully saved. Exiting...\n");
                    return 0;
                } else {
                    printf("Failed to save config.\n");
                    break;
                }
            }
        }

    } while (choice >= 0 && choice <= 4);

    return 0;
}
