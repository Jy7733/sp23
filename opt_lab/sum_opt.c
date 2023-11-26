#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<limits.h>

//#define MAX_NUM 100000000
//#define MAX_NUM 10000
#define MAX_NUM 10000000
//#define MAX_NUM 1000000000
//#define MAX_NUM INT_MAX
//#define MAX_NUM 100000

struct vec {
	int len;
	int *data;
};
typedef struct vec vec_t;

int get_vec_element(vec_t* v, int idx, int *val)
{
	assert(v);

	if(idx >= v->len)
		return 0;

	*val = v->data[idx];
	return 1;
}

int vec_length(vec_t* v)
{
	assert(v);
	return v->len;
}

void combine(vec_t* v, int *dest)
{
	assert(dest);
	int length = vec_length(v); //for 문 밖으로 빼서 지역변수로 선언
	int limit = length - 1;
	int *d = v->data; //벡터의 start 부터 접근할 수 있도록
	int x = 0;

	for (int i=0; i<limit; i+=2)
	{
		x = x +(d[i] + d[i+1]);  
	}

	for (int i=0; i<length; i++){
		x = x + d[i];
	}

	*dest = x;
}

void init(vec_t* v)
{
	assert(v);
	v->len = MAX_NUM;
	v->data = (int*) malloc(sizeof(int) * MAX_NUM);

	// init 
	for(int i = 0; i < MAX_NUM; i++)
		v->data[i] = i;
}

int main()
{
	printf("This is the naive version .. \n");

	vec_t info; 
	int result; 
	// init 
	init(&info);

	// combine 
	combine(&info, &result);
	//printf("combined val = %d\n", result);

	return 0;
}
