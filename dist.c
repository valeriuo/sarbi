#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef enum {false, true} bool;

int dist(const char *s1, const char *s2) {
    if (!s1 || !s2)
        return -1;
        
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int i, j, d[l1+1][l2+2];
    
    for (j=0; j<l2+1; j++)
        d[0][j] = j;
    for (i=1; i<l1+1; i++) {
        d[i][0] = i;
        for (j=1; j<l2+1; j++) {
            int k = d[i-1][j] > d[i][j-1] ? d[i][j-1] : d[i-1][j];
            k = k > d[i-1][j-1] ? d[i-1][j-1] : k;
            d[i][j] = (s2[j-1] != s1[i-1]) + k;
        }
    }
    
    return d[l1][l2];
}

void print_match(const char *s1, const char *s2) {
    if (!s1 || !s2)
        return;
    
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int i, j, d[l1+1][l2+2];
    
    for (j=0; j<l2+1; j++)
        d[0][j] = j;
    for (i=1; i<l1+1; i++) {
        d[i][0] = i;
        for (j=1; j<l2+1; j++) {
            int k = d[i-1][j] > d[i][j-1] ? d[i][j-1] : d[i-1][j];
            k = k > d[i-1][j-1] ? d[i-1][j-1] : k;
            d[i][j] = (s2[j-1] != s1[i-1]) + k;
        }
    }
       
    char sm1[l1+l2+1], sm2[l1+l2+1];
    memset (sm1, 0, sizeof(sm1));
    memset (sm2, 0, sizeof(sm2));
    
    int m= l1+l2;
    int k, p, q;
    i = l1, j = l2;
    while (i >=0 && j>=0) {
        
        if (!i && !j)
            break;
                
        if (!i || d[i-1][j-1] > d[i][j-1]) 
        {   
            k = d[i][j-1], p = i, q = j-1;           // delete
        } else if (!j || d[i-1][j-1] > d[i-1][j]) {
            k = d[i-1][j], p = i-1, q = j;           // insert
        } else {
            k = d[i-1][j-1], p = i-1, q = j-1;       // match or mismatch
        }
        
        if (i>p && j>q) {
            int cap = (k==d[i][j])*32;
            sm1[m] = s1[i-1] - cap;
            sm2[m] = s2[j-1] - cap;
        } else if (j==q) {
            sm1[m] = s1[i-1];
            sm2[m] = '-';
        } else if (i==p) {
            sm1[m] = '-';
            sm2[m] = s2[j-1];
        }
        
        //printf("i=%d, j=%d, p=%d, q=%d, k=%d, m=%d, %c, %c\n", i, j, p, q, k, m, s1[m], s2[m]);
        
        i = p;
        j = q;
        m--;
    }
    m++;
    
    printf("'%s'\n", sm1+m);
    printf("'%s'\n", sm2+m);
}

bool one_edit_apart(const char *s1, const char *s2) {
    int d = dist(s1, s2);
    if (d==1) return true;
    else      return false;
}



int main() {
/*
    char *s1 = "cat";
    char *s2 = "dog";
    char *s3 = "cats";
    char *s4 = "cut";
    char *s5 = "cast";
    char *s6 = "at";
    char *s7 = "act";
    
    printf("'%s'- '%s' = %s\n", s1, s2, one_edit_apart(s1, s2) == true ? "true" : "false");
    printf("'%s'- '%s' = %s\n", s1, s3, one_edit_apart(s1, s3) == true ? "true" : "false");
    printf("'%s'- '%s' = %s\n", s1, s4, one_edit_apart(s1, s4) == true ? "true" : "false");
    printf("'%s'- '%s' = %s\n", s1, s5, one_edit_apart(s1, s5) == true ? "true" : "false");
    printf("'%s'- '%s' = %s\n", s1, s6, one_edit_apart(s1, s6) == true ? "true" : "false");
    printf("'%s'- '%s' = %s\n", s1, s7, one_edit_apart(s1, s7) == true ? "true" : "false");
*/  
    print_match("exponential", "polynomial");
    print_match("attagacctg", "cctgccggaa");

    return 0;
}