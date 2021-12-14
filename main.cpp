#include <iostream>
#include "Server.hpp"
#include "Client.hpp"


//номер порта и константа
#define PORT 5555


//тестовые функции
int readFromClient(int fd, char *buf);
void writeToClient(int fd, char *buf);


int main() {


    struct sockaddr_in client;
    char buf[BUFLEN];
    int i , err, opt = 1;
    int sock, new_sock;
   fd_set activ_set, read_set;
    struct sockaddr_in addr;

    socklen_t size;

    //создаем  TCP сокет приема запроссов на соединение
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Server: cannot creat socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int));
    //заполняем адресную структуру
    // связываем сокет с любым портом
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(err < 0){
        perror("Server: cannot creat socket");
        exit(EXIT_FAILURE);
    }

    //Создание очереди на 3 входящих запроса.
    err = listen(sock, 3);
    if (err < 0)
    {
        perror("Server: listen queue failure");
        exit(EXIT_FAILURE);
    }

//    Client client(sock);
    Server serv;
//    serv.setClient(client);
    serv.work(sock);
    /*
     * подготавливаем множество дискрипторов каналов ввода-вывода
     * Для простоты не вычисляем макс значение дискриптора
     * а далее будем вычислять все дискрипторы вплоть до максималь ного
     * возможного значения FD_SETSIZE
     */
    /*
    FD_ZERO(&activ_set);
    FD_SET(sock, &activ_set);

    // бескконечный цикл проверки состояния сокетов
    for(;;) {
        // указаваем максимальный номер дискриптора среди всех что подлежат проверке
        int max_d = sock;


        //проверим появились ли данные в каком либо сокете .
        //В нашем варианте ждем до фактического появления данных
        read_set = activ_set;
        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
            perror("Server: select failure");
            exit(EXIT_FAILURE);
        }


        //данные появились. Проверим в каком сокете
        for (i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &read_set)) {
                if (i == sock) {
                    //пришел запос на новое соединение
                    size = sizeof(client);
                    new_sock = accept(sock, (struct sockaddr *) &client, &size);
                    if (new_sock < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stdout, "Server: connect from host %s, port %hu.\n",
                            inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                    FD_SET(new_sock, &activ_set);
                } else {
                    //пришли данные уже в существующем соединении
                    err = readFromClient(i,buf);
                    if(err < 0){
                        // ошибка или конец данных
                        close(i);
                        FD_CLR(i, &activ_set);
                    }else {
                        // стоп слово
                        if (strstr(buf,"stop")){
                            close(i);
                            FD_CLR(i, &activ_set);
                        }else {
                            //все прочиталось нормально
                            writeToClient(i, buf);
                        }

                    }
                }
            }
        }
*/
//    Server Serv;
//
//    }
    return 0;
}


int readFromClient(int fd, char *buf)
{
    int nbytes;

    nbytes = read(fd, buf, BUFLEN);
    if (nbytes < 0)
    {
        return (-1);
    }
    else if (nbytes == 0){
        return(-1);
    }
    else{
        fprintf(stdout, "Serv got massage: %s/n", buf);
        return(0);
    }
}

void writeToClient(int fd, char *buf)
{
    int nbytes;
    unsigned char *s;

    for (s = (unsigned char *)buf; *s ;s++) *s = toupper(*s);
    nbytes = write(fd, buf, strlen(buf) + 1);
    fprintf(stdout, "Write back: %s/nbytes=%d/n", buf, nbytes);

    if(nbytes < 0){
        perror("Server: write failure");
    }
}
