import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('results.csv')

t1 = df[df['experiment'] == 't1']
t2 = df[df['experiment'] == 't2']
p1 = df[df['experiment'] == 'p1']
p2 = df[df['experiment'] == 'p2']

plt.figure(figsize=(10, 6))

plt.plot(t1['n_threads'], t1['real_time'], marker='o', label='T1 (Threads s/ sync)')
plt.plot(t2['n_threads'], t2['real_time'], marker='s', label='T2 (Threads c/ mutex)')
plt.plot(p1['n_threads'], p1['real_time'], marker='^', label='P1 (Processos s/ sync)')
plt.plot(p2['n_threads'], p2['real_time'], marker='D', label='P2 (Processos c/ semáforo)')

plt.title('Escalabilidade: Tempo de Execução vs Número de Trabalhadores')
plt.xlabel('Número de Trabalhadores (N)')
plt.ylabel('Tempo Real de Execução (segundos)')
plt.xticks([2, 4, 8])
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

plt.savefig('escalabilidade.png')
print("Gráfico gerado: escalabilidade.png")
