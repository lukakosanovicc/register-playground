#include <stdio.h>
#include <stdbool.h>
#include "utilities.h"

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

void read() {
    printf("----------------------------------------------------------------------");
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

void read_as_numbers()
{
    int i, number = 0;
    if (reg1[0] == 1) {
        for (i = 6; i >= 0; --i) {
            number += reg1[i] == true ? (1 << i) : 0; // (1 << i) [works only on positive numbers] == 2^i faster than pow(2, i) which returns double
        }
        number -= (1 << 7); // -= 128 to bring it into negative
    } else {
        for (i = 6; i >= 0; --i) {
            number += reg1[i] == true ? (1 << i) : 0;
        }
    }
    printf("\nThe value in reg1 is %d", number);
    number = 0;
    if (reg2[0] == 1) {
        for (i = 6; i >= 0; --i) {
            number += reg2[i] == true ? (1 << i) : 0;
        }
        number -= (1 << 7);
    } else {
        for (i = 6; i >= 0; --i) {
            number += reg2[i] == true ? (1 << i) : 0;
        }
    }
    printf("\nThe value in reg2 is %d", number);
    number = 0;
    if (reg3[0] == 1) {
        for (i = 6; i >= 0; --i) {
            number += reg3[i] == true ? (1 << i) : 0;
        }
        number -= (1 << 7);
    } else {
        for (i = 6; i >= 0; --i) {
            number += reg3[i] == true ? (1 << i) : 0;
        }
    }
    printf("\nThe value in reg3 is %d", number);
    number = 0;
    if (reg4[0] == 1) {
        for (i = 6; i >= 0; --i) {
            number += reg4[i] == true ? (1 << i) : 0;
        }
        number -= (1 << 7);
    } else {
        for (i = 6; i >= 0; --i) {
            number += reg4[i] == true ? (1 << i) : 0;
        }
    }
    printf("\nThe value in reg4 is %d", number);
    number = 0;
    if (reg5[0] == 1) {
        for (i = 6; i >= 0; --i) {
            number += reg5[i] == true ? (1 << i) : 0;
        }
        number -= (1 << 7);
    } else {
        for (i = 6; i >= 0; --i) {
            number += reg5[i] == true ? (1 << i) : 0;
        }
    }
    printf("\nThe value in reg5 is %d\n\n", number);
}

bool write(unsigned int *pok, bool *WP) {
    if (*WP) {
        printf("\nWrite protection is enabled for this register. Modifications are not allowed.\n");
        return true;
    }

    unsigned int temp[8] = {0};

    printf("Enter 8 binary digits (0 or 1) separated by spaces:\n");
    if (scanf("%u %u %u %u %u %u %u %u",
              &temp[0], &temp[1], &temp[2], &temp[3],
              &temp[4], &temp[5], &temp[6], &temp[7]) != 8) {
        printf("\nInvalid input.\n");
        clear_input_buffer();
        return true;
    }

    for (int i = 0; i < 8; ++i) {
        if (temp[i] != 0 && temp[i] != 1) {
            printf("\nInvalid input: Only 0 or 1 allowed.\n");
            return true;
        }
        pok[i] = temp[i];
    }

    return false;
}

void toggle_wp(unsigned int reg_index) {
    bool* wp = NULL;

    switch (reg_index) {
        case 0: wp = &WP1; break;
        case 1: wp = &WP2; break;
        case 2: wp = &WP3; break;
        case 3: wp = &WP4; break;
        case 4: wp = &WP5; break;
        default: return;
    }

    *wp = !(*wp);
    printf("\nWrite protection for register %u is now %s.\n", reg_index+1, *wp ? "enabled" : "disabled");
}

bool save_config()
{
    FILE *f = fopen("config.dat", "wb");
    if (!f)
    {
        printf("\nError while saving the config.\n");
        return true;
    }

    fwrite(reg1, sizeof(reg1), 1, f);
    fwrite(reg2, sizeof(reg2), 1, f);
    fwrite(reg3, sizeof(reg3), 1, f);
    fwrite(reg4, sizeof(reg4), 1, f);
    fwrite(reg5, sizeof(reg5), 1, f);

    fwrite(&WP1, sizeof(WP1), 1, f);
    fwrite(&WP2, sizeof(WP2), 1, f);
    fwrite(&WP3, sizeof(WP3), 1, f);
    fwrite(&WP4, sizeof(WP4), 1, f);
    fwrite(&WP5, sizeof(WP5), 1, f);

    fclose(f);
    return false;
}

bool load_config()
{
    FILE *f = fopen("config.dat", "rb");
    if (!f) return true;

    if (fread(&reg1, sizeof(reg1), 1, f) != 1) return true;
    if (fread(&reg2, sizeof(reg2), 1, f) != 1) return true;
    if (fread(&reg3, sizeof(reg3), 1, f) != 1) return true;
    if (fread(&reg4, sizeof(reg4), 1, f) != 1) return true;
    if (fread(&reg5, sizeof(reg5), 1, f) != 1) return true;

    if (fread(&WP1, sizeof(WP1), 1, f) != 1) return true;
    if (fread(&WP2, sizeof(WP2), 1, f) != 1) return true;
    if (fread(&WP3, sizeof(WP3), 1, f) != 1) return true;
    if (fread(&WP4, sizeof(WP4), 1, f) != 1) return true;
    if (fread(&WP5, sizeof(WP5), 1, f) != 1) return true;

    fclose(f);
    return false;
}

int main() {
    set_console_title("Register Playground");

    unsigned int *ar[5] = {reg1, reg2, reg3, reg4, reg5};
    bool *wp[5] = {&WP1, &WP2, &WP3, &WP4, &WP5};

    load_config();

    unsigned int choice;
    do {
        read();

        printf("Welcome to Register Playground! Options:\n");
        printf("0. Clear console\n1. Modify a register\n2. Toggle write protect for a register\n3. Read as numbers\n4. Clear config file\n5. Save to a config file and exit\n\n>>> ");
        scanf("%u", &choice);

        switch(choice) {
            case 0:
            {
                clear_screen();
                break;
            }
            case 1:
            {
                unsigned int reg_choice;
                printf("-> Which register (1-5) do you want to modify (0 to go back)? ");
                scanf("%u", &reg_choice);

                if (reg_choice == 0) break;

                if (reg_choice >= 1 && reg_choice <= 5) {
                    if (!write(ar[reg_choice-1], wp[reg_choice-1])) {
                        printf("\nRegister updated successfully.\n");
                    } else {
                        printf("\nFailed to update register.\n");
                    }
                } else {
                    printf("\nInvalid register number.\n");
                }
                break;
            }
            case 2:
            {
                unsigned int reg_to_toggle;
                printf("-> Enter the register number (1-5) to toggle write protection (0 to go back): ");
                scanf("%u", &reg_to_toggle);

                if (reg_to_toggle == 0) break;

                if (reg_to_toggle >= 1 && reg_to_toggle <= 5) {
                    toggle_wp(reg_to_toggle-1);
                } else {
                    printf("\nInvalid register number.\n");
                }
                break;
            }
            case 3:
            {
                read_as_numbers();
                break;
            }
            case 4:
            {
                FILE *f = fopen("config.dat", "wb");
                if (!f)
                {
                    printf("\nError while clearing the config.\n");
                    break;
                }
                fclose(f);
                return;
            }
            case 5:
            {
                if (!save_config()) {
                    printf("\nSuccessfully saved. Exiting...\n");
                    return 0;
                } else {
                    printf("\nFailed to save config.\n");
                    break;
                }
            }
        }

    } while (choice >= 0 && choice <= 5);

    return 0;
}
