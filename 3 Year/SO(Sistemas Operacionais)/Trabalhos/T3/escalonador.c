#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define GANTT_CHART_BUFFER_SIZE 8192

/*
OBS: alguns comentarios estao como 'padrão', ou seja, é algo que ja fiz antes ou parecido, porém nem sempre vai estar em ordem, pois eu fiz alguns algoritmos primeiro que outros
*/

// estados do processo
typedef enum {
    NEW,
    READY,
    EXECUTING,
    WAITING_IO, // Esperando na fila para o dispositivo de E/S
    IO,         // Executando E/S
    TERMINATED
} ProcessState;

typedef struct Process {
    unsigned int pid;
    unsigned int priority;
    unsigned int arrival_time;

    unsigned int *bursts;
    unsigned int *original_bursts; // resetar a simulacao
    unsigned int total_bursts;
    unsigned int current_burst;

    // métricas
    unsigned int turnaround_time;
    unsigned int waiting_time;
    unsigned int completion_time;

    // controle da simulacao
    unsigned int io_finish_time; // usado no modo paralelo
    ProcessState state;
} Process;


//< Funções Auxiliares

// libera a memoria alocado pelos processos
void freeProcesses(Process *processes, unsigned int count) {
    if (!processes) return;
    for (unsigned int i = 0; i < count; i++) {
        free(processes[i].bursts);
        free(processes[i].original_bursts);
    }
    free(processes);
}

// Reseta o estado dos processos para que um novo algoritmo possa ser executado
void resetProcesses(Process *processes, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        processes[i].state = NEW;
        processes[i].current_burst = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].completion_time = 0;
        processes[i].io_finish_time = 0;

        // restaura os burts originaiss
        memcpy(processes[i].bursts, processes[i].original_bursts, processes[i].total_bursts * sizeof(unsigned int));
    }
}

// cria as métricas finais do algoritmo
void printMetrics(const char* algorithm_name, Process *processes, unsigned int count, unsigned int total_time, unsigned int cpu_busy_time, const char* gantt_chart, const char* base_filename) {
    char out_filename[256];
    snprintf(out_filename, sizeof(out_filename), "%s.out", base_filename);
    FILE *fp = fopen(out_filename, "a");
    if (!fp) {
        perror("Nao foi possivel abrir o arquivo de saida");
        return;
    }

    // APAGAR: Garante que haja um espaço entre as saídas dos algoritmos
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) > 0) {
        fprintf(fp, "\n\n");
    }

    fprintf(fp, "Algoritmo: %s\n", algorithm_name);
    fprintf(fp, "Diagrama de Gantt: %s\n", gantt_chart);

    float cpu_utilization = total_time > 0 ? ((float)cpu_busy_time / total_time) * 100.0f : 0.0f;
    float throughput = total_time > 0 ? (float)count / (total_time / 1000.0f) : 0.0f;

    fprintf(fp, "Utilizacao da CPU: %.2f%%\n", cpu_utilization);
    fprintf(fp, "Throughput: %.2f processos/segundo\n", throughput);

    fprintf(fp, "Tempo de Turnaround:\n");
    float avg_turnaround = 0;
    if (count > 0) {
        for (unsigned int i = 0; i < count; i++) {
            fprintf(fp, "  P%d: %u ms\n", processes[i].pid, processes[i].turnaround_time);
            avg_turnaround += processes[i].turnaround_time;
        }
        avg_turnaround /= count;
    }
    fprintf(fp, "  Tempo Medio de Turnaround: %.2f ms\n", avg_turnaround);

    fprintf(fp, "Tempo de Espera:\n");
    float avg_waiting = 0;
    if (count > 0) {
        for (unsigned int i = 0; i < count; i++) {
            fprintf(fp, "  P%d: %u ms\n", processes[i].pid, processes[i].waiting_time);
            avg_waiting += processes[i].waiting_time;
        }
        avg_waiting /= count;
    }
    fprintf(fp, "  Tempo Medio de Espera: %.2f ms\n", avg_waiting);

    fclose(fp);
}

// le o arquivo de entrada e cria a estrutura dos processos
Process* readInputFile(const char *filename, unsigned int *process_count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo de entrada");
        return NULL;
    }

    *process_count = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strlen(buffer) > 2) (*process_count)++;
    }
    rewind(fp);

    if (*process_count == 0) {
        fclose(fp);
        return NULL;
    }

    Process *processes = malloc(*process_count * sizeof(Process));
    if (!processes) {
        perror("Erro ao alocar memoria para processos");
        fclose(fp);
        return NULL;
    }

    for (unsigned int i = 0; i < *process_count; i++) {
        fgets(buffer, sizeof(buffer), fp);

        char* original_buffer_ptr = buffer;

        char line_copy[MAX_LINE_LENGTH];
        strncpy(line_copy, original_buffer_ptr, sizeof(line_copy));
        line_copy[sizeof(line_copy) - 1] = '\0';

        int element_count = 0;
        char *token = strtok(line_copy, " \t\n");
        while (token) {
            element_count++;
            token = strtok(NULL, " \t\n");
        }

        if (element_count < 2) continue;

        int burst_count = element_count - 2;

        processes[i].bursts = malloc(burst_count * sizeof(unsigned int));
        processes[i].original_bursts = malloc(burst_count * sizeof(unsigned int));
        processes[i].total_bursts = burst_count;

        token = strtok(original_buffer_ptr, " \t\n");
        processes[i].priority = atoi(token);
        token = strtok(NULL, " \t\n");
        processes[i].arrival_time = atoi(token);

        for (int j = 0; j < burst_count; j++) {
            token = strtok(NULL, " \t\n");
            processes[i].bursts[j] = atoi(token);
            processes[i].original_bursts[j] = processes[i].bursts[j];
        }

        processes[i].pid = i + 1;
        processes[i].state = NEW;
    }

    fclose(fp);
    return processes;
}

