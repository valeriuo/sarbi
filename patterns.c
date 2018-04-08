#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define d 256
#define q 83
#define perf 0

void naive(char [], char []);
void kmp(char [], char []);
void rabin_karp(char [], char []);
void match_fa(char[], char[], const char[]);
void boyer_moore(char[], char[], const char[]);


int* build_lps(char*);
void print_lps(int*, int);
int* build_fa(char[], const char[]);
void print_fa(int*, int, int);
int get_state(char[], int, char);
int* build_bc(char[], const char[]);
int* build_gs(char[]);

int main() {
    char alf1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char alf2[] = "ACGT";
    
    char txt1[] = "THIS IS A TEST TEXT";
    char pat1[] = "TEST";
    
    naive(pat1, txt1);
    kmp(pat1, txt1);
    rabin_karp(pat1, txt1);
    match_fa(pat1, txt1, alf1);
    boyer_moore(pat1, txt1, alf1);
    
    char txt2[] = "AgBAACAADAABAABA";
    char pat2[] = "AABA";
    
    naive(pat2, txt2);
    kmp(pat2, txt2);
    rabin_karp(pat2, txt2);
    match_fa(pat2, txt2, alf1);
    boyer_moore(pat2, txt2, alf1);
    
    char txt3[] = "BRASCOMBREREIMONDOANAAREMEREGINAAREPERE";
    char pat3[] = "ARE";
    
    naive(pat3, txt3);
    kmp(pat3, txt3);
    rabin_karp(pat3, txt3);
    match_fa(pat3, txt3, alf1);
    boyer_moore(pat3, txt3, alf1);
/*     
    char txt4[] = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    char pat4[] = "BBB";
    
    naive(pat4, txt4);
    kmp(pat4, txt4);
    rabin_karp(pat4, txt4);
  */
 
    
 
 
 /*
  * fa testing

    char tpat2[] = "ACACAGA";
    print_fa(build_fa(tpat2, alf2), strlen(tpat2), 4);
  */
 
 /* 
  * lps testing     

    char tpat3[] = "AAAA";
    char tpat4[] = "ABCDE";
    char tpat5[] = "AABAACAABAA";
    char tpat6[] = "AAACAAAAAC";
    char tpat7[] = "AAABAAA";
  
    int *lps = build_lps(tpat3);
    if (lps) {
        printf("For the pattern \"%s\"\n", tpat3);
        print_lps(lps, strlen(tpat3));
        free(lps);
    }
  
    lps = build_lps(tpat4);
    if (lps) {
        printf("For the pattern \"%s\"\n", tpat4);
        print_lps(lps, strlen(tpat4));
    }

    lps = build_lps(tpat5);
    if (lps) {
        printf("For the pattern \"%s\"\n", tpat5);
        print_lps(lps, strlen(tpat5));
    }

    lps = build_lps(tpat6);
    if (lps) {
        printf("For the pattern \"%s\"\n", tpat6);
        print_lps(lps, strlen(tpat6));
        free(lps);
    }
    lps = build_lps(tpat7);
    if (lps) {
        printf("For the pattern \"%s\"\n", tpat7);
        print_lps(lps, strlen(tpat7));
        free(lps);
    }
  */      
  
    int *gs = build_gs("ABBABAB");
      
    free(gs);  
    return 0;
}

void naive(char pat[], char txt[]) {
    int n = strlen(txt);
    int m = strlen(pat);
    int i, j, c=0;
    
    for (i = 0; i < n - m + 1; i++) {
        for (j=0; j < m; j++) {
            c++;
            if (txt[i + j] != pat[j]) {
                break;
            }
        }
        
        if (j==m) {
            printf("[naive]: Pattern found at index %d\n", i);
        }
    }
    
    if (perf)
        printf("[naive]: Number of comparisons=%d, n=%d, m=%d, n*m=%d, rel=%f\n",c,n,m,n*m,(float)c/n/m);
}

void naive_different(char pat[], char txt[]) {
    int n = strlen(txt);
    int m = strlen(pat);
    int i=0, j, c=0;
    
    while (i < n - m + 1) {
        for (j=0; j < m; j++) {
            c++;
            if (txt[i + j] != pat[j]) {
                break;
            }
        }
        
        if (j==m) {
            printf("[naive]: Pattern found at index %d\n", i);
        } else {
            if (j == 0) {
                i++;
            } else {
                i += j;
            }
        }
    }
    
    if (perf)
        printf("[naive_different]: Number of comparisons=%d, n=%d, m=%d, n*m=%d, rel=%f\n",c,n,m,n*m,(float)c/n/m);
}

void naive_string(char pat[], char txt[]) {
    char *found = strstr(txt, pat);
    while (found) {
        printf("[naive_string]: Pattern found at index %d\n", (int)(found - txt));
        found = strstr(found + 1, pat);
    }
}


