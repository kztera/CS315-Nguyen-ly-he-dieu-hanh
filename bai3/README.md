## Bài 3: Lập lịch tiến trình

### Đề bài:

Cho dãy tiến trình p1, p2, ... pn trong file `tientrinh.txt` trong đó có dòng đầu tiên ghi số tiến trình, các dòng tiếp theo mỗi dòng ghi tên tiến trình và thời gian thực thi của tiến trình theo thứ tự đến của tiến trình trong hàng đợi.
**Yêu cầu:** Sử dụng thuật toán SJF có ưu tiên đưa ra danh sách thứ tự thực hiện của các tiến trình và thời gian chờ trung bình (average waiting time) của các tiến trình.

### Bài làm: Thực hiện bằng code C trên Ubuntu

#### 1. Thuật toán SJF có ưu tiên (Shortest Job First with Priority)

Trước hết, thuật toán này là một thuật toán lập lịch tiến trình (scheduling algorithm) được sử dụng để lập lịch các tiến trình trong hệ điều hành. Thuật toán này sẽ lựa chọn tiến trình có thời gian thực thi ngắn nhất để thực thi trước. Nếu có nhiều tiến trình có thời gian thực thi bằng nhau, thì tiến trình nào đến trước sẽ được thực thi trước.

Như vậy, thuật toán SJF có ưu tiên sẽ lựa chọn tiến trình có thời gian thực thi ngắn nhất và đến trước để thực thi trước. Với đầu vào là một danh sách các tiến trình đang chờ thực thi, thuật toán sẽ lặp lại các bước sau:

-  Tìm tiến trình có thời gian thực thi ngắn nhất và đến trước trong danh sách các tiến trình chưa được thực thi.
-  Thực thi tiến trình này.
-  Xóa tiến trình này khỏi danh sách các tiến trình chưa được thực thi và thêm tiến trình này vào danh sách các tiến trình đã được thực thi.

#### 2. Cài đặt

Trước hết, ta có file `tientrinh.txt` với nội dung như sau:

-  Dòng đầu ghi số tiến trình
-  Các dòng tiếp theo mỗi dòng ghi tên tiến trình và thời gian thực thi của tiến trình theo thứ tự đến của tiến trình trong hàng đợi.

Thời gian thực hiện của mỗi tiến trình phụ thuộc vào nhiều yếu tố khác nhau như loại tiến trình, tài nguyên máy tính có sẵn và các yêu cầu xử lý khác. Thời gian thực hiện của một tiến trình thường được tính bằng đơn vị thời gian như giây hoặc mili-giây. Trong ví dụ này, ta sẽ giả sử thời gian thực hiện của mỗi tiến trình là một số nguyên.

```txt
5
p1 3
p2 2
p3 1
p4 4
p5 5
```

#### 3. Giải thuật

Chuyển file `tientrinh.txt` thành bảng như sau

| Tiến trình | Thời gian thực thi |
| :--------: | :----------------: |
|     p1     |         9          |
|     p2     |         2          |
|     p3     |         1          |
|     p4     |         4          |
|     p5     |         6          |

Sử dụng thuật toán SJF ta sắp xếp lại thứ tự đến CPU như sau:

$$CPU \leftarrow p3 \leftarrow p2 \leftarrow p4 \leftarrow p1 \leftarrow p5$$

Bảng sau khi đã sắp xếp lại:

| Tiến trình | Thời gian thực thi |
| :--------: | :----------------: |
|     p3     |         1          |
|     p2     |         2          |
|     p4     |         4          |
|     p5     |         6          |
|     p1     |         9          |

Ta có bảng thời gian chờ của các tiến trình

|        | p3  | p2  | p4  | p5  | p1  |
| :----: | :-: | :-: | :-: | :-: | :-: |
| **p3** |  -  |  1  |  1  |  1  |  1  |
| **p2** |  -  |  -  |  2  |  2  |  2  |
| **p4** |  -  |  -  |  -  |  4  |  4  |
| **p5** |  -  |  -  |  -  |  -  |  6  |
| **p1** |  -  |  -  |  -  |  -  |  -  |
|Thời gian chờ|-|1|3|7|13|