// Funções Auxiliares>

// Algoritmos de Escalonamento

void FCFS(Process *processes, unsigned int count, const char* base_filename, bool is_io_sequential) {
    resetProcesses(processes, count);
    if (count == 0) return; // sem processos

    unsigned int max_queue_size = count * 5; // pra nao ter erro na memoria

    int *ready_queue = malloc(max_queue_size * sizeof(int));
    int ready_tail = 0; // Usaremos apenas a cauda (tail)
    int *io_queue = malloc(max_queue_size * sizeof(int));
    int io_head = 0, io_tail = 0;

    int running_process_idx = -1;
    int io_process_idx = -1;
    unsigned int io_device_free_time = 0;

    // variaveis para a simulacaoo
    unsigned int terminated_count = 0;
    unsigned int cpu_busy_time = 0;
    char gantt_chart[GANTT_CHART_BUFFER_SIZE] = "";
    int last_pid_on_cpu = -2;

    // padrão...
    unsigned int simulation_start_time = -1;
    for (unsigned int i = 0; i < count; i++) {
        if (simulation_start_time == (unsigned int)-1 || processes[i].arrival_time < simulation_start_time) {
            simulation_start_time = processes[i].arrival_time;
        }
    }
    unsigned int currentTime = simulation_start_time;

    // padrão...
    while (terminated_count < count) {

        // adiciona os processos que chegaram ou terminaram E/S no final da fila de prontos
        for (unsigned int i = 0; i < count; i++) {
            if (processes[i].state == NEW && processes[i].arrival_time <= currentTime) {
                processes[i].state = READY;
                ready_queue[ready_tail++] = i;
            }
        }
        if (is_io_sequential) {
            if (io_process_idx != -1 && currentTime >= io_device_free_time) {
                processes[io_process_idx].state = READY;
                ready_queue[ready_tail++] = io_process_idx;
                io_process_idx = -1;
            }
        } else {
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == IO && processes[i].io_finish_time <= currentTime) {
                    processes[i].state = READY;
                    ready_queue[ready_tail++] = i;
                }
            }
        }

        // processo na CPU termina burst
        if (running_process_idx != -1 && processes[running_process_idx].bursts[processes[running_process_idx].current_burst] == 0) {
            processes[running_process_idx].current_burst++;
            if (processes[running_process_idx].current_burst >= processes[running_process_idx].total_bursts) {
                processes[running_process_idx].state = TERMINATED;
                processes[running_process_idx].completion_time = currentTime;
                processes[running_process_idx].turnaround_time = currentTime - processes[running_process_idx].arrival_time;
                terminated_count++;
            } else {
                processes[running_process_idx].state = WAITING_IO;
                io_queue[io_tail++] = running_process_idx;
            }
            running_process_idx = -1;
        }

        // inicia E/S pra os processos na fila de espera
        if (is_io_sequential) {
            if (io_process_idx == -1 && io_head < io_tail) {
                io_process_idx = io_queue[io_head++];
                processes[io_process_idx].state = IO;
                io_device_free_time = currentTime + processes[io_process_idx].bursts[processes[io_process_idx].current_burst];
                processes[io_process_idx].current_burst++;
            }
        } else {
            while (io_head < io_tail) {
                int p_idx = io_queue[io_head++];
                processes[p_idx].state = IO;
                processes[p_idx].io_finish_time = currentTime + processes[p_idx].bursts[processes[p_idx].current_burst];
                processes[p_idx].current_burst++;
            }
        }

        // se a cpu esta livre, escalona o proximo da frente da fila
        if (running_process_idx == -1 && ready_tail > 0) {
            running_process_idx = ready_queue[0]; // pega o primeiro elemento sempre
            processes[running_process_idx].state = EXECUTING;

            // desloca a fila para a esquerda para remover o elemento
            for (int i = 0; i < ready_tail - 1; i++) {
                ready_queue[i] = ready_queue[i + 1];
            }
            ready_tail--;
        }

        // atualiza e avança no tempo
        int current_pid_on_cpu = (running_process_idx == -1) ? -1 : processes[running_process_idx].pid;
        if (current_pid_on_cpu != last_pid_on_cpu) {
            if (last_pid_on_cpu != -2) {
                char temp[50];
                if (last_pid_on_cpu >= 0) sprintf(temp, "P%d %u|", last_pid_on_cpu, currentTime);
                else sprintf(temp, "*** %u|", currentTime);
                strncat(gantt_chart, temp, sizeof(gantt_chart) - strlen(gantt_chart) - 1);
            }
            last_pid_on_cpu = current_pid_on_cpu;
        }

        for (int i = 0; i < ready_tail; i++) {
            processes[ready_queue[i]].waiting_time++;
        }

        if (running_process_idx != -1) {
            processes[running_process_idx].bursts[processes[running_process_idx].current_burst]--;
            cpu_busy_time++;
        }

        // salta no tempo se a cpu esta ociosa
        if (running_process_idx == -1 && ready_tail == 0 && io_head == io_tail && terminated_count < count) {
            unsigned int next_event_time = -1;
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == NEW && (next_event_time == -1 || processes[i].arrival_time < next_event_time)) next_event_time = processes[i].arrival_time;
                if (processes[i].state == IO && (next_event_time == -1 || processes[i].io_finish_time < next_event_time)) next_event_time = processes[i].io_finish_time;
            }
            if (is_io_sequential && io_process_idx != -1 && (next_event_time == -1 || io_device_free_time < next_event_time)) next_event_time = io_device_free_time;

            if (next_event_time > currentTime && next_event_time != (unsigned int)-1) {
                currentTime = next_event_time;
                continue;
            }
        }
        currentTime++;
    }

    // parte de criação do diagrama de gantt(padrão para todos os algoritmos)

    if (strlen(gantt_chart) > 0 && gantt_chart[strlen(gantt_chart)-1] == '|') {
        gantt_chart[strlen(gantt_chart) - 1] = '\0';
    }

    char final_gantt[GANTT_CHART_BUFFER_SIZE * 2];
    sprintf(final_gantt, "%u[%s]", simulation_start_time, gantt_chart);
    printMetrics("FCFS", processes, count, currentTime - simulation_start_time, cpu_busy_time, final_gantt, base_filename);

    free(ready_queue);
    free(io_queue);
}