void kmp(char pat[], char txt[]) {
    int n = strlen(txt);
    int m = strlen(pat);
    int i, j=0, c=0;
    int *lps = build_lps(pat);
    if (!lps) {
        return;
    }
    
    for (i = 0; i < n - m + 1; i++) {
        for (; j < m; j++) {
            c++;
            if (txt[i + j] != pat[j]) {
                if (j>0) {
                    j = lps[j-1];
                }
                break;
            }
        }
        
        if (j==m) {
            printf("[kmp]: Pattern found at index %d\n", i);
            j = lps[j-1];
        }
    }
    free(lps);
    
    if (perf)
        printf("[kmp]: Number of comparisons=%d, n=%d, m=%d, n*m=%d, rel=%f\n",c,n,m,n*m,(float)c/n/m);
}

void rabin_karp(char pat[], char txt[]) {
    int n = strlen(txt);
    int m = strlen(pat);
    int i, j, c=0, ht=0, hp=0, h=1;
    
    for (i = 0; i < m; i++) {
        ht = (d*ht + txt[i])%q;
        hp = (d*hp + pat[i])%q;
        if (i < m-1) {
            h = (d*h)%q;
        }
    }
    
    for (i = 0; i < n - m + 1; i++) {
        c++;
        if (hp == ht) {
            for (j = 0; j < m; j++) {
                c++;
                if (txt[i + j] != pat[j]) {
                    break;
                }
            }
            
            if (j==m) {
                printf("[rabin_karp]: Pattern found at index %d\n", i);
            }
        } 
        
        if (i < n - m) {
            ht = (d*(ht - h*txt[i]) + txt[i+m])%q;
            if (ht < 0) {
                ht += q;
            }
        }
    }
    
    if (perf)
        printf("[rabin_karp]: Number of comparisons=%d, n=%d, m=%d, n*m=%d, rel=%f\n",c,n,m,n*m,(float)c/n/m);
}

void match_fa(char pat[], char txt[], const char alf[]) {
    if (!pat || !txt || !alf) {
        return;
    }
  
    int n = strlen(txt);
    int m = strlen(pat);
    int a = strlen(alf);
    int i, s, c=0;
    int *fa = build_fa(pat, alf);
    if (!fa) {
        return;
    }
    
    for (i = 0, s = 0; i < n; i++, c++) {
        char *j = strchr(alf, txt[i]);
        if (!j) {
            printf("[match_fa]: Character '%c' not found in alphabet '%s'. Stopping!\n", txt[i], alf);
            return;
        }
        s = *(fa + s*a + (int)(j-alf));
        if (s == m) {
            printf("[match_fa]: Pattern found at index %d\n", i-m+1);
        }
    }
    free(fa);
    
    if (perf)
        printf("[match_fa]: Number of comparisons=%d, n=%d, m=%d, n*m=%d, rel=%f\n",c,n,m,n*m,(float)c/n/m);
}

void boyer_moore(char pat[], char txt[], const char alf[]) {
   if (!pat || !txt || !alf) {
        return;
    }
  
    int n = strlen(txt);
    int m = strlen(pat);
    int i, j, c=0;
    int *bc = build_bc(pat, alf);
    if (!bc) {
        return;
    }
/*     
    for (i = 0; i < strlen(alf); i++) {
        printf("pat='%s', alf='%s' - bc[%d]=%d\n", pat, alf, i, bc[i]);
    }
 */    
    i = m-1;
 
    while (i < n) {
        for (j = m-1; j >= 0; j--) {
            c++;
            if (pat[j] != txt[i-m+j+1]) {
                char *k = strchr(alf,txt[i-m+j+1]);
                //printf("txt='%s', pat='%s', i=%d, j=%d, cur=%d, k=%c, bc=%d\n", txt, pat, i, j, (i-m+j+1), k[0], bc[(int)(k-alf)]);
                if (k) {
                    if (bc[(int)(k-alf)] > j) {
                        i += 1;
                    } else {
                        i += j-bc[(int)(k-alf)];
                    }
                } else { //the character found is not in the alphabet
                    printf("[boyer_moore]: Character '%c' not found in alphabet '%s'. Continuing...\n", txt[i-m+j+1], alf);
                    i += j+1;
                }
                break;
            }
        }
        //printf("txt='%s',pat='%s',i=%d, j=%d\n", txt, pat, i, j);
        if (j+1 == 0) {
            printf("[boyer_moore]: Pattern found at index %d\n", i-m+1);

            if (i == n-1) {
                break;
            } else { 
                char *k = strchr(alf,txt[i+1]);
                if (k) {
                    i += (m - bc[(int)(k-alf)]);
                }
            }
        }
    }
    free(bc);
    
    if (perf)
        printf("[boyer_moore]: Number of comparisons=%d, n=%d, m=%d, n*m=%d, rel=%f\n",c,n,m,n*m,(float)c/n/m);
}    

 /*
  * Utilities
  */

int *build_fa_brute(char pat[], const char alf[]) {
    if (!pat || !alf) {
        return NULL;
    }
    int m = strlen(pat);
    int a = strlen(alf);
    int *fa = calloc((m+1)*a, sizeof(int));
    if (!fa) {
        return NULL;
    }
    int i, j;

    for (i = 0; i < m + 1; i++) {
        for (j = 0; j < a; j++) {
            *(fa + i*a + j) = get_state(pat, i, alf[j]);
        }
    }
    
    return fa;
}