Thời gian chờ trung bình của các tiến trình là: $$\frac{1+3+7+13}{5} = 4.8$$

Trong khi dó, nếu không sử dụng thuật toán thì thời gian chờ của các tiến trình sẽ như sau:

|       | p1  | p2  | p3  | p4  | p5  |
| :---: | :-: | :-: | :-: | :-: | :-: |
| **p1** |  -  |  9  |  9  |  9  |  9  |
| **p2** |  -  |  -  |  2  |  2  |  2  |
| **p3** |  -  |  -  |  -  |  1  |  1  |
| **p4** |  -  |  -  |  -  |  -  |  4  |
| **p5** |  -  |  -  |  -  |  -  |  -  |
|Thời gian chờ|-|9|11|12|16|

Thời gian chờ trung bình của các tiến trình là: $$\frac{9+11+12+16}{5} = 9.6$$
#### 4. Code

Để thực hiện bài toán, ta sẽ tạo một struct `Process` để lưu trữ thông tin của một tiến trình. Struct này có 3 trường: tên tiến trình, thời gian thực thi và thời gian đến của tiến trình.

```c
typedef struct {
    char name[10];
    int time;
    int arrive;
} Process;
```

Tiếp theo, ta sẽ tạo một struct `Queue` để lưu trữ danh sách các tiến trình. Struct này có 2 trường: mảng các tiến trình và số tiến trình trong danh sách.

```c
typedef struct {
    Process *processes;
    int size;
} Queue;
```

Để đọc dữ liệu từ file `tientrinh.txt` và lưu vào danh sách các tiến trình, ta sẽ viết hàm `readProcess` như sau:

```c
Queue readProcess(char *filename) {
    FILE *f = fopen(filename, "r");
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
    return queue;
}
```

-  `queue.processes = (Process *) malloc(sizeof(Process));` là dòng khởi tạo mảng `queue.processes` với kích thước 1.

-  `queue.processes = (Process *) realloc(queue.processes, (queue.size + 1) * sizeof(Process));` là dòng cấp phát thêm bộ nhớ cho mảng `queue.processes` khi cần thiết.

Ở đây, sử dụng vòng lặp `for` chạy duyệt qua các tiến trình trong file `tientrinh.txt` và lưu vào danh sách các tiến trình. Mỗi lần như thế, ta sẽ tạo một `Process` mới và lưu vào mảng `queue.processes`. Sau đó, ta sẽ tăng `queue.size` lên 1 và cấp phát thêm bộ nhớ cho mảng `queue.processes` với kích thước `queue.size + 1`.

Dựa vào phần giải thuật cùng yêu cầu đề bài, ta xây dựng hàm `SJF` theo các bước sau:

1. Sắp xếp lại các tiến trình trong Queue theo thứ tự thời gian thực thi tăng dần. Nếu cùng thời gian thì xét về thời gian đến (arrive).
2. Sau khi sắp xếp lại Queue thì in ra theo thứ tự đã sắp xếp.
3. Tính thời gian chờ trung bình của các tiến trình trong Queue. Thời gian chờ của tiến trình thứ i là tổng thời gian thực thi của các tiến trình trước nó.

```c
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

-  Hàm được truyền vào là danh sách các tiến trình đã được thêm vào `Queue`. (Hàng đợi các tiến trình chưa được thực thi)
-  Mảng `waiting` được cấp phát động với kích thước bằng với số tiến trình trong hàng đợi và dùng để lưu trữ thời gian chờ của từng tiến trình.
- Dùng vòng lặp `for` để duyệt qua các tiến trình trong hàng đợi và tính thời gian chờ của từng tiến trình. Thời gian chờ của tiến trình thứ i là tổng thời gian thực thi của các tiến trình trước nó.

#### 3. Chạy chương trình

Ta tổng hợp lại và có được chương trình như sau:

```c
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
```