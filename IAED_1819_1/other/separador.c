#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "a Reuniao dos Ze:29032019:1000:60:1:Ze:Ze Carlos:Ze Pedro:Ze Joao";
    char temp[63];
    int i, j = 0;


    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != ':') {
            temp[j++] = str[i];
        } else {
            temp[j] = '\0';
            printf("%s\n", temp);
            j = 0;
        }
    }
    temp[j] = '\0';
    printf("%s\n", temp);

    return 0;
}
