/* tvec.c was an attempt to implement a type in C that
 * behaved like std::vector. It uses a malloced-array of
 * type void*, whose 'constructor' requires the size of
 * the values of which it contains. Retrived values would
 * need to be casted before use.
 *
 */

#include <stdlib.h>
#include <stdio.h>

//#define T int

typedef struct {
	size_t cap;
	size_t siz;
	size_t vsize;
	//T* data;
	void** data;
} Vec;

int mk_vec_with_cap(Vec* V,size_t cap,size_t vsize){
	/* Makes V into a Vector with initial
	 * capacity cap.
	 * Returns 0 for a malloc fail.
	 * V should not be already valid.
	 */
	void* a = (void*)malloc(cap*vsize);
	//T* a = (T*)malloc(cap*sizeof(T));
	if (a==NULL){
		return 0;
	}
	V->cap = cap;
	V->siz = 0;
	V->vsize = vsize;
	V->data = a;
	return 1;
}

int mk_vec(Vec* V,size_t vsize){
	/* Makes V into Vector with default
	 * parameters. See mk_vec_with_cap().
	 * V should not be already valid.
	 */
	return mk_vec_with_cap(V,24,vsize);
}

int del_vec(Vec* V){
	/* Deletes Vector V by freeing under
	 * -lying data. Will not delete an
	 * invalid Vector. Only data is deleted.
	 * Can use mk_vec after del_vec.
	 */
	if (V->data==NULL){
		return 0;
	}
	free(V->data);
	V->data = NULL;
	V->cap = V->siz = 0;
	return 1;
}

unsigned vec_msize(Vec* V){
	return (V->vsize)*(V->cap)+sizeof(size_t)*3;
}

int vec_resize(Vec* V,size_t newsize){
	/* Reserves capacity in Vector V with
	 * newsize. Returns 0 on realloc fail.
	 * Used prior to large push_backs().
	 */
	//T* newdata = realloc(V->data,newsize*sizeof(T));
	void* newdata = realloc(V->data,newsize*V->vsize);
	if (newdata==NULL){
		return 0;
	}
	V->data = newdata;
	V->cap = newsize;
	return 1;
}

int vec_reserve(Vec* V,size_t newsize){
	/* Grows Vector V to newsize.
	 * Returns -1 if no resize occurs.
	 * Returns 0 if realloc fails.
	 * Returns 1 if successful.
	 */
	if (newsize<=(V->cap)){
		return -1;
	}
	void* newdata = realloc(V->data,newsize*V->vsize);
	if (newdata==NULL){
		return 0;
	}
	V->data = newdata;
	V->cap = newsize;
	return 1;
}

int vec_push_back(Vec* V,void* t){
	/* Add element to end of Vector V.
	 * Resizes V to double the prior capacity
	 * if insufficent. Returns 0 if resize
	 * fails.
	 */
	if ((V->siz)+1>=(V->cap) && !vec_resize(V,2*V->cap)){
		return 0;
	}
	V->data[V->siz++] = t;
	return 1;
}

int vec_is_empty(Vec* V){
	/* Returns whether V is empty. Empty
	 * does not mean invalid.
	 */
	return (V->siz)==0;
}

void* vec_top(Vec* V){
	/* Returns last element of Vector V.
	 * Returns NULL and prints to stderr
	 * if there is no such element. Please
	 * use vec_is_empty prior to calling.
	 */
	if (V->siz==0){
		fprintf(stderr,"Empty vector has no top");
		return (void*)NULL;
	}
	return (V->data)[(V->siz)-1];
}

int vec_pop_back(Vec* V){
	/* Removes last element of Vector V.
	 * Returns 0 if there is no such element.
	 * Please use vec_is_empty prior to calling.
	 */
	if (V->siz==0){
		fprintf(stderr,"Cannot pop back empty vector");
		return 0;
	}
	(V->siz)--;
	return 1;
}

void* vec_at(Vec* V,size_t i){
	/* Returns value T as position i of
	 * Vector V. Returns NULL if position is
	 * invalid. Better way to verify is check
	 * whether i<V->siz (as below)
	 */
	if (i>=(V->siz)){
		fprintf(stderr,"Out of bounds access");
		return (void*)NULL;
	}
	return (V->data)[i];
}


int main(){
	Vec scores;
	mk_vec_with_cap(&scores,33,sizeof(int));
	printf("%d \n",scores.cap);
	for (size_t i=0;i<32;++i){
		vec_push_back(&scores,(void*)i+65);
	}
	printf("%d \n",scores.cap);
	for (size_t i=0;i<32;++i){
		printf("%c ",(int)vec_top(&scores));
		vec_pop_back(&scores);
	}
	putchar('\n');
	printf("%d\n",vec_msize(&scores));
	del_vec(&scores);

	return 0;
}
