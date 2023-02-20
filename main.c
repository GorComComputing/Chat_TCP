#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <winsock2.h>

// чат
void chat(SOCKET s){
    char mes[200];
    WSAEventSelect(s, 0, FD_READ);  // Получает сообщение без таймаута
    do{
        if((GetKeyState(VK_F1) < 0) && (GetForegroundWindow() == GetConsoleWindow())){ // печатаем сообщение только при F1 у окна в фокусе
            printf("message: ");
            while(getch() != 0); // убирает лишние семволы в начале
            scanf("\n%200[0-9a-zA-Z.,! ]", mes);
            send(s, mes, sizeof(mes), 0); // Отправляем сообщение
        }

        if(recv(s, mes, sizeof(mes), 0) > 0)
            printf("%s\n", mes);
    }while(GetKeyState(VK_ESCAPE) >= 0);  // выход по ESC
}

int main()
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws); // Инициализируем сокеты

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0); // Создаем сокет TCP

    SOCKADDR_IN sa; // Структура с настройками соединения
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(1234);

    char c;
    printf("s - server\nk - client\n");
    scanf("%c", &c);

    if(c == 'k'){
        sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // Адрес сервера
        connect(s, &sa, sizeof(sa));    // Устанавливаем соединение с сервером

        chat(s);
    }

    if(c == 's'){
        bind(s, &sa, sizeof(sa));  // Устанавливаем настройки соединения на соккет
        listen(s, 100); // Слушаем очередь

        char buf[200];     // буфер для приема данных
        memset(buf, 0, sizeof(buf));

        SOCKET client_socket;
        SOCKADDR_IN client_addr;
        int client_addr_size = sizeof(client_addr);

        // Ждем входящего соединения
        while(client_socket = accept(s, &client_addr, &client_addr_size)){
            printf("connect OK\n");

            chat(client_socket);
        }
    }

    closesocket(s); // Закрываем сокет

    return 0;
}
