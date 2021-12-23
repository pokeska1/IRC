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

    while (it_begin != it_end )
    {
        if (fd == (*it_begin)->getFd())
            (*it_begin)->setAccess(true);
        it_begin++;
    }
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

int &Server::getPort(int i)
{
    std::vector<int>::iterator it_begin = arr_port.begin();
    std::vector<int>::iterator it_end = arr_port.end();
    int j = 0;

    while (it_begin != it_end && j < i)
    {
        if (j == i)
            return(this->arr_port[j]);
        it_begin++;
    }
    return(this->arr_port[j]);
}

bool Server::getAccess(int fd)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();

    while (it_begin != it_end )
    {
        if (fd == (*it_begin)->getFd())
            return((*it_begin)->getAccess());
        it_begin++;
    }
    return(false);
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

int Server::password_verification(std::string &buf, int fd, int num)
{
    if (buf != this->getPassword(0) ) {
        int offical;
        offical = write(fd, "Server say: Wrong password\n", 27 + 1);
        perror("Wrong password");
        return (-1);
    }
    else
    {
        write(fd,
              "Ok, now enter the command <NICK> and enter your nickname\n",
              57 + 1);
        this->arr_client[num]->setPassword_init(true);
    }

    return (0);
    //PRIVMSG приватное сообщение
    //  Parameters: <receiver>{,<receiver>} <text to be sent>
}

int Server::name_verification(std::string &buf, int ls)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    int fd;

    if (it_begin != it_end)  // идем по массиву указателей клиентов с помощью итераторов
    {
        fd = (*it_begin)->getFd();  // вытаскием по адресу определенного клиента его фд
        while (it_begin != it_end)
        {
            fd = (*it_begin)->getFd();
            if (buf == (*it_begin)->getName())
            {
                int offical;
                offical = write(ls, "Server say: This name is taken\n", 31 + 1);
                return (-1);
            }
            it_begin++;
        }
    }

    return(1);
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

        write(fd,
              "Enter the command <PASS> and enter the password to connect to the server\n",
              73 + 1);

        FD_SET(fd, &activfds);// добавляем новичка к активным портам которые мы будем слушать
        Client *new_client = new Client(fd); // создаем для этого клиента объект  класса со своим блэк-джеком и fd
        this->setClient(*new_client); // запоминаем его адрес в массив указателей
    }
}

int Server::find_numb_iter(int fd)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    int j = 0;

    while (it_begin != it_end)
    {
        if (fd == (*it_begin)->getFd())
            return(j);
        it_begin++;
        j++;
    }
    return(j);
}

// ловим что написал старый клиент , проверяем на доступ и распределяем, что с ним делать.
void Server::get_old_client_massage(int &fd, fd_set &activfds, fd_set &writefds, char **buf)
{
    int num = find_numb_iter(fd); // найдем порядковый номер этого клиента в массиве
    if (FD_ISSET(fd, &activfds))//если фд есть в списке акитвистов то начнем с ним работать
    {
        int nbytes;

        nbytes = recv(fd, *buf, 512, 0); // прочтем в массив чаров его сообщение (не обробатывал переполнение буфера)
        if (nbytes < 0){ perror("Server: meh nbytes < 0");}
        else if (nbytes == 0){ perror("Server: meh nbytes == 0");}
        else
        {
            std::cout << "Serv got massage: " << *buf << std::endl;
            // для удобства пусть сервер отобразит что поймал
            if(getAccess(fd) == true)// если у клиента есть дотуп , то вносим его в список "дозволяющие писать"
            {
                FD_SET(fd, &writefds);
            }
            else // иначе проверяем что он там наколякал
            {
                std::string buf_str = *buf;
                if (this->arr_client[num]->getPassword_init() == false) // проверяем вводил ли он корректный пароль
                {
                    if(password_verification(buf_str, fd, num) == -1) // проверяем ввел ли сейчас он корректный пароль
                        return;
                }
                else if (this->arr_client[num]->getName_init() == false) // проверяем вводил ли он ник
                {
                    if (name_verification(buf_str, fd) == -1)//проверяем вводил ли он не занятый ник
                        return;
                    else
                    {
                        write(fd,
                              "Welcome to the club buddy\n",
                              26 + 1);
                        this->arr_client[num]->setName_init(true);
                        this->arr_client[num]->setName(buf_str); // вносим в объект имя
                    }
                }
            }
        }
    }
}


int Server::find_who_talk(fd_set &writefds)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    int j = 0;

    while (it_begin != it_end)
    {
        if (FD_ISSET((*it_begin)->getFd(),&writefds))
            return(j);
        it_begin++;
        j++;
    }
    return(-1);
}

