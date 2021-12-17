//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Server.hpp"

void Server::setHost(std::string &host)
{
    this->host = host;
}

void Server::setPassword(std::string &password)
{
    std::vector<std::string *>::iterator it_begin = arr_password.begin();
    std::vector<std::string *>::iterator it_end = arr_password.end();
    while (it_begin != it_end)
    {
        it_begin++;
    }
    arr_password.push_back(&password);

}

std::string const &Server::getPassword(int i)
{
    int j = 0;

    std::vector<std::string *>::iterator it_begin = arr_password.begin();
    std::vector<std::string *>::iterator it_end = arr_password.end();
    while (it_begin != it_end && j != i)
    {
        it_begin++;
        j++;
    }
    std::string *mimic = (*it_begin);
    return(*mimic);
}

void Server::setPort(int port)
{
    std::vector<int>::iterator it_begin = arr_port.begin();
    std::vector<int>::iterator it_end = arr_port.end();
    while (it_begin != it_end)
    {
        it_begin++;
    }
    arr_port.push_back(port);

}


void Server::deleteClient(int fd)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    while (it_begin != it_end)
    {
        if (fd == (*it_begin)->getFd()) {
            arr_client.erase(it_begin);
            return;
        }
        it_begin++;
    }
}

void Server::setClient(Client &client)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    while (it_begin != it_end)
    {
        it_begin++;
    }
    arr_client.push_back(&client);
}


void Server::setAccess(int fd)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    int j;

    while (it_begin != it_end )
    {
        if (fd == (*it_begin)->getFd())
           (*it_begin)->setAccess(true);
        it_begin++;
    }
}

bool Server::getAccess(int fd)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    int j;

    while (it_begin != it_end )
    {
        if (fd == (*it_begin)->getFd())
            return((*it_begin)->getAccess());
        it_begin++;
    }
    return(false);
}


int Server::password_verification(std::string &buf, int fd)
{
    if (buf != this->getPassword(0) ) {
        perror("Wrong password");
        return (-1);
    }
    else
    {
        this->setAccess(fd);
    }

    return (0);
    //PRIVMSG приватное сообщение
    //  Parameters: <receiver>{,<receiver>} <text to be sent>
}

// здесь мы создаем "список" fd которые будет проверять на активность
void Server::create_many_active_fd(int &fd, fd_set &activfds, int &max_d)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();

    if (it_begin != it_end)  // идем по массиву указателей клиентов с помощью итераторов
    {
        fd = (*it_begin)->getFd();  // вытаскием по адресу определенного клиента его фд
        while (it_begin != it_end)
        {
            fd = (*it_begin)->getFd();
            FD_SET(fd, &activfds); // вносим этот фд в множество активных (для этого прогона фд-шников)
            if (fd > max_d)
                max_d = fd;

            it_begin++;
            fcntl(fd, F_SETFL, O_NONBLOCK); // делаем их не блокируемымы , иначе все встанет на read/reav или на write
        }
    }
}


