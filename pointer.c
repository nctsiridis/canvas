#include <stdio.h>
#include <stdlib.h>

typedef struct {
	void* thing;
} Data;

typedef struct {
	int x, y;
} Pair;

Pair* make_pair() {
	Pair *res = malloc(sizeof(Pair));
	res->x = 10;
	res->y = 20;
	return res;
}

void assign(void** thing) {
	*thing = make_pair();
}

int main() {
	Data data = {NULL};
	assign(&data.thing);
	printf("{%d}, {%d}\n", ((Pair*)data.thing)->x, ((Pair*)data.thing)->y);
	return 0;
}
