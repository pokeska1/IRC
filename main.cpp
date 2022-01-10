#include <iostream>
#include "Server.hpp"
#include "User.hpp"


//номер порта и константа
#define PORT 5555
#define BUFLEN 512
//  ./ircserv [host:port_network:password_network] <port> <password>

void gatekeeper(int argc, char **argv, Server &serv)
{
    if (argc < 2 || argc > 3 )
    {
        std::cout <<
        "You need to run\nhost:port_network:password_network  or <port> <password>"
        << std::endl;
        exit(1);
    }
    if ( (strstr(argv[1], ":") != NULL) && (argc ==2) )// проверка на хост нейма
    {
        char *mimic = strtok(argv[1], ":");
        std::string *host = new std::string(mimic);
        serv.setHost(*host);
        mimic =  strtok(NULL, ":");
        serv.setPort(atoi(mimic));
        mimic =  strtok(NULL, ":");
        std::string *password = new std::string(mimic);
        serv.setPassword(*password);
    }
    else if (argc == 3){
         serv.setPort(atoi(argv[1]));
         std::string *password = new std::string(argv[2]);
         serv.setPassword(*password);
    }
    else{
        std::cout <<
            "You need to run\nhost:port_network:password_network  or <port> <password>"
            << std::endl;
        exit(1);
    }

}


int main(int argc, char **argv) {
    Server serv;
    gatekeeper(argc, argv, serv);    //проверка запуска

    int err, opt = 1;
    int sock;
    struct sockaddr_in addr;


    //создаем  TCP сокет приема запроссов на соединение
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Server: cannot creat socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int));
    //заполняем адресную структуру
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv.getPort(0));
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // связываем сокет с любым портом
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

    serv.work(sock);

    return 0;
}




//    Client client(sock);
//    serv.setClient(client);

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