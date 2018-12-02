#include <stdlib.h>
#include <stdio.h>

typedef struct {
	size_t c;
	size_t r;
	int* a;
} VEC2D;

VEC2D* tdvec(size_t c,size_t r){
	int* a = (int*)malloc(c*r*sizeof(int));
	if (a==NULL){
		return NULL;
	}
	VEC2D* obj = (VEC2D*)malloc(sizeof(VEC2D));
	if (obj==NULL){
		return NULL;
	}
	obj->c = c;
	obj->r = r;
	obj->a = a;
	return obj;
}

//int v(VEC2D* vec,size_t c,size_t r){
	//return
//}

int main(){
	//VEC2D* m = tdvec(10,5);
	//if (m==NULL){
		//return 1;
	//}
	//free(m->a);
	//free(m);
	size_t c = 10;
	size_t r = 5;
	int* a = (int*)malloc(c*r*sizeof(int));
	if (a==NULL){
		return 1;
	}
	int* a2d = (int*)malloc(r*sizeof(int*));
	if (a2d==NULL){
		free(a);
		return 2;
	}
	for (size_t i=0;i<c*r; ++i){
		a[i] = i*10;
	}

	for (size_t i=0;i<r;++i){
		a2d[i] = a[r*i];
	}
	for (size_t i=0;i<r;++i){
		for (size_t j=0;j<c;++j){
			printf("%d ",a2d[i]+j);
		}
		putchar('\n');
	}


	return 0;
}
