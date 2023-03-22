
// void SJF(Queue queue) {
//     int size = queue.size;
//     int time = 0;
//     int total = 0;
//     int *waiting = (int *) malloc(queue.size * sizeof(int));
//     while (queue.size > 0) {
//         int min = 0;
//         for (int i = 0; i < queue.size; i++) {
//             if (queue.processes[i].time < queue.processes[min].time) {
//                 min = i;
//             } else if (queue.processes[i].time == queue.processes[min].time) {
//                 if (queue.processes[i].arrive < queue.processes[min].arrive) {
//                     min = i;
//                 }
//             }
//         }
//         // tìm được min thì đổi chỗ với phần tử cuối cùng
//         Process temp = queue.processes[queue.size - 1];
//         queue.processes[queue.size - 1] = queue.processes[min];
//         queue.processes[min] = temp;

//         printf("%s %d %d\n", queue.processes[min].name, queue.processes[min].time, queue.processes[min].arrive);
//     }
//     // loại bỏ tiến trình vừa in ra khỏi queue
//     queue.size--;
// }