void SJF(Process *processes, unsigned int count, const char* base_filename, bool is_io_sequential) {
    resetProcesses(processes, count);
    if (count == 0) return; // sem processos

    unsigned int max_queue_size = count * 5;
    int *ready_queue = malloc(max_queue_size * sizeof(int));
    int ready_tail = 0;
    int *io_queue = malloc(max_queue_size * sizeof(int));
    int io_head = 0, io_tail = 0;

    int running_process_idx = -1;

    int io_process_idx = -1;
    unsigned int io_device_free_time = 0;

    // variaveis para métricas e diagrama de gantt
    unsigned int terminated_count = 0;
    unsigned int cpu_busy_time = 0;
    char gantt_chart[GANTT_CHART_BUFFER_SIZE] = "";
    int last_pid_on_cpu = -2;

    // calcula o tempo inicial, tempo de chegado do primeiro processo
    unsigned int simulation_start_time = -1;
    for (unsigned int i = 0; i < count; i++) {
        if (simulation_start_time == (unsigned int)-1 || processes[i].arrival_time < simulation_start_time) {
            simulation_start_time = processes[i].arrival_time;
        }
    }

    unsigned int currentTime = simulation_start_time;

    // enquanto nenhum processo termina faça
    while (terminated_count < count) {

        // adiciona processos que chegaram no tempo 'currentTime' na fila de prontos
        for (unsigned int i = 0; i < count; i++) {
            if (processes[i].state == NEW && processes[i].arrival_time <= currentTime) {
                processes[i].state = READY;
                ready_queue[ready_tail++] = i;
            }
        }

        // verifica se algum processo terminou sua operação de E/S, para poder voltar na fila de prontos
        if (is_io_sequential) {
            if (io_process_idx != -1 && currentTime >= io_device_free_time) {
                processes[io_process_idx].state = READY;
                ready_queue[ready_tail++] = io_process_idx;
                io_process_idx = -1;
            }
        } else { // se o modo de E/S é paralelo '-seq'
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == IO && processes[i].io_finish_time <= currentTime) {
                    processes[i].state = READY;
                    ready_queue[ready_tail++] = i;
                }
            }
        }

        // checa se o processo atualmente na CPU terminou seu burst
        if (running_process_idx != -1 && processes[running_process_idx].bursts[processes[running_process_idx].current_burst] == 0) {
            processes[running_process_idx].current_burst++;
            if (processes[running_process_idx].current_burst >= processes[running_process_idx].total_bursts) {

                processes[running_process_idx].state = TERMINATED;
                processes[running_process_idx].completion_time = currentTime;
                processes[running_process_idx].turnaround_time = currentTime - processes[running_process_idx].arrival_time;

                terminated_count++;
            } else {

                processes[running_process_idx].state = WAITING_IO;
                io_queue[io_tail++] = running_process_idx;
            }
            running_process_idx = -1; // CPU livre
        }

        // inicia operacoes de E/S para processos que estão na fila de espera de E/S
        if (is_io_sequential) {
            if (io_process_idx == -1 && io_head < io_tail) {
                io_process_idx = io_queue[io_head++];
                processes[io_process_idx].state = IO;
                io_device_free_time = currentTime + processes[io_process_idx].bursts[processes[io_process_idx].current_burst];
                processes[io_process_idx].current_burst++;
            }
        } else {
            // modo paralelo
            while (io_head < io_tail) {
                int p_idx = io_queue[io_head++];
                processes[p_idx].state = IO;
                processes[p_idx].io_finish_time = currentTime + processes[p_idx].bursts[processes[p_idx].current_burst];
                processes[p_idx].current_burst++;
            }
        }

        // se a cpu esta livre e ainda tem processos prontos
        if (running_process_idx == -1 && ready_tail > 0) {
            int shortest_job_q_idx = 0;

            // percorre a fila de prontos pra encontrar o processo com menor burst de CPU
            for (int i = 1; i < ready_tail; i++) {
                unsigned int current_burst_time = processes[ready_queue[i]].bursts[processes[ready_queue[i]].current_burst];
                unsigned int shortest_burst_time = processes[ready_queue[shortest_job_q_idx]].bursts[processes[ready_queue[shortest_job_q_idx]].current_burst];
                if (current_burst_time < shortest_burst_time) {
                    shortest_job_q_idx = i;
                }
            }

            // O processo com menor burst é escolhido para a execução
            running_process_idx = ready_queue[shortest_job_q_idx];
            processes[running_process_idx].state = EXECUTING;
            for (int i = shortest_job_q_idx; i < ready_tail - 1; i++) {
                ready_queue[i] = ready_queue[i + 1];
            }
            ready_tail--;
        }

        // Atualiza o diagrama de gantt
        int current_pid_on_cpu = (running_process_idx == -1) ? -1 : processes[running_process_idx].pid;
        if (current_pid_on_cpu != last_pid_on_cpu) {
            if (last_pid_on_cpu != -2) {
                char temp[50];
                if (last_pid_on_cpu >= 0) sprintf(temp, "P%d %u|", last_pid_on_cpu, currentTime);
                else sprintf(temp, "*** %u|", currentTime);
                strncat(gantt_chart, temp, sizeof(gantt_chart) - strlen(gantt_chart) - 1);
            }
            last_pid_on_cpu = current_pid_on_cpu;
        }

        // incrementa o tempo de espera para todos os processos que estão na fila de prontos
        for (int i = 0; i < ready_tail; i++) {
            processes[ready_queue[i]].waiting_time++;
        }

        // se ainda tem um processo executando na CPU, decrementa o tempo restante do seu burst e incrementa o tempo de ocupacao da cpu
        if (running_process_idx != -1) {
            processes[running_process_idx].bursts[processes[running_process_idx].current_burst]--;
            cpu_busy_time++;
        }


        // Se a cpu esta ociosa, a fila de prontos esta vazia e a fila de E/S tbm
        // salta no tempo se ainda tem processos para terminar
        // para o proximo envento significativo (chegada de processo ou termino de E/S)
        if (running_process_idx == -1 && ready_tail == 0 && io_head == io_tail && terminated_count < count) {
            unsigned int next_event_time = -1;

            // busca o proximo evento mais cedo
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == NEW && (next_event_time == -1 || processes[i].arrival_time < next_event_time)) next_event_time = processes[i].arrival_time;
                if (processes[i].state == IO && (next_event_time == -1 || processes[i].io_finish_time < next_event_time)) next_event_time = processes[i].io_finish_time;
            }
            // p/ E/S sequencial, verifica o tempo de liberação do unico dispositivo de E/S
            if (is_io_sequential && io_process_idx != -1 && (next_event_time == -1 || io_device_free_time < next_event_time)) next_event_time = io_device_free_time;
            if (next_event_time > currentTime && next_event_time != (unsigned int)-1) { // seo proximo evento esta no futuro avança o tempo pra ele e recomeca o loop
                currentTime = next_event_time;
                continue;
            }
        }
        currentTime++;
    }

    // padrão

    if (strlen(gantt_chart) > 0 && gantt_chart[strlen(gantt_chart)-1] == '|') {
        gantt_chart[strlen(gantt_chart) - 1] = '\0';
    }
    char final_gantt[GANTT_CHART_BUFFER_SIZE * 2];
    sprintf(final_gantt, "%u[%s]", simulation_start_time, gantt_chart);
    printMetrics("SJF", processes, count, currentTime - simulation_start_time, cpu_busy_time, final_gantt, base_filename);

    free(ready_queue);
    free(io_queue);
}

