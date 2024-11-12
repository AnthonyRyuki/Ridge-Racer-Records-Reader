#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

char result[20];

/*The records are expressed in milliseconds and are stored as integers, with Little-endian as their byte order. So here's how they work.
For example, we take the default 1st place Time Trial Lap Record. Its value is 7DAC03. The first thing we need to do is convert it from Little-endian to Big-endian.
In other words, we separate its bytes in this manner: 7D-AC-03, then invert its byte order, and the resulting value will be 03-AC-7D.
Finally, we convert the value from hexadecimal to decimal and we will get 240765, which is the record we're working with expressed in milliseconds.
If expressed in the minutes'seconds"milliseconds format that the game uses, the value is 4'00"765.*/
char* timeConverter(uint8_t digit1, uint8_t digit2, uint8_t digit3)
{
    int milliseconds = digit1 + digit2*0x100 + digit3*0x10000;
    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;

    while (seconds >= 60)
        seconds -= 60;

    while (milliseconds >= 1000)
        milliseconds -= 1000;

    sprintf(result, "%d'%02d\"%03d", minutes, seconds, milliseconds);

    return result;
}

char* carConverter(uint8_t value)
{
    switch (value)
    {
    case 0:
        sprintf(result, "F/A Racing");
        break;
    case 1:
        sprintf(result, "RT Ryukyu");
        break;
    case 2:
        sprintf(result, "RT Yellow Solvalou");
        break;
    case 3:
        sprintf(result, "RT Blue Solvalou");
        break;
    case 4:
        sprintf(result, "RT Pink Mappy");
        break;
    case 5:
        sprintf(result, "RT Blue Mappy");
        break;
    case 6:
        sprintf(result, "Galaga RT Plid's");
        break;
    case 7:
        sprintf(result, "Galaga RT Carrot");
        break;
    case 8:
        sprintf(result, "RT Bosconian");
        break;
    case 9:
        sprintf(result, "RT Nebulasray");
        break;
    case 10:
        sprintf(result, "RT Xevious Red");
        break;
    case 11:
        sprintf(result, "RT Xevious Green");
        break;
    case 12:
        sprintf(result, "13th Racing");
        break;
    }

    return result;
}

char* positionMaker(uint32_t value)
{
    switch (value)
    {
        case 0:
            sprintf(result, "1st");
            break;
        case 1:
            sprintf(result, "2nd");
            break;
        case 2:
            sprintf(result, "3rd");
            break;
        default:
            sprintf(result, "%dth", value+1);
            break;
    }

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "To use this application, drag and drop a Ridge Racer save file to it.\n");
        system("pause");
        return 1;
    }

    FILE *fin = fopen(argv[1], "rb");

    if (!fin)
    {
        fprintf(stderr, "Unable to open save file\n");
        system("pause");
        return 1;
    }

    fseek(fin, 0xC, SEEK_SET);

    char check[20];
    uint32_t i, j, counter;
    uint8_t buf[4];
    uint8_t bufcheck[10];

    fread(bufcheck,1,10,fin);
    for (i = 0; i < 10; i++)
        check[i] = bufcheck[i];

    if (strcmp("SLPS-00001", check) != 0 && strcmp("SLPS-91001", check) != 0 && strcmp("SCUS-94300", check) != 0 && strcmp("SLUS-00540", check) != 0 && strcmp("SCES-00001", check) != 0)
    {
        fprintf(stderr, "Not a valid Ridge Racer save file\n");
        system("pause");
        return 1;
    }

    fseek(fin, 0x180, SEEK_SET);

    j = 0;
    counter = 0;

    printf("Time Trial Fastest Laps\n");
    for (i = 0; i < 18; i++)
    {
        fread(buf,1,4,fin);
        switch (j)
        {
            case 0:
                printf("%s: %c%c%c ", positionMaker(counter), buf[0], buf[1], buf[2]);
                break;
            case 1:
                printf("%s ", timeConverter(buf[0], buf[1], buf[2]));
                break;
            case 2:
                printf("%s", carConverter(buf[0]));
                break;
        }
        if (j == 2)
        {
            j = 0;
            counter++;
            printf("\n");
        }
        else
            j++;
    }

    counter = 0;

    printf("\nTotal Time Records\n");
    for (i = 0; i < 16; i++)
    {
        fread(buf,1,4,fin);

        switch (counter)
        {
            case 1:
                printf("Extra ");
                break;
            case 2:
                printf("Mirrored ");
                break;
            case 3:
                printf("Extra Mirrored ");
                break;
        }

        switch (j)
        {
            case 0:
                printf("Beginner: ");
                break;
            case 1:
                printf("Mid-Level: ");
                break;
            case 2:
                printf("High Level: ");
                break;
            case 3:
                printf("Time Trial: ");
                break;
        }
        printf("%s\n", timeConverter(buf[0], buf[1], buf[2]));
        if (j == 3)
        {
            j = 0;
            counter++;
        }
        else
            j++;
    }

    counter = 0;

    printf("\nLap Time Records\n");
    for (i = 0; i < 16; i++)
    {
        fread(buf,1,4,fin);

        switch (counter)
        {
            case 1:
                printf("Extra ");
                break;
            case 2:
                printf("Mirrored ");
                break;
            case 3:
                printf("Extra Mirrored ");
                break;
        }

        switch (j)
        {
            case 0:
                printf("Beginner: ");
                break;
            case 1:
                printf("Mid-Level: ");
                break;
            case 2:
                printf("High Level: ");
                break;
            case 3:
                printf("Time Trial: ");
                break;
        }
        printf("%s\n", timeConverter(buf[0], buf[1], buf[2]));
        if (j == 3)
        {
            j = 0;
            counter++;
        }
        else
            j++;
    }

    fseek(fin, 0x264, SEEK_SET);
    fread(buf,1,4,fin);

    printf("\nBonus Cars Unlocked? ");
    if (buf[0] == 1)
        printf("Yes\n");
    else
    {
        printf("No\n");
    }

        printf("13th Racing Unlocked? ");
    if (buf[2] == 1)
        printf("Yes\n");
    else
    {
        printf("No\n");
    }

    fclose(fin);

    system("pause");

    return 0;
}
