#include "imgui.h"
#include "libgraphics.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

void DrawTextStringWithinArea(char *s,double x, double y, double w, double h, double th) {
	char t[110];
	memset(t, 0, sizeof t);
	int i = 0, j = 0, cntl = 0;
	while (i < strlen(s)) {
		for (; i < strlen(s) && TextStringWidth(t) <= w && s[i] != '\n'; i++, j++) {
			t[j] = s[i];
		}
		cntl++;
		j = 0;
		memset(t, 0, sizeof(t));
		if (s[i] == '\n') i++;
	}
	int cntln = 0, maxl = floor(h / th), k = 1;
	i = 0, j = 0;
	while (i < strlen(s)) {
		for (; i < strlen(s) && TextStringWidth(t) <= w && s[i] != '\n'; i++, j++) {
			t[j] = s[i];
		}
		cntln++;
		if (cntln + maxl > cntl) {
			MovePen(x, y - th * k);
			DrawTextString(t);
			k++;
		}
		// printf("DRAW:%d %d %d %s\n", cntl, cntln, maxl, t);
		memset(t, 0, sizeof(t));
		j = 0;
		if (s[i] == '\n') i++;
	}
}