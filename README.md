<h1>Nguyên lý hệ điều hành - Cuối kỳ </h1>

<h2> Table of Contents </h2>

- [Bài 1:](#bài-1)
  - [Code](#code)
  - [Chạy thử](#chạy-thử)
- [Bài 2: Socket Programming](#bài-2-socket-programming)
  - [Mô tả:](#mô-tả)
  - [Code](#code-1)
    - [2.1. Server](#21-server)
    - [2.2. Client](#22-client)
    - [2.3. Cho phép nhiều client có thể cùng giao tiếp với Server](#23-cho-phép-nhiều-client-có-thể-cùng-giao-tiếp-với-server)
  - [Giải thích](#giải-thích)
    - [2.1. Server](#21-server-1)
      - [2.1.1 Khai báo thư viện, định nghĩa cổng mà server sẽ lắng nghe kết nối](#211-khai-báo-thư-viện-định-nghĩa-cổng-mà-server-sẽ-lắng-nghe-kết-nối)
      - [2.1.2. Tạo socket server](#212-tạo-socket-server)
      - [2.1.3. Gán địa chỉ cho socket](#213-gán-địa-chỉ-cho-socket)
      - [2.1.4. Liên kết socket với địa chỉ](#214-liên-kết-socket-với-địa-chỉ)
        - [2.1.5. Lắng nghe kết nối](#215-lắng-nghe-kết-nối)
      - [2.1.6. Chấp nhận kết nối](#216-chấp-nhận-kết-nối)
      - [2.1.7. Đọc và gửi dữ liệu](#217-đọc-và-gửi-dữ-liệu)
      - [2.1.8. Đóng kết nối](#218-đóng-kết-nối)
    - [2.2. Chương trình client](#22-chương-trình-client)

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

## Bài 2: Socket Programming

### Mô tả:
1. Viết ứng dụng Socket phía Server
2. Viết ứng dụng Socket phía Client
3. Cho phép nhiều client có thể cùng giao tiếp với Server

### Code

#### 2.1. Server

```c
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
	
    address.sin_family = AF_INET; //IPv4
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
	{
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) 
	{
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) 
	{
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
  
    //Closing the connected socket
    close(new_socket);
    //Closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
```

#### 2.2. Client

```c
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
  
int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        printf("\n Socket creation error \n");
        return -1;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary form
	// This process is ran in local with IP: 127.0.0.1
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0) 
	{
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))< 0) 
	{
        printf("\nConnection Failed \n");
        return -1;
    }
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer, 1024);
    printf("%s\n", buffer);
  
    //Closing the connected socket
    close(client_fd);
    return 0;
}
```

#### 2.3. Cho phép nhiều client có thể cùng giao tiếp với Server

// TODO: Chưa làm được

### Giải thích

#### 2.1. Server

Chương trình này tạo một `socket server` và lắng nghe kết nối đến cổng 8080. Khi có một kết nối đến, chương trình sẽ nhận dữ liệu từ client và gửi lại thông điệp “Hello from server”

##### 2.1.1 Khai báo thư viện, định nghĩa cổng mà server sẽ lắng nghe kết nối

```c
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
```

##### 2.1.2. Tạo socket server

```c
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
```

Tiếp theo, chương trình tạo một `socket file descriptor` bằng cách gọi hàm `socket()` với các tham số `AF_INET` (sử dụng giao thức IPv4), SOCK_STREAM (sử dụng giao thức TCP) và 0 (sử dụng giao thức mặc định). Nếu hàm socket() trả về giá trị nhỏ hơn 0, chương trình sẽ in ra lỗi “socket failed” và thoát.

**Thuật ngữ:**

- `socket file descriptor` là một số nguyên duy nhất được sử dụng để tham chiếu đến một socket. Nó được sử dụng trong các hàm sau khi socket được tạo. 

- `socket` là một cổng kết nối giữa hai máy tính. Nó có thể được sử dụng để gửi và nhận dữ liệu giữa hai máy tính.

- `AF_INET` là một định danh cho giao thức IPv4. Nó được sử dụng trong hàm `socket()` để chỉ ra giao thức mạng nào sẽ được sử dụng.

- `SOCK_STREAM` là một định danh cho giao thức TCP. Nó được sử dụng trong hàm `socket()` để chỉ ra giao thức truyền dữ liệu nào sẽ được sử dụng.

Khi giá trị trả về của hàm `socket()` là lớn hơn hoặc bằng 0, chương trình sẽ gán giá trị này cho biến `server_fd` để sử dụng trong các hàm sau. Hay nói cách khác đó là tín hiệu cho chương trình biết rằng socket đã được tạo thành công.

##### 2.1.3. Gán địa chỉ cho socket

```c
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET; //IPv4
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
```

Sau đó, chương trình gọi hàm `setsockopt()` để thiết lập tùy chọn `SO_REUSEADDR` và `SO_REUSEPORT` cho socket vừa tạo. Nếu hàm `setsockopt()` trả về giá trị khác 0, chương trình sẽ in ra lỗi “setsockopt” và thoát. Trong dó:

- `SO_REUSEADDR` cho phép nhiều socket có thể liên kết với cùng một địa chỉ IP và cổng (port) nếu mỗi socket đều thiết lập tùy chọn này. Tùy chọn này hữu ích khi bạn muốn khởi động lại một server ngay lập tức mà không phải đợi hệ thống giải phóng địa chỉ IP và cổng trước khi sử dụng lại.

- `SO_REUSEPORT` cho phép nhiều socket có thể liên kết với cùng một địa chỉ IP và cổng (port) nếu mỗi socket đều thiết lập tùy chọn này. Tuy nhiên, khác với `SO_REUSEADDR`, tùy chọn này cho phép các ứng dụng **khác nhau chia sẻ cùng một địa chỉ IP và cổng**. Tùy chọn này hữu ích khi bạn muốn có nhiều tiến trình hoặc luồng xử lý kết nối đến cùng một địa chỉ IP và cổng.

Tiếp theo, chương trình thiết lập địa chỉ cho socket bằng cách khởi tạo cấu trúc `sockaddr_in` với các giá trị: 
- `sin_family = AF_INET` (sử dụng giao thức IPv4)
- `sin_addr.s_addr = INADDR_ANY` (cho phép kết nối từ bất kỳ địa chỉ IP nào) 
- `sin_port = htons(PORT)` (chuyển đổi số hiệu cổng sang network byte order). Việc chuyển đổi số hiệu cổng sang network byte order là bắt buộc vì cổng được lưu trữ trong cấu trúc `sockaddr_in` là kiểu `unsigned short` và có thể lưu trữ giá trị từ 0 đến 65535. Nếu không chuyển đổi sang network byte order, giá trị cổng sẽ không đúng.

Đây là bước quan trọng nhất trong quá trình tạo socket server. Nếu chương trình không thiết lập địa chỉ cho socket, socket sẽ không thể nhận được kết nối từ client. 

##### 2.1.4. Liên kết socket với địa chỉ

```c
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
```

Sau khi thiết lập địa chỉ cho socket, chương trình gọi hàm `bind()` để liên kết socket với địa chỉ. Nếu hàm `bind()` trả về giá trị nhỏ hơn 0, chương trình sẽ in ra lỗi “bind failed” và thoát.

###### 2.1.5. Lắng nghe kết nối

```c
    if (listen(server_fd, 3) < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
```

Sau khi liên kết thành công socket với địa chỉ, chương trình gọi hàm `listen()` để cho phép socket lắng nghe kết nối từ client với số lượng kết nối tối đa là 3. Nếu hàm `listen()` trả về giá triệu nhỏ hơn 0, chương trình sẽ in ra lỗi “listen” và thoát.

##### 2.1.6. Chấp nhận kết nối

```c
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
```

Sử dụng hàm `accept()` để chấp nhận kết nối từ client. Nếu hàm `accept()` trả về giá trị nhỏ hơn 0, chương trình sẽ in ra lỗi “accept” và thoát. 
Trong đó:

- `server_fd` là socket được tạo trong bước trước.
- `address` là cấu trúc `sockaddr_in` chứa địa chỉ của socket.
- `addrlen` là độ dài của cấu trúc `sockaddr_in`.

Với các thông số trên, hàm `accept()` sẽ chờ đợi cho đến khi có một kết nối từ client. Khi có kết nối từ client, hàm `accept()` sẽ trả về một socket mới được gán cho biến `new_socket`. Socket này sẽ được sử dụng để giao tiếp với client.

##### 2.1.7. Đọc và gửi dữ liệu

```c
    valread = read(new_socket, buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
```

Sau khi chấp nhận kết nối từ client, chương trình sẽ đọc dữ liệu gửi từ client bằng hàm `read()` và gửi dữ liệu đến client bằng hàm `send()`.
Trong đó:
- `buffer` là mảng chứa dữ liệu đọc được từ client.
- `1024` là kích thước của mảng `buffer`.

Sau khi đọc dữ liệu từ client, chương trình sẽ in ra dữ liệu đó và gửi dữ liệu “Hello from server” đến client. Gửi thành công, chương trình sẽ in ra “Hello message sent” ở phía server thông báo gửi thành công.

##### 2.1.8. Đóng kết nối

```c
    close(new_socket);
    shutdown(server_fd, SHUT_RDWR);
```

Cuối cùng, chương trình đóng socket kết nối với client bằng cách gọi hàm `close(new_socket)` và đóng socket lắng nghe bằng cách gọi hàm `shutdown(server_fd, SHUT_RDWR)`.

#### 2.2. Chương trình client