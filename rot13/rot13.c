#include <stdio.h>
#include <string.h>

char s1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
char s2[] = "NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm";

int main(int argc, char* argv[]) {
    char c;
    while ((c = getchar()) != EOF) {
        int i = 0;
        char* src = s1;
        char* dst = s2;
        if (argc > 1 && strcmp(argv[1], "-d")) {
            src = s2;
            dst = s1;
        }
        for (i = 0; i < (int)sizeof(s1); i++, src++) {
            if (c == *src) {
                printf("%c", *(dst + i));
            }
        }
    }
    return 0;
}