void SRTF(Process *processes, unsigned int count, const char* base_filename, bool is_io_sequential) {
    resetProcesses(processes, count);
    if (count == 0) return; // sem processos

    unsigned int max_queue_size = count * 5;

    int *ready_queue = malloc(max_queue_size * sizeof(int));
    int ready_tail = 0;
    int *io_queue = malloc(max_queue_size * sizeof(int));
    int io_head = 0, io_tail = 0;

    int running_process_idx = -1;
    int io_process_idx = -1;
    unsigned int io_device_free_time = 0;

    // padrão...
    unsigned int terminated_count = 0;
    unsigned int cpu_busy_time = 0;
    char gantt_chart[GANTT_CHART_BUFFER_SIZE] = "";
    int last_pid_on_cpu = -2;

    // padrão...
    unsigned int simulation_start_time = -1;
    for (unsigned int i = 0; i < count; i++) {
        if (simulation_start_time == (unsigned int)-1 || processes[i].arrival_time < simulation_start_time) {
            simulation_start_time = processes[i].arrival_time;
        }
    }
    unsigned int currentTime = simulation_start_time;

    // padrão...
    while (terminated_count < count) {
        for (unsigned int i = 0; i < count; i++) {
            if (processes[i].state == NEW && processes[i].arrival_time <= currentTime) {
                processes[i].state = READY;
                ready_queue[ready_tail++] = i;
            }
        }

        // verifica se algum processo terminou sua operacao de E/S e volta na fila de prontos
        if (is_io_sequential) {
            if (io_process_idx != -1 && currentTime >= io_device_free_time) {
                processes[io_process_idx].state = READY;
                ready_queue[ready_tail++] = io_process_idx;
                io_process_idx = -1;
            }
        } else { // se modo de E/S é paralelo
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == IO && processes[i].io_finish_time <= currentTime) {
                    processes[i].state = READY;
                    ready_queue[ready_tail++] = i;
                }
            }
        }

        // checa se o processo atualmente na cpu terminou sua burst
        if (running_process_idx != -1 && processes[running_process_idx].bursts[processes[running_process_idx].current_burst] == 0) {
            processes[running_process_idx].current_burst++;
            if (processes[running_process_idx].current_burst >= processes[running_process_idx].total_bursts) {
                processes[running_process_idx].state = TERMINATED;
                processes[running_process_idx].completion_time = currentTime;
                processes[running_process_idx].turnaround_time = currentTime - processes[running_process_idx].arrival_time;
                terminated_count++;
            } else {
                processes[running_process_idx].state = WAITING_IO;
                io_queue[io_tail++] = running_process_idx;
            }
            running_process_idx = -1;
        }

        // encontra o processo com o menor tempo restante
        int best_p_idx = -1;
        // se tem um processo rodando, ele é candidato inicial para o "melhor"
        if (running_process_idx != -1) {
            best_p_idx = running_process_idx;
        }

        // percorre a fila de prontos pra encontrar um processo com tempo restante menor
        for (int i = 0; i < ready_tail; i++) {
            int candidate_idx = ready_queue[i];
            if (best_p_idx == -1 || processes[candidate_idx].bursts[processes[candidate_idx].current_burst] < processes[best_p_idx].bursts[processes[best_p_idx].current_burst]) {
                best_p_idx = candidate_idx;
            }
        }
        // se um novo melhor processo foi encontrado e ele é diferente do que estava rodando
        if (best_p_idx != -1 && best_p_idx != running_process_idx) {
            // se havia um processo rodando, ele é preemptado e volta para a fila de prontos
            if (running_process_idx != -1) {
                processes[running_process_idx].state = READY;
                ready_queue[ready_tail++] = running_process_idx;
            }
            // o novo melhor processo assume a cpu
            running_process_idx = best_p_idx;
            processes[running_process_idx].state = EXECUTING;

            // remove o processo que acabou de ser escalonado da fila de prontos
            int q_idx_to_remove = -1;
            for(int i = 0; i < ready_tail; i++) {
                if(ready_queue[i] == running_process_idx) {
                    q_idx_to_remove = i;
                    break;
                }
            }
            if(q_idx_to_remove != -1) {
                for (int i = q_idx_to_remove; i < ready_tail - 1; i++) {
                    ready_queue[i] = ready_queue[i + 1];
                }
                ready_tail--;
            }
        }

        // padrão...
        if (is_io_sequential) {
             if (io_process_idx == -1 && io_head < io_tail) {
                io_process_idx = io_queue[io_head++];
                processes[io_process_idx].state = IO;
                io_device_free_time = currentTime + processes[io_process_idx].bursts[processes[io_process_idx].current_burst];
                processes[io_process_idx].current_burst++;
            }
        } else {
            while (io_head < io_tail) {
                int p_idx = io_queue[io_head++];
                processes[p_idx].state = IO;
                processes[p_idx].io_finish_time = currentTime + processes[p_idx].bursts[processes[p_idx].current_burst];
                processes[p_idx].current_burst++;
            }
        }

        // padrão...
        int current_pid_on_cpu = (running_process_idx == -1) ? -1 : processes[running_process_idx].pid;
        if (current_pid_on_cpu != last_pid_on_cpu) {
            if (last_pid_on_cpu != -2) {
                char temp[50];
                if (last_pid_on_cpu >= 0) sprintf(temp, "P%d %u|", last_pid_on_cpu, currentTime);
                else sprintf(temp, "*** %u|", currentTime);
                strncat(gantt_chart, temp, sizeof(gantt_chart) - strlen(gantt_chart) - 1);
            }
            last_pid_on_cpu = current_pid_on_cpu;
        }

        for (int i = 0; i < ready_tail; i++) {
            processes[ready_queue[i]].waiting_time++;
        }
        if (running_process_idx != -1) {
            processes[running_process_idx].bursts[processes[running_process_idx].current_burst]--;
            cpu_busy_time++;
        }

        // padrão...
        if (running_process_idx == -1 && ready_tail == 0 && io_head == io_tail && terminated_count < count) {
            unsigned int next_event_time = -1;
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == NEW && (next_event_time == -1 || processes[i].arrival_time < next_event_time)) next_event_time = processes[i].arrival_time;
                if (processes[i].state == IO && (next_event_time == -1 || processes[i].io_finish_time < next_event_time)) next_event_time = processes[i].io_finish_time;
            }
            if (is_io_sequential && io_process_idx != -1 && (next_event_time == -1 || io_device_free_time < next_event_time)) next_event_time = io_device_free_time;
            if (next_event_time > currentTime && next_event_time != (unsigned int)-1) {
                currentTime = next_event_time;
                continue;
            }
        }
        currentTime++;
    }

    // padrão...
    if (strlen(gantt_chart) > 0 && gantt_chart[strlen(gantt_chart)-1] == '|') {
        gantt_chart[strlen(gantt_chart) - 1] = '\0';
    }
    char final_gantt[GANTT_CHART_BUFFER_SIZE * 2];
    sprintf(final_gantt, "%u[%s]", simulation_start_time, gantt_chart);
    printMetrics("SRTF", processes, count, currentTime - simulation_start_time, cpu_busy_time, final_gantt, base_filename);

    free(ready_queue);
    free(io_queue);
}

