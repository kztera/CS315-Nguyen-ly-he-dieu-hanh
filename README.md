<h1>Nguyên lý hệ điều hành - Cuối kỳ </h1>

<h2> Table of Contents </h2>

- [Bài 1:](#bài-1)
  - [Code](#code)
  - [Chạy thử](#chạy-thử)

## Bài 1:

Xét 2 ma trận A và B trong đó A là ma trận M hàng và K cột; và B là ma trận K hàng và N cột. Tích ma trận A và B là ma trận C có M hàng và N cột. Các phần tử của C được tính như sau:

$C_{ij} = \sum_{n=1}^{K} A_{i,n} * B_{n,j}$

Ví dụ, nếu A có 3 hàng 2 cột và B có 2 hàng và 3 cột, thì phần tử $C_{3,1}$ được tính là $A_{3,1} * B_{1,1} + A_{3,2} * B_{2,1}$

**Yêu cầu:**

-  Thực hiện code bằng code C
-  Phải tạo ra M \* N tiến trình con để tính các phần tử của C
-  Các ma trận A, B phải được đọc từ file data.dat trong đó các số nguyên dương M, K và N được ghi ở dòng đầu tiên; M dòng sau của ma trận A; K dòng sau của ma trận B.
-  Tiến trình cha sẽ tạo ra M \* N tến trình con để tính toán, các tiến trình này nhận số đầu vào là dòng i cột j, tính toán phần tử $C_{ij}$ của ma trận kết quả.
-  Khi tất cả các tiến trình con đã tính toán xong, tuyến cha sẽ in ra màn hình ma trận C. Để làm điều này, tiến trình cha phải đợi tất cả các tiến trình con hoàn thành việc tính toán trước khi in ra màn hình.

### Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int M, K, N;
int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

void *multiply(void *arg) {
    int *data = (int *)arg;
    int row = data[0];
    int col = data[1];
    int sum = 0;

    for (int i = 0; i < K; i++) {
        sum += A[row][i] * B[i][col];
    }

    C[row][col] = sum;
    pthread_exit(NULL);
}

int main() {
    FILE *fp;
    fp = fopen("data.dat", "r");

    if (fp == NULL) {
        printf("Khong the mo file\n");
        exit(1);
    }

    fscanf(fp, "%d,%d,%d;", &M, &K, &N);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            fscanf(fp, "%d,", &A[i][j]);
        }
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(fp, "%d,", &B[i][j]);
        }
    }

    fclose(fp);

    pthread_t tid[MAX][MAX];
    int thread_data[MAX][MAX][2];

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            thread_data[i][j][0] = i;
            thread_data[i][j][1] = j;
            pthread_create(&tid[i][j], NULL, multiply, (void *)thread_data[i][j]);
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(tid[i][j], NULL);
        }
    }

    printf("Ma tran C:\n");

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }

        printf("\n");
    }
    return 0;
}
```
**1. Khai báo các biến và hàm tính phần tử của ma trận C**

```c
    int M, K, N;
    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
```

Hàm `multiply` nhận vào 1 con trỏ `arg` là 1 mảng 2 chiều, mỗi phần tử là 1 mảng 1 chiều có 2 phần tử là dòng và cột của ma trận C. Hàm này sẽ tính phần tử $C_{ij}$ của ma trận C.

```c
    void *multiply(void *arg) {
        int *data = (int *)arg; // khởi tạo con trỏ data trỏ đến con trỏ arg có tái khai báo kiểu int
        int row = data[0];
        int col = data[1];
        int sum = 0;

        for (int i = 0; i < K; i++) {
            sum += A[row][i] * B[i][col];
        }

        C[row][col] = sum;
        pthread_exit(NULL); // kết thúc tiến trình con
    }
```

**2. Đọc dữ liệu từ file data.dat, lấy giá trị của M, K, N**

```c
    FILE *fp;
    fp = fopen("data.dat", "r");

    if (fp == NULL) {
        printf("Khong the mo file\n");
        exit(1);
    }

    fscanf(fp, "%d,%d,%d;", &M, &K, &N);
