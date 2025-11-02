// src/image_vertical.c
// gcc src/image_vertical.c -o image_vertical -lpthread -O3 -Wall -Wextra
//
// Versão B: Divisão por FATIAS VERTICAIS
// Cada thread recebe um intervalo de COLUNAS.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define THRESHOLD 128

typedef struct {
    int width;
    int height;
    unsigned char *data;
} Image;

static long long g_contador_global = 0;
static pthread_mutex_t g_mutex_global = PTHREAD_MUTEX_INITIALIZER;

static inline int pixel_match(unsigned char p) {
    return p > THRESHOLD;
}

static Image *carregar_imagem_fake(int w, int h) {
    Image *img = malloc(sizeof(Image));
    img->width = w;
    img->height = h;
    img->data = malloc((size_t)w * h);
    for (long long i = 0; i < (long long)w * h; i++) {
        img->data[i] = (unsigned char)(rand() % 256);
    }
    return img;
}

typedef struct {
    int id;
    Image *img;
    int x_start;
    int x_end;
} ThreadArgsV;

static void *thread_func_v(void *arg) {
    ThreadArgsV *a = arg;
    Image *img = a->img;
    long long local = 0;

    for (int y = 0; y < img->height; y++) {
        int off = y * img->width;
        for (int x = a->x_start; x < a->x_end; x++) {
            unsigned char p = img->data[off + x];
            if (pixel_match(p)) local++;
        }
    }

    pthread_mutex_lock(&g_mutex_global);
    g_contador_global += local;
    pthread_mutex_unlock(&g_mutex_global);
    return NULL;
}

int main(int argc, char **argv) {
    srand(1234);

    int threads = 4;
    int W = 1024, H = 768;

    if (argc > 1) {
        int v = atoi(argv[1]);
        if (v > 0) threads = v;
    }

    Image *img = carregar_imagem_fake(W, H);
    g_contador_global = 0;

    pthread_t *ths = malloc((size_t)threads * sizeof(pthread_t));
    ThreadArgsV *args = malloc((size_t)threads * sizeof(ThreadArgsV));

    int cols_base = W / threads;
    int resto = W % threads;
    int x = 0;

    for (int i = 0; i < threads; i++) {
        int ini = x;
        int fim = ini + cols_base + (i == threads - 1 ? resto : 0);
        args[i].id = i;
        args[i].img = img;
        args[i].x_start = ini;
        args[i].x_end = fim;
        pthread_create(&ths[i], NULL, thread_func_v, &args[i]);
        x = fim;
    }

    for (int i = 0; i < threads; i++)
        pthread_join(ths[i], NULL);

    printf("[VERTICAL] Contador final = %lld\n", g_contador_global);

    free(ths);
    free(args);
    free(img->data);
    free(img);
    return 0;
}