void PriorityPreemptive(Process *processes, unsigned int count, const char* base_filename, bool is_io_sequential) {
    resetProcesses(processes, count);
    if (count == 0) return; // sem processos

    unsigned int max_queue_size = count * 5;
    int *ready_queue = malloc(max_queue_size * sizeof(int));
    int ready_tail = 0;
    int *io_queue = malloc(max_queue_size * sizeof(int));
    int io_head = 0, io_tail = 0;

    int running_process_idx = -1;
    int io_process_idx = -1;
    unsigned int io_device_free_time = 0;

    unsigned int terminated_count = 0;
    unsigned int cpu_busy_time = 0;
    char gantt_chart[GANTT_CHART_BUFFER_SIZE] = "";
    int last_pid_on_cpu = -2;

    unsigned int simulation_start_time = -1;
    for (unsigned int i = 0; i < count; i++) {
        if (simulation_start_time == (unsigned int)-1 || processes[i].arrival_time < simulation_start_time) {
            simulation_start_time = processes[i].arrival_time;
        }
    }
    unsigned int currentTime = simulation_start_time;

    while (terminated_count < count) {
        for (unsigned int i = 0; i < count; i++) {
            if (processes[i].state == NEW && processes[i].arrival_time <= currentTime) {
                processes[i].state = READY;
                ready_queue[ready_tail++] = i;
            }
        }
        if (is_io_sequential) {
            if (io_process_idx != -1 && currentTime >= io_device_free_time) {
                processes[io_process_idx].state = READY;
                ready_queue[ready_tail++] = io_process_idx;
                io_process_idx = -1;
            }
        } else {
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == IO && processes[i].io_finish_time <= currentTime) {
                    processes[i].state = READY;
                    ready_queue[ready_tail++] = i;
                }
            }
        }

        if (running_process_idx != -1 && processes[running_process_idx].bursts[processes[running_process_idx].current_burst] == 0) {
            processes[running_process_idx].current_burst++;
            if (processes[running_process_idx].current_burst >= processes[running_process_idx].total_bursts) {
                processes[running_process_idx].state = TERMINATED;
                processes[running_process_idx].completion_time = currentTime;
                processes[running_process_idx].turnaround_time = currentTime - processes[running_process_idx].arrival_time;
                terminated_count++;
            } else {
                processes[running_process_idx].state = WAITING_IO;
                io_queue[io_tail++] = running_process_idx;
            }
            running_process_idx = -1;
        }

        // encontra o processo com maior prioridade entre os prontos e oque esta executando
        int best_p_idx = -1;
        unsigned int highest_priority = 0;

        // se ja ha um processo rodando, ele é o candidato inicial pra o 'melhor'
        if (running_process_idx != -1) {
            best_p_idx = running_process_idx;
            highest_priority = processes[best_p_idx].priority;
        }

        // percorre a fila de prontos pra encontrar um processo com prioridade maior
        for (int i = 0; i < ready_tail; i++) {
            int candidate_idx = ready_queue[i];

            // se nao há um 'melhor' ainda, ou se o candidato tem um prioridade mauor, ai ele se torna o melhor
            if (best_p_idx == -1 || processes[candidate_idx].priority > highest_priority) {
                highest_priority = processes[candidate_idx].priority;
                best_p_idx = candidate_idx;
            }
        }

        // padrão...
        if (best_p_idx != -1 && best_p_idx != running_process_idx) {
            // se havia um processo rodando, ele é preemptivo (interrompido) e volta para a fila de prontos
            if (running_process_idx != -1) {
                processes[running_process_idx].state = READY;
                ready_queue[ready_tail++] = running_process_idx;
            }

            // o novo 'melhor' processo (de maior prioridade) assume a cpu
            running_process_idx = best_p_idx;
            processes[running_process_idx].state = EXECUTING;

            // remove o processo que acabou de ser escalonado da fila de prontos
            int q_idx_to_remove = -1;
            for(int i = 0; i < ready_tail; i++) {
                if(ready_queue[i] == running_process_idx) {
                    q_idx_to_remove = i;
                    break;
                }
            }
            if(q_idx_to_remove != -1) {
                for (int i = q_idx_to_remove; i < ready_tail - 1; i++) {
                    ready_queue[i] = ready_queue[i + 1];
                }
                ready_tail--;
            }
        }
       // padrão...

        if (is_io_sequential) {
             if (io_process_idx == -1 && io_head < io_tail) {
                io_process_idx = io_queue[io_head++];
                processes[io_process_idx].state = IO;
                io_device_free_time = currentTime + processes[io_process_idx].bursts[processes[io_process_idx].current_burst];
                processes[io_process_idx].current_burst++;
            }
        } else {
            while (io_head < io_tail) {
                int p_idx = io_queue[io_head++];
                processes[p_idx].state = IO;
                processes[p_idx].io_finish_time = currentTime + processes[p_idx].bursts[processes[p_idx].current_burst];
                processes[p_idx].current_burst++;
            }
        }

        int current_pid_on_cpu = (running_process_idx == -1) ? -1 : processes[running_process_idx].pid;
        if (current_pid_on_cpu != last_pid_on_cpu) {
            if (last_pid_on_cpu != -2) {
                char temp[50];
                if (last_pid_on_cpu >= 0) sprintf(temp, "P%d %u|", last_pid_on_cpu, currentTime);
                else sprintf(temp, "*** %u|", currentTime);
                strncat(gantt_chart, temp, sizeof(gantt_chart) - strlen(gantt_chart) - 1);
            }
            last_pid_on_cpu = current_pid_on_cpu;
        }

        for (int i = 0; i < ready_tail; i++) {
            processes[ready_queue[i]].waiting_time++;
        }
        if (running_process_idx != -1) {
            processes[running_process_idx].bursts[processes[running_process_idx].current_burst]--;
            cpu_busy_time++;
        }

        if (running_process_idx == -1 && ready_tail == 0 && io_head == io_tail && terminated_count < count) {
            unsigned int next_event_time = -1;
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == NEW && (next_event_time == -1 || processes[i].arrival_time < next_event_time)) next_event_time = processes[i].arrival_time;
                if (processes[i].state == IO && (next_event_time == -1 || processes[i].io_finish_time < next_event_time)) next_event_time = processes[i].io_finish_time;
            }
            if (is_io_sequential && io_process_idx != -1 && (next_event_time == -1 || io_device_free_time < next_event_time)) next_event_time = io_device_free_time;
            if (next_event_time > currentTime && next_event_time != (unsigned int)-1) {
                currentTime = next_event_time;
                continue;
            }
        }
        currentTime++;
    }

    // padrão...

    if (strlen(gantt_chart) > 0 && gantt_chart[strlen(gantt_chart)-1] == '|') {
        gantt_chart[strlen(gantt_chart) - 1] = '\0';
    }
    char final_gantt[GANTT_CHART_BUFFER_SIZE * 2];
    sprintf(final_gantt, "%u[%s]", simulation_start_time, gantt_chart);
    printMetrics("Prioridade Preemptivo", processes, count, currentTime - simulation_start_time, cpu_busy_time, final_gantt, base_filename);

    free(ready_queue);
    free(io_queue);
}

