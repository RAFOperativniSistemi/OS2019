#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct matrix_t
{
    int *m_data;
    int m_rows, m_columns;
};

struct matrix_multiplier_args
{
    int x, y;
    struct matrix_t *m1, *m2, *result;

};

void* thread_matrix_multiplier(void *arg);
void matrix_multiply(struct matrix_t *mat_result, struct matrix_t mat1, struct matrix_t mat2);

void show_matrix(struct matrix_t *m);

int main(int argc, char *argv[])
{
    int m1_data[] = {2, 1, 4, 0, 1, 1};
    int m2_data[] = {6, 3, -1, 0, 1, 1, 0, 4, -2, 5, 0, 2};

    struct matrix_t m1 = {m1_data, 2, 3};
    struct matrix_t m2 = {m2_data, 3, 4};

    struct matrix_t res = {NULL, 0, 0};

    show_matrix(&m1);
    printf("\n");

    show_matrix(&m2);
    printf("\n");

    matrix_multiply(&res, m1, m2);

    show_matrix(&res);
    free(res.m_data);

    exit(0);

}

void show_matrix(struct matrix_t *m)
{
    int i, j;
    for(i = 0; i < m->m_rows; i++)
    {
        for(j = 0; j < m->m_columns; j++)
        {
            printf("%2d ", m->m_data[m->m_columns * i + j]);
        }
        printf("\n");
    }
}

void matrix_multiply(struct matrix_t *mat_result, struct matrix_t mat1, struct matrix_t mat2)
{
    if(mat1.m_columns != mat2.m_rows)
        return;

    int n = mat1.m_rows, m = mat2.m_columns;
    int size = n*m;
    int i, j, k = 0;

    if(mat_result->m_rows == 0 || mat_result->m_columns == 0)
    {
        mat_result->m_rows = n;
        mat_result->m_columns = m;
        mat_result->m_data = calloc(size, sizeof(int));
    }

    struct matrix_multiplier_args arguments[size];
    pthread_t threads[size];

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            arguments[k] =(struct matrix_multiplier_args) {j, i, &mat1, &mat2, mat_result};
            pthread_create(&threads[k], NULL, thread_matrix_multiplier, (void*) (arguments + k));
            k ++;
        }
    }

    for(k = 0; k < size; k++)
    {
        pthread_join(threads[k], NULL);
    }

}


void* thread_matrix_multiplier(void *_args)
{
    struct matrix_multiplier_args *args = (struct matrix_multiplier_args *) _args;
    struct matrix_t *m1 = args->m1;
    struct matrix_t *m2 = args->m2;
    struct matrix_t *result = args->result;

    int i, sum = 0, m1_tmp, m2_tmp;
    for(i = 0; i < m1->m_columns; i++)
    {
        m1_tmp = m1->m_data[m1->m_columns * args->y + i];
        m2_tmp = m2->m_data[m2->m_columns * i + args->x];
        sum += m1_tmp * m2_tmp;
    }
    result->m_data[result->m_columns * args->y + args->x] = sum;
}