//при появлении нового клиента создаем его объект и запоминаем его адрес, а после закидываем его фд в список активистов.
void Server::get_new_client(int &ls, int &fd, fd_set &activfds)
{
    socklen_t size;
    struct sockaddr_in clientfd;

    if (FD_ISSET(ls, &activfds)) { //  если ls - прослушивающий порт поймал сигал, то это новенький клиент
        size = sizeof(clientfd);
        fd = accept(ls, (struct sockaddr *) &clientfd, &size);// оформляем его
        if (fd < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::cout << "\"Server: connect from host " << inet_ntoa(clientfd.sin_addr)
                  << ", port " << ntohs(clientfd.sin_port) << "." << std::endl;
        FD_SET(fd, &activfds);// добавляем новичка к активным портам которые мы будем слушать
        Client *new_client = new Client(fd); // создаем для этого клиента объект  класса со своим блэк-джеком и fd
        this->setClient(*new_client); // запоминаем его адрес в массив указателей
    }
}


// ловим что написал старый клиент , проверяем на доступ и распределяем, что с ним делать.
void Server::get_old_client_massage(int &i, fd_set &activfds, fd_set &writefds, char **buf)
{
    if (FD_ISSET(i, &activfds))//если фд есть в списке акитвистов то начнем с ним работать
    {
        int nbytes;

        nbytes = recv(i, *buf, 512, 0); // прочтем в массив чаров его сообщение (не обробатывал переполнение буфера)
        if (nbytes < 0){ perror("Server: meh nbytes < 0");}
        else if (nbytes == 0){ perror("Server: meh nbytes == 0");}
        else
        {
            std::cout << "Serv got massage: " << *buf << std::endl; // для удобства пусть сервер отобразит что поймал
            if(getAccess(i))// если у клиента есть дотуп , то вносим его в список "дозволяющие писать"
            {
                FD_SET(i, &writefds);
            }
            else // иначе проверяем что он там наколякал
            {
                std::string buf_str = *buf;
                if (password_verification(buf_str, i) == -1) //и если он ошибся хоть раз сжигаем все его данные =^_^=
                {
                    FD_CLR(i, &writefds);
                    FD_CLR(i, &activfds);
                    deleteClient(i);
                }
            }
        }
    }
}

//
void Server::write_massage_to_client(int &fd, fd_set &writefds, char **buf)
{
    std::vector<Client *>::iterator it_begin_new = arr_client.begin();
    std::vector<Client *>::iterator it_end_new = arr_client.end();

    while (it_begin_new != it_end_new) // идем по всему списку клиентов
    {
        fd = (*it_begin_new)->getFd();// достаем фд нужного клиента по адресу его объекта
        if (getAccess(fd))  // проверяем на наличие допуска
        {
            if (!FD_ISSET(fd, &writefds)) // мы будем отправлять сообщение всем , кроме того кто написал.
            {
                int nbytes;
                int offical;

                if (*buf[0] != '\0' || *buf[0] != '\n') // не уверен что это нужно , не дает спамить
                {
                    offical = write(fd, "Server say:", 11 + 1); // отправляем сообщеньку по фд
//                    std::cout << "Write back: " << "Server: say" << std::endl << "bytes=" << offical << std::endl;
                    nbytes = write(fd, *buf, strlen(*buf) + 1);
                    std::cout << "Write back: " << *buf << "bytes=" << nbytes << std::endl;
                    if (offical < 0)
                    {
                        perror("Server: write failure");
                    }
                }
            }
        }
        it_begin_new++; // двигаемся далее по списку клиентов
    }
}

void Server::work(int ls) {
    fd_set writefds, activfds;

    int max_d = ls; // считаем что макс сокет это нынешний слушающий

    for (;;) {
        int fd;
        char *buf = (char *)calloc(sizeof(char), BUFLEN + 1);

        FD_ZERO(&writefds);
        FD_ZERO(&activfds);
        FD_SET(ls, &activfds);//вводим fd слушашего сокета в множество
        // а теперь цикл по сокетам заносим все fd из списка клиентов в множество;
        create_many_active_fd(fd, activfds, max_d);
        int res = select(FD_SETSIZE, &activfds, NULL, NULL, NULL);// проверяем на активность
        if (res < 1)
        {
            if (errno != EINTR)
            {
                perror("Server: cannot creat socket");
                exit(EXIT_FAILURE);
            }
        }
        for (int i = 0; i <= max_d; i++)
        { //поочереди проверим все айди
            if (i == ls)
            {   // если сигнал пришел со слышашего порта то это новое подключение!
                get_new_client(ls, fd, activfds);
            }
            else
            {  // если с любого другого , то это уже старечки что то пишут и нужно посмотреть что там
                get_old_client_massage(i, activfds, writefds, &buf);
            }
        }
        write_massage_to_client(fd, writefds, &buf);  // здесь отправляем сообщеники
        free(buf);
    }

}