//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Server.hpp"

void Server::setHost(std::string &host) { this->host = host; }

void Server::setPassword(std::string &password){
    std::vector<std::string *>::iterator it_begin = arr_password.begin();
    std::vector<std::string *>::iterator it_end = arr_password.end();
    while (it_begin != it_end){ it_begin++; }
    arr_password.push_back(&password);
}

void Server::setPort(int port){
    std::vector<int>::iterator it_begin = arr_port.begin();
    std::vector<int>::iterator it_end = arr_port.end();
    while (it_begin != it_end) { it_begin++; }
    arr_port.push_back(port);
}

void Server::setUser(User &user){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    while (it_begin != it_end){ it_begin++; }
    arr_user.push_back(&user);
}

void Server::setChannel(Channel &channel){
    std::vector<Channel *>::iterator it_begin = arr_channel.begin();
    std::vector<Channel *>::iterator it_end = arr_channel.end();
    while (it_begin != it_end) { it_begin++; }
    arr_channel.push_back(&channel);
}

void Server::setAccess(int fd){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    while (it_begin != it_end ){
        if (fd == (*it_begin)->getFd())
            (*it_begin)->setAccess(true);
        it_begin++;
    }
}

std::string const &Server::getPassword(int i){
    int j = 0;
    std::vector<std::string *>::iterator it_begin = arr_password.begin();
    std::vector<std::string *>::iterator it_end = arr_password.end();
    while (it_begin != it_end && j != i){ it_begin++; j++; }
    std::string *mimic = (*it_begin);
    return(*mimic);
}

int &Server::getPort(int i){
    std::vector<int>::iterator it_begin = arr_port.begin();
    std::vector<int>::iterator it_end = arr_port.end();
    int j = 0;
    while (it_begin != it_end && j < i){
        if (j == i)
            return(this->arr_port[j]);
        it_begin++;
    }
    return(this->arr_port[j]);
}

bool Server::getAccess(int fd)
{
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    while (it_begin != it_end ){
        if (fd == (*it_begin)->getFd())
            return((*it_begin)->getAccess());
        it_begin++;
    }
    return(false);
}

void Server::deleteClient(int fd){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    while (it_begin != it_end)
    {
        if (fd == (*it_begin)->getFd()) {
            arr_user.erase(it_begin);
            return;
        }
        it_begin++;
    }
}

void Server::deleteChannel(std::string topic){
    std::vector<Channel *>::iterator it_begin = arr_channel.begin();
    std::vector<Channel *>::iterator it_end = arr_channel.end();
    while (it_begin != it_end){
        if (topic == (*it_begin)->getTopic()) {
            arr_channel.erase(it_begin);
            return;
        }
        it_begin++;
    }
}

int Server::password_verification(std::string &buf, int fd, int num){
    if (buf != this->getPassword(0) ) {
        int offical;
//        offical = send(fd, "Server say: Wrong password\n", 27 + 1, 0);
            std::cout << "Wrong password\n";
        //perror("Wrong password");
        return (-1);
    }else{
            std::cout << "OK password\n";

//        send(fd,
//              "Ok, now enter the command <NICK> and enter your nickname\n",
//              57 + 1, 0);
        this->arr_user[num]->setPassword_init(true);
    }

    return (0);
}

int Server::name_verification(std::string &buf, int ls)
{
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    int fd;
    if (it_begin != it_end){  // идем по массиву указателей клиентов с помощью итераторов
        fd = (*it_begin)->getFd();  // вытаскием по адресу определенного клиента его фд
        while (it_begin != it_end){
            fd = (*it_begin)->getFd();
            if (buf == (*it_begin)->getNickname()){
                int offical;
                std::cout << "This name is taken\n";
//                offical = send(ls, "Server say: This name is taken\n", 31 + 1, 0);
                return (-1);
            }
            it_begin++;
        }
    }
    return(1);
}

