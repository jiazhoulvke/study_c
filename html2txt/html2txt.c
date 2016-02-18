#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

enum State { OUT_TAG = 0, IN_TAG, IN_TEXT, OPEN_TAG, CLOSE_TAG };

char* readTag(char* content) {
    char c;
    char pre_c;
    int state = OUT_TAG;
    char tagName[32] = "";
    while ((c = *content++) != '\0') {
        switch (state) {
            case OUT_TAG:
                if (c == '<') {
                    state = OPEN_TAG;
                    /* 跳过注释 */
                    if (*content == '!') {
                        content = strcasestr(content, "-->") + 3;
                    }
                    int i = 0;
                    /* 获取tag名 */
                    while ((c = *content++) != '\0') {
                        if (isspace(c)) {
                            i++;
                            break;
                        }
                        if (c == '>') {
                            state = IN_TAG;
                            i++;
                            break;
                        }
                        tagName[i] = tolower(c);
                        i++;
                    }
                    tagName[i] = '\0';
                    if (strcmp(tagName, "input") == 0 ||
                        strcmp(tagName, "meta") == 0) {
                        while ((c = *content++) != '\0') {
                            if (c == '>') {
                                return content;
                            }
                        }
                    }
                    /* 跳过script */
                    if (strcmp(tagName, "script") == 0) {
                        content = strcasestr(content, "</script>") + 9;
                        return content;
                    }
                    /*printf("Tag:%s\n", tagName);*/
                }
                break;
            case OPEN_TAG:
                if (c == '>') {
                    state = IN_TAG;
                }
                break;
            case IN_TAG:
                if (c == '<') {
                    if (*content == '/') {
                        state = CLOSE_TAG;
                    } else {
                        content = readTag(--content);
                    }
                } else {
                    if (isspace(c)) {
                        c = ' ';
                        if (pre_c != ' ') {
                            printf("%c", c);
                        }
                    } else {
                        printf("%c", c);
                    }
                }
                break;
            case CLOSE_TAG:
                if (c == '>') {
                    return content;
                }
                break;
        }
        pre_c = c;
    }
    return content;
}

int main(int argc, char* argv[]) {
    FILE* fp;
    char* filename = "test.html";
    if (argc > 1) {
        filename = argv[1];
    }
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "无法打开文件\n");
        return EXIT_FAILURE;
    }
    struct stat buf;
    if (stat(filename, &buf) == -1) {
        fprintf(stderr, "无法获取文件状态\n");
        return EXIT_FAILURE;
    }
    char* m = (char*)malloc(sizeof(char) * buf.st_size);
    char* content = m;
    if (fread(content, sizeof(char) * buf.st_size, 1, fp) <= 0) {
        fprintf(stderr, "读取文件失败\n");
        return EXIT_FAILURE;
    }
    fclose(fp);
    /* 跳过head,直接转到body */
    content = strcasestr(content, "<body");
    char* bodyend = strcasestr(content, "</body");
    char c;
    while ((c = *bodyend) != '\0') {
        *bodyend = '\0';
        bodyend++;
    }
    while (*content != '\0') {
        content = readTag(content);
    }
    free(m);
    return EXIT_SUCCESS;
}
