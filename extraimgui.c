#include "imgui.h"
#include "libgraphics.h"
#include <string.h>
#include <stdio.h>

void DrawTextStringWithWidth(char *s,double x, double y, double tw, double th) {
	char t[110];
	memset(t, 0, sizeof t);
	int i = 0, j = 0, cntl = 0;
	while (i < strlen(s)) {
		for (; i < strlen(s) && TextStringWidth(t) <= tw && s[i] != '\n'; i++, j++) {
			t[j] = s[i];
		}
		MovePen(x, y - th * cntl);
		DrawTextString(t);
		memset(t, 0, sizeof(t));
		j = 0;
		cntl++;
		if (s[i] == '\n') i++;
	}
}