void Server::write_massage_to_client(int &fd, fd_set &writefds, char **buf)
{
    std::vector<Client *>::iterator it_begin_new = arr_client.begin();
    std::vector<Client *>::iterator it_end_new = arr_client.end();

    while (it_begin_new != it_end_new) // идем по всему списку клиентов
    {

        fd = (*it_begin_new)->getFd();// достаем фд нужного клиента по адресу его объекта
        int num = find_who_talk(writefds);// находим кто у нас отправляет письма дабы использовать его имя
        int num_it = find_numb_iter(fd);// находим порядковый номер в векторном массиве данного фд
        if (num != -1 && getAccess(fd))  // проверяем на наличие допуска и что кто то вообще писал
        {
            if (!FD_ISSET(fd, &writefds)) // мы будем отправлять сообщение всем , кроме того кто написал.
            {
                int nbytes;
                int offical;

                if (*buf[0] != '\0' || *buf[0] != '\n') // не уверен что это нужно , не дает спамить
                {
                    offical = write(fd, this->arr_client[num]->getName().c_str(), this->arr_client[num]->getName().length() - 1); // отправляем сообщеньку по фд
                   offical = write(fd, " say: ", 6 + 1);
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
        // выдаем разрешение на получение и отправку писем .
        if (this->arr_client[num_it]->getPassword_init() == true && this->arr_client[num_it]->getName_init() == true)
        {
            this->setAccess(fd);
        }
        it_begin_new++; // двигаемся далее по списку клиентов
    }
}

enum    forms
{
    NOT_DEFINED,
    USER,
    OPER,
    PRIVMSG,
    NOTICE,
    JOIN,
    MODE,
    TOPIC,
    INVITE,
    KICK,
    PART,
    KILL,
    VERSION,
    INFO
};

void Server::privmisg_work(int num)
{
    size_t pos = this->arr_client[num]->getMassage().find_first_of(' ');
    std::string name = this->arr_client[num]->getMassage().substr(-1, pos);
    std::string massage = this->arr_client[num]->getMassage().substr(pos + 2);

    int i = 0;
    while (this->arr_client[i]->getName() != name)
        i++;

    int nbytes;
    int offical;
    offical = write(this->arr_client[i]->getFd(), this->arr_client[num]->getName().c_str(), this->arr_client[num]->getName().length() - 1); // отправляем сообщеньку по фд
    offical = write(this->arr_client[i]->getFd(), " say: ", 6 + 1);
    nbytes = write(this->arr_client[i]->getFd(), this->arr_client[num]->getMassage().c_str(),
                   strlen(this->arr_client[num]->getMassage().c_str()) + 1);
    std::cout << "Write back: " << this->arr_client[num]->getMassage().c_str() << "bytes=" << nbytes << std::endl;
    if (offical < 0)
    {
        perror("Server: write failure");
    }
}

/*void Server::parser(int num , std::string buf_str, int fd)
{
    std::string command = this->arr_client.msg_frags;
    if (this->arr_client[num]->getPassword_init() == false) // проверяем вводил ли он корректный пароль
    {
        if(command == "PASS")
            if(password_verification(buf_str, fd, num) == -1) // проверяем ввел ли сейчас он корректный пароль
                return;
    }
    else if (this->arr_client[num]->getName_init() == false) // проверяем вводил ли он ник
    {
        if (command == "NICK") {
            if (name_verification(buf_str, fd) == -1)//проверяем вводил ли он не занятый ник
            {
                return;
            } else {
                this->arr_client[num]->setName_init(true);
                this->arr_client[num]->setName(buf_str); // вносим в объект имя
            }
        }
    }
    else if(getAccess(fd) == true)
    {

        std::map<std::string, forms> map_forms;
        map_forms["USER"] = USER;
        map_forms["OPER"] = OPER;
        map_forms["PRIVMSG"] = PRIVMSG;
        map_forms["NOTICE"] = NOTICE;
        map_forms["JOIN"] = JOIN;
        map_forms["MODE"] = MODE;
        map_forms["TOPIC"] = TOPIC;
        map_forms["INVITE"] = INVITE;
        map_forms["KICK"] = KICK;
        map_forms["PART"] = PART;
        map_forms["KILL"] = KILL;
        map_forms["VERSION"] = VERSION;
        map_forms["INFO"] = INFO;


        switch (map_forms[command]) {
            case USER:
                break;
            case OPER:
                break;
            case PRIVMSG:
                privmisg_work(num);
                break;
            case NOTICE:
                break;
            case JOIN:
                break;
            case MODE:
                break;
            case TOPIC:
                break;
            case INVITE:
                break;
            case KICK:
                break;
            case PART:
                break;
            case KILL:
                break;
            case VERSION:
                break;
            case INFO:
                break;
            default:
                break;
        }
    }
}*/



void Server::work(int ls) {
    fd_set writefds, activfds;

    int max_d = ls; // считаем что макс сокет это нынешний слушающий

    for (;;) {
        int fd;
        char *buf = (char *) calloc(sizeof(char), BUFLEN + 1);

        FD_ZERO(&writefds);
        FD_ZERO(&activfds);
        FD_SET(ls, &activfds);//вводим fd слушашего сокета в множество
        // а теперь цикл по сокетам заносим все fd из списка клиентов в множество;
        create_many_active_fd(fd, activfds, max_d);
        int res = select(FD_SETSIZE, &activfds, NULL, NULL, NULL);// проверяем на активность
        if (res < 1) {
            if (errno != EINTR) {
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