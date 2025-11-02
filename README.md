# Processamento Concorrente – Fatias Horizontais e Verticais

Este repositório tem **duas implementações separadas**, cada uma em um arquivo C:

1. `src/image_horizontal.c` → **Versão A** – divide a imagem em **fatias horizontais** (por linhas).
2. `src/image_vertical.c` → **Versão B** – divide a imagem em **fatias verticais** (por colunas).

Ambas fazem a mesma tarefa: **contar quantos pixels da imagem têm valor maior que 128**.  
A diferença é **apenas** na forma de distribuir o trabalho entre as threads.

---

## Como compilar

```bash
# Versão A – horizontal
gcc src/image_horizontal.c -o image_horizontal -lpthread -O3 -Wall -Wextra

# Versão B – vertical
gcc src/image_vertical.c -o image_vertical -lpthread -O3 -Wall -Wextra
```

## Como rodar

```bash
# A) horizontal, 4 threads
./image_horizontal 4

# B) vertical, 4 threads
./image_vertical 4
```

Se não passar o número de threads, cada programa usa **4** por padrão.

---

## O que o contador significa?

Cada thread conta, no seu pedaço da imagem, **quantos pixels têm valor > 128** (constante `THRESHOLD`).  
No final, cada thread soma o seu `contador_local` no `g_contador_global` usando **um mutex global**.  
O valor que aparece no final:

```text
[HORIZONTAL] Contador final = 390000
```

é **o total de pixels que passaram no critério**, na imagem inteira.

---

## Estrutura para GitHub

```text
.
├── src
│   ├── image_horizontal.c
│   └── image_vertical.c
├── docs
│   └── EXPLICACAO.md
└── README.md
```

Você pode fazer:

```bash
git init
git add .
git commit -m "Versões horizontal e vertical"
git remote add origin <url>
git push -u origin main
```

Gerado em 2025-11-02T19:48:25