int *build_fa(char pat[], const char alf[]) {
    if (!pat || !alf) {
        return NULL;
    }
    int m = strlen(pat);
    int a = strlen(alf);
    int *fa = calloc((m+1)*a, sizeof(int));
    if (!fa) {
        return NULL;
    }
    int i, j, lps=0;
    
    for (j = 0; j < a; j++) {
        *(fa + j) = (alf[j] == pat[0]);
    }

    for (i = 1; i < m + 1; i++) {
        for (j = 0; j < a; j++) {
            *(fa + i*a + j) = *(fa + lps*a +j);
        }
        char *k = strchr(alf, pat[i]);
        if (!k) {
            free(fa);
            return NULL;
        }
        *(fa + i*a + (int)(k-alf)) = i + 1;
        
        if (i < m) {
            lps = *(fa + lps*a + (int)(k-alf));
        }
    }
    
    return fa;
}

void print_fa(int *fa, int m, int a) {
    int i, j;
    
    for (i = 0; i < m + 1; i++) {
        for (j = 0; j < a; j++) {
            printf("fa[%d][%d]=%d ", i, j, *(fa + i*a + j));
        }
        printf("\n");
    }
}

int get_state_brute(char *pat, int cur, char x) {
    if (!pat) {
        return -1;
    }
    int m = strlen(pat);
    int j, max = 0;
    
    for (j = 0; j < m && j < cur + 1; j++) {
        if (pat[j] == x) {
            int k = j;
            while (--k >= 0) {
                if (pat[k] != pat[cur-j+k]) {
                    break;
                }
            }
            if (k+1 == 0 && max < j+1) {
                max = j+1;
            }
        }
    }
        
    return max;    
}

int get_state(char *pat, int cur, char x) {
    if (!pat) {
        return -1;
    }
    int i, max = 0;
    
    if (pat[cur] == x) {
        return cur+1;
    } else { 
        for (i = cur-1; i >= 0; i--) {
            if (max && pat[i] != pat[cur-max+i+1]) {
                max = 0;
            }
            if (pat[i] == x && max < i+1) {
                max = i+1;
            }
        }
    }
        
    return max;    
}

int* build_lps(char *pat) {
    if (!pat) {
        return NULL;
    }
    int m = strlen(pat);
    int *lps = calloc(m, sizeof(int));
    if (!lps) {
        return NULL;
    }
    
    int i=1, cur=0;
    
    while (i < m) {
        if (pat[i] == pat[cur]) {
            lps[i++] = ++cur;
        } else {
            if (cur) {
                cur = lps[cur-1];
            } else {
                lps[i++] = 0;
            }
        }
    }
    
    return lps;
}
    
int* build_lps_brute(char *pat) {
    if (!pat) {
        return NULL;
    }
    int m = strlen(pat);
    int *lps = calloc(m, sizeof(int));
    if (!lps) {
        return NULL;
    }
    
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < i; j++) {
            if (pat[j] == pat[i]) {
                int k = j;
                while (--k >= 0) {
                    if (pat[k] != pat[i-j+k]) {
                        break;
                    }
                }
                if (k+1 == 0) {
                    lps[i] = j+1;
                }
            }
        }
    }

    return lps;
}

void print_lps(int *lps, int m) {
    int i;
    printf("lps[] is [");
    for (i = 0; i < m-1; i++) {
        printf("%d, ", lps[i]);
    }
    printf("%d]\n", lps[m-1]);
}

int *build_bc(char pat[], const char alf[]) {
    if (!pat || !alf) {
        return NULL;
    }
    int m = strlen(pat);
    int a = strlen(alf);
    
    int *bc = calloc(a, sizeof(int));
    if (!bc) {
        return NULL;
    }
    int i;
    for (i = 0; i < a; i++) {
        bc[i] = -1;
    }
    for (i = 0; i < m; i++) {
        char *j = strchr(alf, pat[i]);
        if (!j) {
            free(bc);
            return NULL;
        }
        bc[(int)(j-alf)] = i;
    }
    
    return bc;
}

int *build_gs(char pat[]) {
    if (!pat) {
        return NULL;
    }
    int m = strlen(pat);
    
    int *gs = calloc(m+1, sizeof(int));
    int *border = calloc(m+1, sizeof(int));
    if (!gs || !border) {
        return NULL;
    }
    int i=m-1, j=m;
    border[m] = m+1;
    
    while (i>=0) {
        if (pat[i] == pat[j]) {
            border[i] = j;
        } else { //border cannot be extended to the left
            while (j < m && pat[i] != pat[j]) {
                if (gs[j+1] ==0) { 
                    gs[j+1] = j-i;
                }
                j = border[j+1]-1;
            }
            border[i] = border[j]-1;
        }
        i--, j--;
    }
    
    j = border[0];
    for(i=0; i<m+1; i++) {
        if (gs[i] == 0) {
            gs[i] = j;
        }
        if (i==j) {
            j = border[j];
        }
        //printf("gs[%d]=%d\n", i, gs[i]);
    }  
    free(border);
    
    return gs;
}