// здесь мы создаем "список" fd которые будет проверять на активность
void Server::create_many_active_fd(int &fd, fd_set &activfds, int &max_d){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    if (it_begin != it_end){ // идем по массиву указателей клиентов с помощью итераторов
        fd = (*it_begin)->getFd();  // вытаскием по адресу определенного клиента его фд
        while (it_begin != it_end){
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
void Server::get_new_client(int &ls, int &fd, fd_set &activfds){
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
//        send(fd,
//              "Enter the command <PASS> and enter the password to connect to the server\n",
//              73 + 1, 0);
        std::cout << "Client mast write command <PASS> and enter the password to connect to the server\n";
        FD_SET(fd, &activfds);// добавляем новичка к активным портам которые мы будем слушать
        User *arr_user = new User(fd); // создаем для этого клиента объект  класса со своим блэк-джеком и fd
        this->setUser(*arr_user); // запоминаем его адрес в массив указателей
    }
}

int Server::find_numb_iter(int fd){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    int j = 0;

    while (it_begin != it_end){
        if (fd == (*it_begin)->getFd())
            return(j);
        it_begin++;
        j++;
    }
    return(j);
}

enum    forms{
    NOT_DEFINED,
    USER, // workv
    OPER,
    PRIVMSG,//work
    NOTICE,
    JOIN,
    MODE,//build
    TOPIC,
    INVITE,
    KICK,
    PART,
    KILL,
    VERSION, // build
    INFO // work , but mast upgrade
};

int Server::find_num_chan_by_topic(std::string const &topic) {
    std::vector<Channel *>::iterator it_begin = arr_channel.begin();
    std::vector<Channel *>::iterator it_end = arr_channel.end();

    int i = 0;
    while (it_begin != it_end){
        if ((*it_begin)->getTopic() == topic)
            return(i);
        i++;
        it_begin++;
    }
    return (-1);
}

int Server::find_num_by_nickname(std::string const &nick){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();

    int i = 0;
    while (it_begin != it_end){
        if ((*it_begin)->getNickname() == nick)
            return(i);
        i++;
        it_begin++;
    }
    return (-1);
}

// не работает c множеством каналов.
void Server::privmisg_work(int num) {
    int nbytes;

    size_t pos = this->arr_user[num]->getMsgArgs().find_first_of(' ');
    std::string name = this->arr_user[num]->getMsgArgs().substr(0, pos);
    pos = this->arr_user[num]->getMsgArgs().find_first_of(':');
    std::string massage = this->arr_user[num]->getMsgArgs().substr(pos + 1);
    //удаление лишних пробелов в начале строки
    while (massage.find_first_not_of(" ") > 0)
        massage.erase(0, 1);

    if ((pos = this->arr_user[num]->getMsgArgs().find_first_of("#&")) != std::string::npos) {
        int pos2 = this->arr_user[num]->getMsgArgs().find_first_of(' ');
        std::string channel = arr_user[num]->getMsgArgs().substr(pos + 1, pos2 - 1);
        pos = this->arr_user[num]->getMsgArgs().find_first_of(':');
        std::cout << "channel topic =" << channel << "|massage =|" << massage << "|" << "\n";
        int num_channel = find_num_chan_by_topic(channel);
        std::vector<User *>::iterator it_begin = arr_channel[num_channel]->getUsersVector_red().begin();
        std::vector<User *>::iterator it_end = arr_channel[num_channel]->getUsersVector_red().end();

        while (it_begin != it_end){
            if ((*it_begin)->getNickname() != arr_user[num]->getNickname()) {
                std::string msg = ":" + arr_user[num]->getNickname() + "!" + arr_user[num]->getNickname() +
                                  "@" + arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " " +
                                  arr_user[num]->getMsgArgs() + "\r\n";
                int nbytes = send((*it_begin)->getFd(), msg.c_str(), msg.length(), 0);
                std::cout << msg << std::endl;
            }
            it_begin++;
        }
    }else {
        int num_friend = find_num_by_nickname(name);
        if (num_friend < 0) {
            std::cout << "Нет такого челика\n";
            return;
        }
        // отправляем сообщеньку по фд
        std::string msg = ":" + arr_user[num]->getNickname() + "!" + arr_user[num]->getNickname() +
                          "@" + arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " " +
                          arr_user[num_friend]->getNickname() + ":" + arr_user[num]->getMsgArgs() + "\r\n";
        nbytes = send(this->arr_user[num_friend]->getFd(), msg.c_str(), msg.length(), 0);
        std::cout << "Private massage: " << msg.c_str() << "bytes=" << nbytes
                  << std::endl;
        if (nbytes < 0) {
            perror("Server: write failure");
        }
    }
}

// не работает
void Server::info_work(int num)
{

    std::string timeee = std::to_string( clock() / 1000 - start_time);
    // отправляем сообщеньку по фд
    send(this->arr_user[num]->getFd(),"Server vesion: v1.0 ",21, 0);
    send(this->arr_user[num]->getFd(),"\ntime start server = ",22, 0);
    send(this->arr_user[num]->getFd(),timeee.c_str(), timeee.length(), 0);
    send(this->arr_user[num]->getFd()," second",7, 0);
    send(this->arr_user[num]->getFd(), "\n",  1, 0);
    std::cout << "info massage: " << "Server vesion: v1.0, " << "time =" << timeee.c_str() << std::endl;
}

void Server::say_hello_to_new_in_channel(int num, std::vector<Channel *>::iterator it_b_channel, std::string topic){
    //отправляем что он в канале
    std::string msg = arr_user[num]->getNickname() + "!" + arr_user[num]->getNickname() + "@"
                      + arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " #" +
                      topic + "\r\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
    // отправляем всех кто есть на канале новичку
    std::vector<User *>::iterator it_begin = (*it_b_channel)->getUsersVector_red().begin();
    std::vector<User *>::iterator it_end = (*it_b_channel)->getUsersVector_red().end();

    //3 сообщение
    msg = ":localhost 332 " + arr_user[num]->getNickname() + " #" + topic +
          " :A timey-wimey channel\r\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);

    //4а сообщение
    msg = ":localhost 353 " + arr_user[num]->getNickname() + " = #" + topic +
          " :@";
    while (it_begin != it_end) {
        msg += (*it_begin)->getNickname() + " ";
        it_begin++;
    }
    msg.erase(msg.length() - 1, 1);
    msg += "\r\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
    //4b
    msg = ":localhost 366 " + arr_user[num]->getNickname() + " #" + topic +
          " :END of NAMES list\r\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);


}

//не работаеt
//структура
//1) расщепление аргумента на массивы topic и их паролей
//2) проверяем есть ли хоть 1 канал если нет то сразу создаем
//3)если массив каналов есть то начинаем сравнивать их topic
//      3.1)есть совпадение
//              3.1.1) проверяем есть ли данный topic в списке у клиента
//                      3.1.1.1) да тогда пишел нахуй от сюда , ты уже в канале
//                      3.1.1.2) нет ,проверяем совпал ли пароль если он есть
//                         3.1.1.2.1) да пароль совпал или пароля у канала нет,
//                                          клиента в список канала и канал в список клиента new
//                                          оповеаем всех что появился новеньки + список стариков новичку
//                         3.1.1.2.2) нет , смотри до запятой 3.1.1.1)
//      3.2)если нет то создаем канал , заносим клиента в список канала и канал в список клиента
//      3.2)добавляем клиента как опера
//          3.2.1) если есть пароль ддобавляем его в канал
//
void Server::join_work(int num) {

    //1*
    std::string msg = this->arr_user[num]->getMsgArgs();
    int pos = msg.find_first_of(" ");
    int pos2 = 0;
    std::vector<Channel *>::iterator it_b_channel =  this->arr_channel.begin();
    std::vector<Channel *>::iterator it_e_channel =  this->arr_channel.end();
    if ((pos2 = msg.find_first_of(" ", pos +1)) != std::string::npos) {
        std::cout << "JOIN [channel] [key], joins the channel with the key if provided, reconnects"
                                      " to the current channel if no channel is specified.\n";
        return;
    }
    std::string topic = msg.substr(1, pos);
    std::string key = "";
    if (pos != -1)
        std::string key = msg.substr(pos + 1, msg.length());
    //2*
    if (this->arr_channel.empty())
    {
        //создаем канал, заполняем topic, пароль если есть, опера , и юзера в список, вносим канал в список сервера
        Channel *channel = new Channel(topic);
        if (!key.empty())
            channel->setPassword(key);
        channel->setOper(arr_user[num]);
        channel->addUser(arr_user[num]);
        setChannel(* channel);
        //закидываем канал в список юзера;
        this->arr_user[num]->setVecChannel(*channel);
        it_b_channel =  this->arr_channel.begin();
        it_e_channel =  this->arr_channel.end();
        say_hello_to_new_in_channel(num, it_b_channel, topic);
    } else {//3*
        while (it_b_channel != it_e_channel) {
            if ((*it_b_channel)->getTopic() == topic)//3.1
            {
                std::vector<Channel *>::iterator it_b_u_channel = this->arr_user[num]->getVecChannel().begin();
                std::vector<Channel *>::iterator it_e_u_channel = this->arr_user[num]->getVecChannel().end();
                while (it_b_u_channel != it_e_u_channel) {
                    if ((*it_b_u_channel)->getTopic() == topic) {//3.1.1
                        //3.1.1.1
                        std::cout << "What are you doing here ? don't you know? You are"
                                     " already in the channel ...\n";
//                        send(arr_user[num]->getFd(), "What are you doing here ? don't you know? You are" " already in the channel ...\n\r", 49+30, 0);
                        return;
                    }
                    it_b_u_channel++;
                }
                if ((*it_b_channel)->getPassword() == "" || (*it_b_channel)->getPassword() == key) {
                    //3.1.1.2
                    //отправляем всем кто присоединился
                    this->arr_user[num]->setVecChannel(*(*it_b_channel));
                    (*it_b_channel)->addUser(arr_user[num]);
                    std::vector<User *>::iterator it_begin = (*it_b_channel)->getUsersVector_red().begin();
                    std::vector<User *>::iterator it_end = (*it_b_channel)->getUsersVector_red().end();
                    std::string msg = arr_user[num]->getNickname() + "!" + arr_user[num]->getNickname() + "@"
                                      + arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " #" +
                                      topic + "\r\n";

                    while (it_begin != it_end) {
                        //2 второе сообщение после JOIN #channel
                        int nbytes = send((*it_begin)->getFd(), msg.c_str(), msg.length(), 0);
                        std::cout << "Private massage: " << msg.c_str() << "bytes=" << nbytes
                                  << std::endl;
                        it_begin++;
                    }
                    it_b_channel =  this->arr_channel.begin();
                    it_e_channel =  this->arr_channel.end();
                    say_hello_to_new_in_channel(num, it_b_channel, topic);
                    break;
                } else {
                    // 3.1.1.2.2
                    std::cout << "Lebowski where is the key ?!"
                                 " You need a password to enter the channel!\n";
//                   send(arr_user[num]->getFd(), "Lebowski where is the key ?!"
//                                                 " You need a password to enter the channel!\n\r", 43+30, 0);
                    return;
                }
            }
            it_b_channel++;
        }
        if (it_b_channel == it_e_channel)// точная копия 2*  3.2
        {
            Channel *channel = new Channel(topic);
            if (!key.empty())
                channel->setPassword(key);
            channel->setOper(arr_user[num]);
            channel->addUser(arr_user[num]);
            setChannel(*channel);
            //закидываем канал в список юзера;
            this->arr_user[num]->setVecChannel(*channel);
            it_b_channel =  this->arr_channel.begin();
            it_e_channel =  this->arr_channel.end();
            say_hello_to_new_in_channel(num, it_b_channel, topic);
        }
    }
}

void    Server::user_work(std::string &arg, int num)
{
    int pos = arg.find_first_of(" ");
    std::string user_name = arg.substr(0, pos);
    int pos2 = arg.find(" ", pos + 1);
    std::string host_name = arg.substr(pos + 1, pos2 - pos - 1);
    pos = pos2;
    pos2 = arg.find(" ", pos + 1);
    std::string server_name = arg.substr(pos + 1, pos2 - pos - 1);
    pos = arg.find_first_of(":");
    std::string real_name = arg.substr(pos + 1, arg.length() - pos - 1);
    while ((pos = real_name.find_first_of(" ")) != 0)
        real_name.erase(0, 1);
    this->arr_user[num]->setUsername(user_name);
    this->arr_user[num]->setHostname(host_name);
    this->arr_user[num]->setServername(server_name);
    this->arr_user[num]->setRealname(real_name);
    std::cout << "OK useraname" << std::endl;
}


void Server::parser(int num , std::string buf_str, int fd, fd_set &writefds) {

//    printf("|%s|%s|\n" ,this->arr_user[num]->getMsgCom().c_str(), this->arr_user[num]->getMsgArgs().c_str());
//    std::string plz = "getMsgCom |" + this->arr_user[num]->getMsgCom() + "| getMsgArgs|" + this->arr_user[num]->getMsgArgs()
//            + "| buf_str|" + buf_str;
//    std::cout  << plz << std::endl;
    arr_user[num]->make_msg(buf_str);
    std::string command = "";
    std::string arg = "";
    int pos_n = 0;
    int pos_r = 0;
    if (getAccess(fd) != true) {
//        arr_user[num]->make_msg(buf_str);
        if (this->arr_user[num]->getPassword_init() == false) {
            if (arr_user[num]->getMsgCom() == "PASS") {
                std::string arg;
                // проверяем ввел ли сейчас он корректный пароль
                if (password_verification(arr_user[num]->getMsgArgs(), fd, num) == -1)
                    return;
            } else
                return;
        } else if (this->arr_user[num]->getName_init() == false) {
            if (arr_user[num]->getMsgCom() == "NICK") {
                if (name_verification(arr_user[num]->getMsgArgs(), fd) == -1)//проверяем вводил ли он не занятый ник
                {
                    return;
                } else {
                    this->arr_user[num]->setName_init(true);
                    this->arr_user[num]->setNickname(arr_user[num]->getMsgArgs()); // вносим в объект имя
//                            send(fd, "Welcome to the club buddy\r\n", 27 + 1, 0);
                            std::cout << "OK nickname " << std::endl;
                }
            }
        } else if (arr_user[num]->getMsgCom() == "USER") {
            user_work(arr_user[num]->getMsgArgs(), num);
            std::string send_msg = ":localhost 001 " + this->arr_user[num]->getNickname() +
                                   " :Welcome to the Internet Relay Network " +
                                   this->arr_user[num]->getNickname() +
                                   "!" + this->arr_user[num]->getUsername() + "@" +
                                   this->arr_user[num]->getHostname() + "\r\n";
            send(fd, send_msg.c_str(), send_msg.length(), 0);
            this->arr_user[num]->setAccess(true);
        } else
            return;
    }
    else if (getAccess(fd) == true) {
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

        std::cout << "Switch" << "\n";
        switch (map_forms[arr_user[num]->getMsgCom()]) {
            case USER:
//                user_work(arg, num);
                break;
            case OPER:
                break;
            case PRIVMSG:
                std::cout << "privmisg_work" << "\n";
                privmisg_work(num);//* доделать, отправка множеству юзеров
                break;
            case NOTICE:
                break;
            case JOIN:
                join_work(num);
                break;
            case MODE:
				mode_chan(num);
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
				version(num);
                break;
            case INFO:
                info_work(num);
                break;
            default:
                FD_SET(fd, &writefds);
                return;
        }
    }
}

// ловим что написал старый клиент , проверяем на доступ и распределяем, что с ним делать.
int Server::get_old_client_massage(int &fd, fd_set &activfds, fd_set &writefds, char **buf)
{
    int num = find_numb_iter(fd); // найдем порядковый номер этого клиента в массиве
    if (FD_ISSET(fd, &activfds)){//если фд есть в списке акитвистов то начнем с ним работать
        int nbytes;
        int num = find_numb_iter(fd);
        nbytes = recv(fd, *buf, 512, 0); // прочтем в массив чаров его сообщение (не обробатывал переполнение буфера)
        std::string buf_str_bad = *buf;
        size_t pos = buf_str_bad.find_first_of("\r\n");
        std::string buf_str = buf_str_bad.substr(0, pos);
        if (nbytes < 0){ perror("Server: meh nbytes < 0");}
        else if (nbytes == 0){ perror("Server: meh nbytes == 0");}
        else{
            std::cout << "Serv got massage: |" << buf_str << "|" << std::endl;
            parser(num , buf_str,  fd, writefds);
        }
        return (fd);
    }
    return 0;
}


int Server::find_who_talk(fd_set &writefds)
{
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
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

//void Server::write_massage_to_client(int &fd, fd_set &writefds, char **buf)
//{
//    std::vector<User *>::iterator it_begin_new = arr_user.begin();
//    std::vector<User *>::iterator it_end_new = arr_user.end();
//    while (it_begin_new != it_end_new){ // идем по всему списку клиентов
//        fd = (*it_begin_new)->getFd();// достаем фд нужного клиента по адресу его объекта
//        int num = find_who_talk(writefds);// находим кто у нас отправляет письма дабы использовать его имя
//        int num_it = find_numb_iter(fd);// находим порядковый номер в векторном массиве данного фд
//        if (num != -1 && getAccess(fd)){  // проверяем на наличие допуска и что кто то вообще писал
//                    if (!FD_ISSET(fd, &writefds)){
//                        int nbytes;
//                        int offical;
//
//                        if (*buf[0] != '\0' || *buf[0] != '\n'){ // не уверен что это нужно , не дает спамить
//                            offical = send(fd, this->arr_user[num]->getNickname().c_str(),
//                                            this->arr_user[num]->getNickname().length(), 0); // отправляем сообщеньку по фд
//                            offical = send(fd, " say: ", 6 + 1, 0);
////                    std::cout << "Write back: " << "Server: say" << std::endl << "bytes=" << offical << std::endl;
//                            nbytes = send(fd, *buf, strlen(*buf) + 1, 0);
//                            std::cout << "Write back: " << *buf << "bytes=" << nbytes << std::endl;
//                            if (offical < 0) {
//                                perror("Server: write failure");
//                            }
//                        }
//                    }
//        }
//        // выдаем разрешение на получение и отправку писем .
////        if (this->arr_user[num_it]->getPassword_init() == true && this->arr_user[num_it]->getName_init() == true){
////            this->setAccess(fd);
////        }
//        it_begin_new++; // двигаемся далее по списку клиентов
//    }
//}

void Server::work(int ls) {

    fd_set writefds, activfds;
    int work_fd;
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
        for (int i = 0; i <= max_d; i++) { //поочереди проверим все айди
            if (i == ls) {   // если сигнал пришел со слышашего порта то это новое подключение!
                get_new_client(ls, fd, activfds);
            } else {  // если с любого другого , то это уже старечки что то пишут и нужно посмотреть что там
                work_fd = get_old_client_massage(i, activfds, writefds, &buf);
            }
        }
       // write_massage_to_client(fd, writefds, &buf);  // здесь отправляем сообщеники
        if (FD_ISSET(work_fd, &activfds)) {
            int num = find_numb_iter(work_fd);
            this->arr_user[num]->cleaner();
        }
        free(buf);

    }
}


//		########  ##     ## ######## ########  ########  #### ########
//		##     ## ###   ### ##       ##     ## ##     ##  ##  ##
//		##     ## #### #### ##       ##     ## ##     ##  ##  ##
//		########  ## ### ## ######   ########  ########   ##  ######
//		##   ##   ##     ## ##       ##   ##   ##   ##    ##  ##
//		##    ##  ##     ## ##       ##    ##  ##    ##   ##  ##
//		##     ## ##     ## ######## ##     ## ##     ## #### ########


std::vector<std::string>	Server::splitStr(std::string str)
{
	std::vector<std::string>	res;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string tmp;

	while ((pos = str.find(delimiter)) != std::string::npos) {
		tmp = str.substr(0, pos);
		res.push_back(tmp); //action
		str.erase(0, pos + delimiter.length());
		while (str.find_first_not_of(" ") > 0) //удаление лишних пробелов в начале строки
			str.erase(0,1);
	}
	tmp = str.substr(0, pos);
	res.push_back(tmp); //action
	return res;
}

int		Server::version(int num)
	{
		std::string msg = "Server vesion: v1.0\n";
		write(this->arr_user[num]->getFd(), msg.c_str(), msg.length());
		std::cout << "version massage: " << msg;
		return 0;
	}

bool	Server::is_chan(std::string str)
	{
		if (str[0] != '#' && str[0] != '&') //проверка: не канал
			return false;
		return true;
	}
bool	Server::chan_in_list(std::string str, std::vector<Channel *> &arr_channel)
{
	std::vector<Channel *>::iterator it_chan = arr_channel.begin();
	for (; it_chan != arr_channel.end(); ++it_chan)
	{
		if (str == (*it_chan)->getName())
			return true;
	}
	return false;
}
Channel	*Server::find_chan(std::string str)
{
	std::vector<Channel *>::iterator it_chan = arr_channel.begin();
	for (; it_chan != arr_channel.end(); ++it_chan)
	{
		if (str == (*it_chan)->getName())
			return *it_chan;
	}
	return *it_chan;
}
int		Server::mode_chan(int num)
{
	Channel *test = new Channel("test"); //default channel - delete on production
	arr_channel.push_back(test);
	std::cout << "arr_channel.size()=" << arr_channel.size() << std::endl;
	// this->arr_user[num] - обращение к классу User
	std::vector<std::string> args = splitStr(this->arr_user[num]->getMsgArgs());
	std::cout << this->arr_user[num]->getMsgArgs() << "   |" << args.size() << std::endl;
	if (this->arr_user[num]->getMsgArgs() == "")
	//if (args.size() == 0) //проверка нет аргументов
	{
		std::string msg = "<command> :Not enough parameters\n";
		write(this->arr_user[num]->getFd(), msg.c_str(), msg.length());
		return 1;
	}
	if (is_chan(args[0]) == false) //проверка: не канал
	{
		std::string msg = "<channel name> :No such channel\n";
		write(this->arr_user[num]->getFd(), msg.c_str(), msg.length());
		return 1;
	}
	(args[0]).erase(0,1); // удаляем символ #/&
	if (chan_in_list(args[0], arr_channel) == false) //проверка: нет в списке каналов
	{
		std::string msg = "<channel 2name> :No such channel\n";
		write(this->arr_user[num]->getFd(), msg.c_str(), msg.length());
		return 1;
	}
	Channel *cur_chan = find_chan(args[0]);
	if ((args[1])[0] == '+') //флаги в true
	{
		(args[1]).erase(0,1);
		std::size_t found = (args[1]).find_first_not_of("psitnml");
		if (found!=std::string::npos)
		{
			std::string msg = ":Unknown MODE flag\n";
			write(this->arr_user[num]->getFd(), msg.c_str(), msg.length());
			return 1;
		}
		if (args.size() > 2)
			cur_chan->setParamTrue(args[1], args[2]);
		else
			cur_chan->setParamTrue(args[1]);
	}
	if ((args[1])[0] == '-') //флаги в false
	{
		(args[1]).erase(0,1);
		std::size_t found = (args[1]).find_first_not_of("psitnm");
		if (found!=std::string::npos)
		{
			std::string msg = ":Unknown MODE flag\n";
			write(this->arr_user[num]->getFd(), msg.c_str(), msg.length());
			return 1;
		}
		cur_chan->setParamFalse(args[1]);
	}
	return 0;
}

// THE END