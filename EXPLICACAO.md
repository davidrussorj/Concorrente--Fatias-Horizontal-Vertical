# Explicação das duas versões

## 1. Versão A – Fatias Horizontais (`image_horizontal.c`)
- A imagem é percorrida **linha por linha**.
- Se temos N threads e H linhas, cada thread recebe aproximadamente `H / N` linhas.
- Vantagem: acesso sequencial na memória (melhor para imagens armazenadas row-major).
- Padrão usado:
  - contador_local dentro da thread
  - ao final: `pthread_mutex_lock(...)` → soma no contador global → `pthread_mutex_unlock(...)`

## 2. Versão B – Fatias Verticais (`image_vertical.c`)
- A imagem é percorrida **coluna por coluna**.
- Se temos N threads e W colunas, cada thread recebe aproximadamente `W / N` colunas.
- Isso mostra que conseguimos mudar o **eixo de paralelização** sem mudar o padrão de sincronização.
- Mesmo padrão de contador.

## 3. Por que manter duas versões?
Porque muitas vezes o professor quer ver se você:
1) entendeu o uso de threads e mutex;
2) entendeu que a partição dos dados pode ser feita em **direções diferentes** sem mudar a lógica geral;
3) consegue comparar acesso horizontal vs vertical (localidade de memória).

## 4. Como testar
1. Compile os dois.
2. Rode os dois com o mesmo número de threads.
3. Como a imagem é gerada com `srand(1234);`, os dois devem dar o mesmo resultado.