void RoundRobin(Process *processes, unsigned int count, int quantum, const char* base_filename, bool is_io_sequential) {
    resetProcesses(processes, count);
    if (count == 0) return; // sem processos

    char algorithm_name[50];
    snprintf(algorithm_name, sizeof(algorithm_name), "Round-Robin (q=%dms)", quantum);

    unsigned int max_queue_size = count * 5;
    int *ready_queue = malloc(max_queue_size * sizeof(int));
    int ready_tail = 0; // Usaremos apenas a cauda
    int *io_queue = malloc(max_queue_size * sizeof(int));
    int io_head = 0, io_tail = 0;

    int running_process_idx = -1;
    int time_slice_remaining = 0;

    int io_process_idx = -1;
    unsigned int io_device_free_time = 0;

    unsigned int terminated_count = 0;
    unsigned int cpu_busy_time = 0;
    char gantt_chart[GANTT_CHART_BUFFER_SIZE] = "";
    int last_pid_on_cpu = -2;

    unsigned int simulation_start_time = -1;
    for (unsigned int i = 0; i < count; i++) {
        if (simulation_start_time == (unsigned int)-1 || processes[i].arrival_time < simulation_start_time) {
            simulation_start_time = processes[i].arrival_time;
        }
    }
    unsigned int currentTime = simulation_start_time;

    // padrão...
    while (terminated_count < count) {
        for (unsigned int i = 0; i < count; i++) {
            if (processes[i].state == NEW && processes[i].arrival_time <= currentTime) {
                processes[i].state = READY;
                ready_queue[ready_tail++] = i;
            }
        }
        if (is_io_sequential) {
            if (io_process_idx != -1 && currentTime >= io_device_free_time) {
                processes[io_process_idx].state = READY;
                ready_queue[ready_tail++] = io_process_idx;
                io_process_idx = -1;
            }
        } else {
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == IO && processes[i].io_finish_time <= currentTime) {
                    processes[i].state = READY;
                    ready_queue[ready_tail++] = i;
                }
            }
        }

        // verifica se o processo em execução deve sair da CPU
        if (running_process_idx != -1) {
            bool burst_finished = processes[running_process_idx].bursts[processes[running_process_idx].current_burst] == 0;
            bool quantum_expired = time_slice_remaining == 0;

            if (burst_finished) {
                processes[running_process_idx].current_burst++;
                if (processes[running_process_idx].current_burst >= processes[running_process_idx].total_bursts) {
                    processes[running_process_idx].state = TERMINATED;
                    processes[running_process_idx].completion_time = currentTime;
                    processes[running_process_idx].turnaround_time = currentTime - processes[running_process_idx].arrival_time;
                    terminated_count++;
                } else {
                    processes[running_process_idx].state = WAITING_IO;
                    io_queue[io_tail++] = running_process_idx;
                }
                running_process_idx = -1;
            } else if (quantum_expired) {
                processes[running_process_idx].state = READY;
                ready_queue[ready_tail++] = running_process_idx;
                running_process_idx = -1;
            }
        }

        // se a cpu esta livre, escalona o proximo da frente da fila
        if (running_process_idx == -1 && ready_tail > 0) {
            running_process_idx = ready_queue[0];
            processes[running_process_idx].state = EXECUTING;
            time_slice_remaining = quantum;

            // desloca a fila para remover o processo
            for (int i = 0; i < ready_tail - 1; i++) {
                ready_queue[i] = ready_queue[i + 1];
            }
            ready_tail--;
        }

        // inicia E/S
        if (is_io_sequential) {
             if (io_process_idx == -1 && io_head < io_tail) {
                io_process_idx = io_queue[io_head++];
                processes[io_process_idx].state = IO;
                io_device_free_time = currentTime + processes[io_process_idx].bursts[processes[io_process_idx].current_burst];
                processes[io_process_idx].current_burst++;
            }
        } else {
            while (io_head < io_tail) {
                int p_idx = io_queue[io_head++];
                processes[p_idx].state = IO;
                processes[p_idx].io_finish_time = currentTime + processes[p_idx].bursts[processes[p_idx].current_burst];
                processes[p_idx].current_burst++;
            }
        }

        // atualizações e avanço do tempo
        int current_pid_on_cpu = (running_process_idx == -1) ? -1 : processes[running_process_idx].pid;
        if (current_pid_on_cpu != last_pid_on_cpu) {
            if (last_pid_on_cpu != -2) {
                char temp[50];
                if (last_pid_on_cpu >= 0) sprintf(temp, "P%d %u|", last_pid_on_cpu, currentTime);
                else sprintf(temp, "*** %u|", currentTime);
                strncat(gantt_chart, temp, sizeof(gantt_chart) - strlen(gantt_chart) - 1);
            }
            last_pid_on_cpu = current_pid_on_cpu;
        }

        for (int i = 0; i < ready_tail; i++) {
            processes[ready_queue[i]].waiting_time++;
        }

        if (running_process_idx != -1) {
            processes[running_process_idx].bursts[processes[running_process_idx].current_burst]--;
            time_slice_remaining--;
            cpu_busy_time++;
        }

        // salto no tempo se ocioso
        if (running_process_idx == -1 && ready_tail == 0 && io_head == io_tail && terminated_count < count) {
            unsigned int next_event_time = -1;
            for (unsigned int i = 0; i < count; i++) {
                if (processes[i].state == NEW && (next_event_time == -1 || processes[i].arrival_time < next_event_time)) next_event_time = processes[i].arrival_time;
                if (processes[i].state == IO && (next_event_time == -1 || processes[i].io_finish_time < next_event_time)) next_event_time = processes[i].io_finish_time;
            }
            if (is_io_sequential && io_process_idx != -1 && (next_event_time == -1 || io_device_free_time < next_event_time)) next_event_time = io_device_free_time;

            if (next_event_time > currentTime && next_event_time != (unsigned int)-1) {
                currentTime = next_event_time;
                continue;
            }
        }
        currentTime++;
    }

    // padrão...

    if (strlen(gantt_chart) > 0 && gantt_chart[strlen(gantt_chart)-1] == '|') {
        gantt_chart[strlen(gantt_chart) - 1] = '\0';
    }
    char final_gantt[GANTT_CHART_BUFFER_SIZE * 2];
    sprintf(final_gantt, "%u[%s]", simulation_start_time, gantt_chart);
    printMetrics(algorithm_name, processes, count, currentTime - simulation_start_time, cpu_busy_time, final_gantt, base_filename);

    free(ready_queue);
    free(io_queue);
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada> <quantum> [-seq]\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int quantum = atoi(argv[2]);
    bool is_io_sequential = (argc > 3 && strcmp(argv[3], "-seq") == 0);

    unsigned int process_count;
    Process *processes = readInputFile(filename, &process_count);

    if (processes) {
        char out_filename[256];
        char *base_name = strdup(filename);
        char *dot = strrchr(base_name, '.');
        if (dot) *dot = '\0';

        snprintf(out_filename, sizeof(out_filename), "%s.out", base_name);
        FILE *fp = fopen(out_filename, "w");
        if(fp) fclose(fp);

        FCFS(processes, process_count, base_name, is_io_sequential);
        SJF(processes, process_count, base_name, is_io_sequential);
        SRTF(processes, process_count, base_name, is_io_sequential);
        PriorityPreemptive(processes, process_count, base_name, is_io_sequential);
        RoundRobin(processes, process_count, quantum, base_name, is_io_sequential);

        printf("Simulacao concluida. Verifique o arquivo '%s'.\n", out_filename);
        free(base_name);
        freeProcesses(processes, process_count);
    }

    return 0;
}
