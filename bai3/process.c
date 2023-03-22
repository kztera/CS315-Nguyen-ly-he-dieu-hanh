#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[100];
    int arrive;
    int time;
} Process;

typedef struct {
    Process *processes;
    int size;
} Queue;

void SJF(Queue queue);

int main() {
    // đọc file tientrinh.txt
    FILE *f = fopen("tientrinh.txt", "r");
    Queue queue;
    queue.size = 0;
    queue.processes = (Process *) malloc(sizeof(Process));
    int n;
    fscanf(f, "%d", &n);
    for (int i = 0; i < n; i++) {
        Process p;
        fscanf(f, "%s %d", p.name, &p.time);
        p.arrive = i;
        queue.processes[i] = p;
        queue.size++;
        queue.processes = (Process *) realloc(queue.processes, (queue.size + 1) * sizeof(Process));
    }
    fclose(f);
    SJF(queue);
    return 0;
}

void SJF(Queue queue) {
    // Sắp xếp lại Queue theo thời gian thực thi tăng dần
    for (int i = 0; i < queue.size - 1; i++) {
        for (int j = i + 1; j < queue.size; j++) {
            if (queue.processes[i].time > queue.processes[j].time) {
                Process temp = queue.processes[i];
                queue.processes[i] = queue.processes[j];
                queue.processes[j] = temp;
            }
            // nếu bằng nhau thì so sánh thời gian đến 
            if (queue.processes[i].time == queue.processes[j].time) {
                if (queue.processes[i].arrive > queue.processes[j].arrive) {
                    Process temp = queue.processes[i];
                    queue.processes[i] = queue.processes[j];
                    queue.processes[j] = temp;
                }
            }
        }
    }

    // Tính thời gian chờ trung bình
    int size = queue.size;
    int time = 0;
    int total = 0;

    int *waiting = (int *) malloc(size * sizeof(int));

        for (int i = 0; i < size; i++) {
        waiting[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < i; j++) {
            waiting[i] += queue.processes[j].time;
        }
    }

    for (int i = 0; i < size; i++) {
        total += waiting[i];
    }
    printf("  Tên  |  Thời gian thực thi  |  Thứ tự đến  |  Thời gian chờ  \n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < queue.size; i++) {
            printf("  %s   |          %d           |      %d       |        %d        ", queue.processes[i].name, queue.processes[i].time, queue.processes[i].arrive, waiting[i]);
        printf("\n");
    }
    printf("---------------------------------------------------------------\n");
    printf("Thoi gian cho trung binh: %.2f\n", (float) total / size);
}
