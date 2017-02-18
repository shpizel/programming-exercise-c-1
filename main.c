#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *normalize(const char *path);

int main() {
    float startTime = (float) clock() / CLOCKS_PER_SEC;

    /*printf("'%s' vs '%s'\n", normalize("../bar"), "/bar");
    printf("'%s' vs '%s'\n", normalize("/foo/bar"), "/foo/bar");
    printf("'%s' vs '%s'\n", normalize("/foo/bar/../baz"), "/foo/baz");
    printf("'%s' vs '%s'\n", normalize("/foo/bar/./baz/"), "/foo/bar/baz/");
    printf("'%s' vs '%s'\n", normalize("/foo/../../baz"), "/baz");*/
//    printf("'%s' vs '%s'\n", normalize("/foo/../bar/../../baz"), "/baz");

    /* Do work */
    for (int i=0;i<1000;i++) {

        normalize("/foo/../../");
    }

    float endTime = (float)clock()/CLOCKS_PER_SEC;

    float timeElapsed = endTime - startTime;
    printf("%f\n", timeElapsed);

    return 0;
}

char *getchunk(const char *path, int number) {
    char *pointer = (char *) path;
    int slashes = 1, size = 0;

    if (path[0] == '/') {
        pointer++;
    }

    while (*pointer != '\0' && slashes != number) {
        if (*pointer == '/') {
            slashes++;
        }
        pointer++;
    }

    while (*pointer != '\0' && *pointer != '/') {
        size++;
        pointer++;
    }

    char *ret = (char *) calloc((size_t) size, sizeof(char));
    pointer -= size;

    for (int i = 0; i < size; i++) {
        ret[i] = *pointer;
        pointer++;
    }

    return ret;
}

char *normalize(const char *path) {
    //printf("%s:\n", path);

    int size = 1;
    char *pointer = (char *) path;
    while (*pointer != '\0') {
        if (*pointer == '/') {
            size++;
        }
        pointer++;
    }

    /** не учитывать первый слеш если он есть */
    if (path[0] == '/') {
        size--;
    }

    char **array = (char **) malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        array[i] = getchunk(path, i + 1);
        //printf("%d\t%s\n", i, array[i]);
    }

    for (int i = 0; i < size; i++) {
        if (array[i][0] == '.' && array[i][1] == '.') {
            if (i == 0) {
                // .. вначале
                free(array[i]);
                array[i] = 0;
            } else {
                // .. в середине - нужно двигаться вверх по списку до верха либо первого не NULL */
                for (int j = i - 1; j >= 0; j--) {
                    if (array[j] != NULL) {
                        free(array[j]);
                        array[j] = 0;
                        break;
                    }
                }
                free(array[i]);
                array[i] = NULL;
            }
        } else if (array[i][0] == '.') {
            free(array[i]);
            array[i] = 0;
        }
    }

//    for (int i=0;i<size;i++) {
////        array[i] = getchunk(path, i+1);
//        printf("%d\t%s\n", i, array[i]);
//    }

    int len = 1; // минимум / сначала будет
    char *helper;
    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            helper = array[i];
            while (*helper != '\0') {
                len++;
                helper++;
            }
            len++;
        }
    }

    char *ret = (char *) calloc((size_t) len, sizeof(char));
    ret[0] = '/';
    int index = 1;
    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            helper = array[i];
            while (*helper != '\0') {
                ret[index] = *helper;
                index++;
                helper++;
            }

            if (i < size - 1) {
                ret[index] = '/';
                index++;
            }

            free(array[i]);
        }
    }


    return ret;
}
