#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls(int n) {
    int c = 0;
    char *old, *new;
    old = calloc(2, sizeof(char));
    old[0] = '1';
    
    while (c++<n) {
        printf("%s\n", old);
        int l = strlen(old);
        new = calloc((l<<1)+1, sizeof(char));
        int i, j, k, m;
        for (i=old[0], j=1, k=1, m=0; k<l; k++) {
            if (i == old[k]) {
                j++;
            } else {
                new[m++] = '0'+j;
                new[m++] = i;
                i = old[k];
                j = 1;
            }
        }
        new[m++] = '0'+j;
        new[m++] = i;
        
        free(old);
        old = new;
    }
    free(old);
}

int main(int argc, char **argv) {
    
    ls(atoi(argv[1]));
    return 0;
}