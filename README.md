# O Duelo de Contextos (Processos vs. Threads)

## 1. Objetivo
Realizar um experimento exploratório comparando o overhead de criação, custo de comunicação e consistência de dados entre Processos (padrão POSIX fork) e Threads (padrão POSIX pthreads) em ambiente Unix-like. O desafio consiste em incrementar um contador global até 1.000.000.000, distribuindo a carga entre N unidades de execução (N = 2, 4 e 8).

## 2. Cenários de Experimento

| ID | Modelo | Sincronização | API / Mecanismo |
| :--- | :--- | :--- | :--- |
| **T1** | Threads | Nenhuma | pthreads |
| **T2** | Threads | Mutex | pthread_mutex |
| **P1** | Processos | Nenhuma | fork + shmget/shmat |
| **P2** | Processos | Semáforos | sem_open |

## 3. Requisitos e Ferramentas
* **Ambiente**: Uso obrigatório de Linux ou macOS.
* **Linguagem**: C (padrão POSIX).
* **Compilação**: Makefile.
* **Benchmarking**: Comando `time` para medição de tempo real.
* **Hardware**: Identificação via `lscpu` (Linux) ou `sysctl` (macOS).
* **Gráficos**: Tempo de execução vs. número de trabalhadores (Python ou Excel).

## 4. Entregáveis
O repositório no GitHub deve conter:
* Código-fonte em C organizado.
* Makefile funcional.
* Relatório (README.md) com:
    * Assinatura do Hardware.
    * Tabela de Tempos para os 12 cenários.
    * Análise de Corrupção (T1 e P1).
    * Gráficos de Escalabilidade.
    * Conclusão sobre overhead de criação e eficiência de comunicação.

## 5. Divisão de Tarefas Sugerida (3 Integrantes)
* **Integrante 1**: Implementação do módulo de Threads (T1 e T2) e lógica de incremento.
* **Integrante 2**: Implementação do módulo de Processos (P1 e P2), memória compartilhada e semáforos.
* **Integrante 3**: Automação (Makefile), execução dos testes, coleta de dados de hardware e geração dos gráficos/relatório.