```
**3. Tạo ma trận A, B**

```c
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            fscanf(fp, "%d,", &A[i][j]);
        }
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(fp, "%d,", &B[i][j]);
        }
    }

    fclose(fp);
```

**5. Tạo M * N tiến trình con để tính toán**

```c
    pthread_t tid[MAX][MAX];  
    int thread_data[MAX][MAX][2];

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            thread_data[i][j][0] = i;
            thread_data[i][j][1] = j;
            pthread_create(&tid[i][j], NULL, multiply, (void *)thread_data[i][j]);
        }
    }
```

Trong đó,

```c
    pthread_t tid[MAX][MAX];  
    int thread_data[MAX][MAX][2];
```

- tid là mảng 2 chiều chứa các thread id
- thread_data là mảng 3 chiều chứa dữ liệu của các thread
- Việc khởi tạo mảng `tid` và mảng `thread_data` là để sau này khi gọi hàm `pthread_join` thì có thể truyền vào đúng thread id và dữ liệu của thread đó. (`thread_join` dùng để truyền dữ liệu từ các thread con về thread cha). Nếu không khởi tạo mảng `tid` và `thread_data` thì khi gọi hàm `pthread_join` sẽ không biết truyền vào đúng thread id và dữ liệu của thread đó.

```c
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            thread_data[i][j][0] = i;
            thread_data[i][j][1] = j;
            pthread_create(&tid[i][j], NULL, multiply, (void *)thread_data[i][j]);
        }
    }
```

- Sử dụng 2 vòng lặp lồng nhau để tạo M * N tiến trình con bằng hàm `pthread_create`. Trong đó, mỗi tiến trình con sẽ tính toán phần tử thứ i, j của ma trận C.
- `thread_data[i][j][0] = i;` lưu trữ dòng của phần tử thứ i, j của ma trận C. 
- `thread_data[i][j][1] = j;` lưu trữ cột của phần tử thứ i, j của ma trận C.
- Tạo tiến trình con với hàm `pthread_create(&tid[i][j], NULL, multiply, (void *)thread_data[i][j]);`. Trong đó, `tid[i][j]` là thread id của tiến trình con, `multiply` là hàm tính phần tử của ma trận C, `(void *)thread_data[i][j]` là con trỏ trỏ đến dữ liệu của tiến trình con được ép kiểu về kiểu `void *` để có thể truyền vào hàm `multiply`.

**6. Gọi hàm `pthread_join` để chờ các tiến trình con kết thúc**

```c
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(tid[i][j], NULL);
        }
    }
```

- Sử dụng 2 vòng lặp lồng nhau, gọi hàm `pthread_join` để chờ tất cả các tiến trình con kết thúc.
- Nói rõ hơn về hàm `pthread_join`:

```c
    int pthread_join(pthread_t thread, void **retval);
```

- Hàm `pthread_join` là một hàm trong C/C++ được sử dụng để đợi cho việc kết thúc của một luồng . Sau khi luồng kết thúc, ứng dụng có thể lựa chọn để dọn dẹp các tài nguyên đã được sử dụng bởi luồng. Ví dụ, sau khi `pthread_join` trả về, bất kỳ bộ nhớ ngăn xếp được cung cấp bởi ứng dụng có thể được thu hồi.

**7. In ra màn hình ma trận C**

```c
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
```

- Sau khi `pthread_join` trả về, các phần tử của ma trận C đã được tính toán xong. In ra màn hình ma trận C.

**8. Kết thúc tiến trình cha**

```c
    pthread_exit(NULL);
```

### Chạy thử

Ta có file data.dat như sau:

```c
    3,2,3
    2,3
    1,3
    4,7
    -2,4,5
    1,8,3
```

Dòng đầu tiên là 3, 2, 3. Tức, A là ma trận 3x2, B là ma trận 2x3.

Output:

```
    Ma tran C:
    -1 32 19
    1 28 14
    -1 72 41
```