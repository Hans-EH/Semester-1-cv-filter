#include <stdio.h>
#include <string.h>

/* 
source code for the levenstein algorithm from 
https://rosettacode.org/wiki/Levenshtein_distance#C
Modified to be used in our program.
fuzzy string matching to find out how similar words are, to counteract misspelling.
*/
int levenshtein(const char *s, const char *t) {
	int ls = strlen(s), lt = strlen(t);
	int d[ls + 1][lt + 1];
 
	for (int i = 0; i <= ls; i++) {
		for (int j = 0; j <= lt; j++) {
			d[i][j] = -1;
		}
	}
	
	int dist(int i, int j){
		if (d[i][j] >= 0) return d[i][j];
 
		int x;
		if (i == ls)
			x = lt - j;
		else if (j == lt)
			x = ls - i;
		else if (s[i] == t[j])
			x = dist(i + 1, j + 1);
		else {
			x = dist(i + 1, j + 1);
 
			int y;
			if ((y = dist(i, j + 1)) < x) x = y;
			if ((y = dist(i + 1, j)) < x) x = y;
			x++;
		}
		return d[i][j] = x;
	}
	return dist(0, 0);
}