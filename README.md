# O Duelo de Contextos (Processos vs. Threads)

## 1. Objetivo
O objetivo deste trabalho é realizar um experimento exploratório comparando o overhead de criação, o custo de comunicação e a consistência de dados entre Processos (padrão POSIX fork) e Threads (padrão POSIX pthreads) em ambiente Unix-like. O problema consiste em incrementar um contador global até o valor de 1.000.000.000 (um bilhão), distribuindo o esforço entre N unidades de execução (onde N = 2, 4 e 8).

## 2. Assinatura do Hardware
Arquitetura:             x86_64
CPUs:                    20
Modelo:                  13th Gen Intel(R) Core(TM) i7-13650HX
Threads por núcleo:      2
Núcleos físicos:         14
Frequência máxima:       4.9 GHz
Cache L3:                24 MB
Virtualização:           VT-x
Sistema Operacional:     Linux (ambiente Unix-like)

## 3. Tabela de Tempos

A tabela abaixo apresenta os tempos reais de execução (em segundos) obtidos através do comando `time`, variando o número de trabalhadores (N) para cada um dos quatro cenários:

| Cenário | Sincronização | API Utilizada | Tempo (N=2) | Tempo (N=4) | Tempo (N=8) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **T1 (Threads)** | Nenhuma | pthreads | 1.13 s | 1.27 s | 1.38 s |
| **T2 (Threads)** | Mutex | pthread_mutex | 43.44 s | 45.58 s | 53.71 s |
| **P1 (Processos)** | Nenhuma | shm + fork | 0.19 s | 0.09 s | 0.15 s |
| **P2 (Processos)** | Semáforos | sem_open + shm | 101.27 s | 113.00 s | 187.98 s |

## 4. Análise de Corrupção

Nos experimentos **T1** e **P1**, o valor esperado era de 1 bilhão. No entanto, os resultados reais obtidos foram:

* **T1 (Threads):**
  * N=2: 502.774.241
  * N=4: 256.883.467
  * N=8: 199.798.420
* **P1 (Processos):**
  * N=2: 504.140.916
  * N=4: 264.625.804
  * N=8: 256.127.022

**Explicação do Erro:**
O contador não alcançou 1 bilhão devido à falta de sincronização, gerando o fenômeno de **Condição de Corrida (Race Condition)**. A operação `counter++` não é atômica para o processador; ela envolve ler o valor da memória, incrementar e escrever o resultado de volta. Como a máquina utilizada possui alto número de núcleos (20 cores lógicos identificados no `results.csv`), as threads e processos rodam verdadeiramente em paralelo (nível de hardware). 

Múltiplas execuções leem simultaneamente o mesmo valor inicial da memória e, ao incrementarem e escreverem, sobrescrevem o progresso das outras execuções, causando intensa perda de contagem. Observa-se que, quanto maior o número de `N` (trabalhadores), maior a perda de informações (mais distante do valor de 1 bilhão), pois as colisões simultâneas de acesso à variável em múltiplos cores aumentam consideravelmente.

## 5. Gráfico de Escalabilidade

Abaixo está o gráfico comparativo do tempo de execução versus o número de trabalhadores (N):

![Gráfico de Escalabilidade](escalabilidade.png)

## 6. Conclusão

Através dos experimentos conduzidos, é possível analisar importantes características arquiteturais entre as abordagens de concorrência e paralelismo:

1. **Overhead de Criação e Execução**: Os Processos (P2) demonstraram um overhead gigantesco comparado às Threads (T2) sob sincronização pesada. Enquanto em T2 com 8 threads a duração foi de ~53s, o cenário P2 com 8 processos levou mais de 3 minutos (~187s). Criar processos via `fork()` implica uma duplicação de recursos mais pesada do que criar threads (que dividem a mesma área de memória do processo pai), embora o impacto mais brutal nos resultados esteja na forma de comunicação e sincronismo.
2. **Eficiência na Comunicação e Sincronização**: Nas Threads (T2), o uso de variáveis e `pthread_mutex` diretos em memória compartilhada é bastante eficiente no Linux. Nos Processos (P2), os incrementos são feitos através de memória compartilhada System V (`shmget`/`shmat`) e protegidos por Semáforos POSIX (`sem_open`/`sem_wait`/`sem_post`). Como cada incremento força uma trava do semáforo nomeado em nível do kernel do SO, ocorre um volume excessivo de chamadas de sistema ("sys_time" alcançou incríveis 1205 segundos somados entre todos os cores). Dessa forma, percebe-se claramente que **Threads tiveram uma comunicação/sincronização absurdamente mais leve e um menor overhead** em relação a Processos utilizando recursos IPC (Semáforos IPC) para tarefas com altíssima granulidade.

Em suma, as abordagens sem sincronismo (T1 e P1) revelaram que, embora a velocidade seja estupenda, ocorre forte inconsistência nos dados em arquiteturas multicore, inviabilizando sua aplicação quando a exatidão das variáveis compartilhadas é fundamental.
