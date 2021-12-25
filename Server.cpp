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
        offical = write(fd, "Server say: Wrong password\n", 27 + 1);
        perror("Wrong password");
        return (-1);
    }else{
        write(fd,
              "Ok, now enter the command <NICK> and enter your nickname\n",
              57 + 1);
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
                offical = write(ls, "Server say: This name is taken\n", 31 + 1);
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
        write(fd,
              "Enter the command <PASS> and enter the password to connect to the server\n",
              73 + 1);
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

// не работает
void Server::privmisg_work(int num)
{
    size_t pos = this->arr_user[num]->getMsgArgs().find_first_of(' ');
    std::string name = this->arr_user[num]->getMsgArgs().substr( 0, pos);
    pos = this->arr_user[num]->getMsgArgs().find_first_of(':');
    std::string massage = this->arr_user[num]->getMsgArgs().substr(pos + 1);
    //удаление лишних пробелов в начале строки
    while (massage.find_first_not_of(" ") > 0)
        massage.erase(0,1);

    int num_friend = find_num_by_nickname(name);
    if (num_friend < 0) {
        perror("НЕТ СОВПОДЕНИЙ ПО НИКУ !");
    }
    int nbytes;
    int offical;
    // отправляем сообщеньку по фд
    write(this->arr_user[num_friend]->getFd(),
          this->arr_user[num]->getNickname().c_str(),
          this->arr_user[num]->getNickname().length());
    write(this->arr_user[num_friend]->getFd(), " say: ", 6 + 1);
    write(this->arr_user[num_friend]->getFd(), massage.c_str(), massage.length());
    write(this->arr_user[num_friend]->getFd(), "\n",  1);
    std::cout << "Private massage: " << this->arr_user[num]->getMsgArgs().c_str() << "bytes=" << nbytes << std::endl;
    if (offical < 0)
    {
        perror("Server: write failure");
    }
}

// не работает
void Server::info_work(int num)
{

    std::string timeee = std::to_string( clock() / 1000 - start_time);
    // отправляем сообщеньку по фд
    write(this->arr_user[num]->getFd(),"Server vesion: v1.0 ",21);
    write(this->arr_user[num]->getFd(),"\ntime start server = ",22);
    write(this->arr_user[num]->getFd(),timeee.c_str(), timeee.length());
    write(this->arr_user[num]->getFd()," second",7);
    write(this->arr_user[num]->getFd(), "\n",  1);
    std::cout << "info massage: " << "Server vesion: v1.0, " << "time =" << timeee.c_str() << std::endl;
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
//                                           клиента в список канала и канал в список клиента
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
    if ((pos2 = msg.find_first_of(" ", pos +1)) != std::string::npos) {
        write(arr_user[num]->getFd(), "JOIN [channel] [key], joins the channel with the key if provided, reconnects"
                                      " to the current channel if no channel is specified.\n\r", 76+54);
        return;
    }
    std::string topic = msg.substr(0, pos);
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
    } else {//3*
        std::vector<Channel *>::iterator it_b_channel =  this->arr_channel.begin();
        std::vector<Channel *>::iterator it_e_channel =  this->arr_channel.end();
        while (it_b_channel != it_e_channel){
            if ((*it_b_channel)->getTopic() == topic)//3.1
            {
                std::vector<Channel *>::iterator it_b_u_channel =  this->arr_user[num]->getVecChannel().begin();
                std::vector<Channel *>::iterator it_e_u_channel =  this->arr_user[num]->getVecChannel().end();
                while (it_b_u_channel != it_e_u_channel){
                    if ((*it_b_u_channel)->getTopic() == topic){//3.1.1
                        //3.1.1.1
                        write(arr_user[num]->getFd(), "What are you doing here ? don't you know? You are"
                                                      " already in the channel ...\n\r", 49+30);
                    }
                    it_b_u_channel++;
                }
               if ((*it_b_channel)->getPassword() == "" || (*it_b_channel)->getPassword() == key) {
                   //3.1.1.2
                   (*it_b_channel)->addUser(arr_user[num]);
                   this->arr_user[num]->setVecChannel(*(*it_b_channel));
               } else{
                   // 3.1.1.2.2
                   write(arr_user[num]->getFd(), "Lebowski where is the key ?!"
                                                 " You need a password to enter the channel!\n\r", 43+30);
               }
            }else {// точная копия 2*  3.2
                Channel *channel = new Channel(topic);
                if (!key.empty())
                    channel->setPassword(key);
                channel->setOper(arr_user[num]);
                channel->addUser(arr_user[num]);
                setChannel(* channel);
                //закидываем канал в список юзера;
                this->arr_user[num]->setVecChannel(*channel);
            }
            it_b_channel++;
        }
    }





    /*std::vector<std::string> topic;
    std::vector<std::string> password;
    std::vector<std::string>::iterator it_begin = topic.begin();
    std::vector<std::string>::iterator it_end = topic.end();

    std::string grate_mimic = this->arr_user[num]->getMsgArgs();
    int i = 0;
    int j = 0;
    int pos = 0;
    int pos2 = 0;
    std::string mimic;
    //выделяем из общей строки все названия "тайтл"  каналов и пихаем их в массив
    // pos это там где нашли #или & в строке
    while ((pos = grate_mimic.find_first_of("#&", i)) != std::string::npos) {
        //pos 2 это ближайшая запятая или прабел если других тайтлов нет
        pos2 = grate_mimic.find_first_of(" ,", j);
        //выделяем нужный кусочек из строки и копируем в mimic  + 1 нужен так как pos цказывает на # или  &
        //-1 нужен в противовес предыдущему +1
        mimic = grate_mimic.substr(pos + 1, pos2 - pos - 1);
        while (it_begin != it_end)
        {

            it_begin++;
        }
        topic.push_back(mimic);
        grate_mimic.erase(0, mimic.length() + 2);
        while (grate_mimic.find_first_not_of(" ") > 0) //удаление лишних пробелов в начале строки
            grate_mimic.erase(0,1);
        //обязательно делать +1 дабы двигаться по строке
       // i = pos + 1;
       // j = pos2 + 1 ;
    }

    while ((pos = grate_mimic.find(" ")) !=  std::string::npos) //удаление лишних пробелов в начале строки
        grate_mimic.erase(pos,1);
    it_begin = password.begin();
    it_end = password.end();
    i = 0;
    j = 0;
    pos = 0;
    pos2 = 0;
    //выделяем в векторный массив парроли к названиям тайтлов
    //pos это ближай
    while ((pos = grate_mimic.find_first_of(" ,", i)) != std::string::npos) {
       // int pos2 = grate_mimic.find_first_of(" ,", pos);
//        if (pos2 == std::string::npos)
//            pos2 = grate_mimic.length();
        mimic = grate_mimic.substr(pos2, pos);
        while (it_begin != it_end)
        {
            it_begin++;
        }
        password.push_back(mimic);
        grate_mimic.erase(0, mimic.length() + 1);
        while (grate_mimic.find_first_not_of(",") > 0) //удаление лишних пробелов в начале строки
            grate_mimic.erase(0,1);
//        pos2 = pos;
//        i = pos + 1;

    }
    if (grate_mimic.size() != 0)
        while (it_begin != it_end)
        {
            it_begin++;
        }
    password.push_back(grate_mimic);
    std::vector<std::string>::iterator it_begin_topic = topic.begin();
    std::vector<std::string>::iterator it_end_topic = topic.end();
    std::vector<std::string>::iterator it_begin_pas = password.begin();
    std::vector<std::string>::iterator it_end_pas = password.end();
    std::vector<Channel *>::iterator it_begin_channel= this->arr_channel.begin();
    std::vector<Channel *>::iterator it_end_channel = this->arr_channel.end();

    // движемся по массиву с наименованиями топиков
    while (it_begin_topic != it_end_topic){
        // движемся по массиву созданных каналов
        while (it_begin_channel != it_end_channel) {
            //  если их никнеймы совподают то заходим
            if ((*it_begin_channel)->getTopic() == (*it_begin_topic)) {
                //делаем итераторы для движения по массиву адресов узеров зареганых на канале
                std::vector<User *>::iterator it_begin_user = (*it_begin_channel)->getUsersVector_red().begin();
                std::vector<User *>::iterator it_end_user = (*it_begin_channel)->getUsersVector_red().end();
                //если ник юзера написавшего JOIN уже есть в списке , то гони его ссаными тряпками
                if ((*it_begin_user)->getNickname() == this->arr_user[num]->getNickname()) {
                    write(this->arr_user[num]->getFd(), "you are already in this channel", 31 + 1);
                    break;
                }
                //пишем какой конал подключен
                write(this->arr_user[num]->getFd(), "Welcome to the channel ", 22 + 1);
                write(this->arr_user[num]->getFd(), (*it_begin_channel)->getTopic().c_str(),
                      (*it_begin_channel)->getTopic().length());
                write(this->arr_user[num]->getFd(), "\n", 1 + 1);
                // пишем кто еще в этом канале
                while (it_begin_user != it_end_user) {
                    write(this->arr_user[num]->getFd(), (*it_begin_user)->getNickname().c_str(),
                          (*it_begin_user)->getNickname().length());
                    write(this->arr_user[num]->getFd(), "\n", 2);
                    it_begin_user++;
                }
                //добавляем  юзера в "векторный массив адресов юзеров"  кнала
                (*it_begin_channel)->addUser((this->arr_user[num]));
                //удаляем совпадения
                topic.erase( it_begin_topic);
                it_begin_topic = topic.begin();
                it_end_topic = topic.end();
                break;
            }
            it_begin_channel++;
        }
            if (it_begin_channel == it_end_channel || this->arr_channel.size() == 0) {
                // если массив каналов пуст то сразу создаем
                Channel *channel = new Channel((*it_begin_topic));
                // если есть пароль вставляем
                if (it_begin_pas != it_end_pas)
                    channel->setPassword((*it_begin_pas));
                //также заполняем закидываем юзера в список и делаем его царем канала
                channel->addUser((this->arr_user[num]));
                channel->setOper((this->arr_user[num]));
                // закидываем канал в список каналов на сервере
                this->setChannel(*channel);
            }
        // обнуляем итераторы и двигаем тайтоловский и пасвордский
        it_begin_channel = this->arr_channel.begin();
        it_end_channel = this->arr_channel.end();
        it_begin_topic++;
        it_begin_pas++;
    }
*/
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
}


void Server::parser(int num , std::string buf_str, int fd, fd_set &writefds) {

    std::string command = "";
    std::string arg = "";
    int pos = 0;
    if(getAccess(fd) != true) {
        if (this->arr_user[num]->getPassword_init() == false) // проверяем вводил ли он корректный пароль
        {
            pos = buf_str.find_first_of(" ");
            command =  buf_str.substr(0, pos);
            buf_str.erase(0, pos + 1);
            pos = buf_str.find_first_of("\r");
            arg = buf_str.substr(0, pos);
            buf_str.erase(0, pos + 1);
            if (command == "PASS") {
                if (password_verification(arg, fd, num) == -1) // проверяем ввел ли сейчас он корректный пароль
                    return;
            } else {
                perror("Wrong password");
                write(fd,
                      "Enter the command <PASS> and enter the password to connect to the server\n",
                      73 + 1);
            }
        }
        if (this->arr_user[num]->getName_init() == false) // проверяем вводил ли он ник
        {
            if ((pos = buf_str.find_first_of(' ')) != std::string::npos)
                if (buf_str.size() != 0) {
                    command = buf_str.substr(0, pos);
                    this->arr_user[num]->setMsgCom(command);
                    buf_str.erase(0, pos + 1);
                    pos = buf_str.find_first_of("\r");
                    arg = buf_str.substr(0, pos);
                    buf_str.erase(0, pos + 2);
                    if (command == "NICK") {
                        if (name_verification(arg, fd) == -1)//проверяем вводил ли он не занятый ник
                        {
                            return;
                        } else {
                            this->arr_user[num]->setName_init(true);
                            this->arr_user[num]->setNickname(arg); // вносим в объект имя
                            write(fd, "Welcome to the club buddy\r\n", 27 + 1);
                        }
                    } else {
                        write(fd,
                              "now enter the command <NICK> and enter your nickname\n",
                              53 + 1);
                    }
                }
        }
        if ((pos = buf_str.find_first_of(' ')) != std::string::npos)
            if (buf_str.size() != 0) {
                command = buf_str.substr(0, pos);
                this->arr_user[num]->setMsgCom(command);
                command = this->arr_user[num]->getMsgCom();
                buf_str.erase(0, pos + 1);
                pos = buf_str.find_first_of("\r");
                if (pos != -1) {
                    buf_str.erase(pos, 2);
                }
                this->arr_user[num]->setAccess(true);
            }
    }
    if (getAccess(fd) == true) {
        arg = buf_str;
        command = this->arr_user[num]->getMsgCom();
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
                user_work(arg, num);
                break;
            case OPER:
                break;
            case PRIVMSG:
                privmisg_work(num);//* доделать, отправка множеству юзеров
                break;
            case NOTICE:
                break;
            case JOIN:
                join_work(num);
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
        size_t pos =  buf_str_bad.find_first_of('\n');
        buf_str_bad.erase(pos, 1);
        std::string buf_str = buf_str_bad;
        arr_user[num]->make_msg(buf_str);
        if (nbytes < 0){ perror("Server: meh nbytes < 0");}
        else if (nbytes == 0){ perror("Server: meh nbytes == 0");}
        else{
            std::cout << "Serv got massage: " << *buf << std::endl;
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

void Server::write_massage_to_client(int &fd, fd_set &writefds, char **buf)
{
    std::vector<User *>::iterator it_begin_new = arr_user.begin();
    std::vector<User *>::iterator it_end_new = arr_user.end();
    while (it_begin_new != it_end_new){ // идем по всему списку клиентов
        fd = (*it_begin_new)->getFd();// достаем фд нужного клиента по адресу его объекта
        int num = find_who_talk(writefds);// находим кто у нас отправляет письма дабы использовать его имя
        int num_it = find_numb_iter(fd);// находим порядковый номер в векторном массиве данного фд
        if (num != -1 && getAccess(fd)){  // проверяем на наличие допуска и что кто то вообще писал
                    if (!FD_ISSET(fd, &writefds)){
                        int nbytes;
                        int offical;

                        if (*buf[0] != '\0' || *buf[0] != '\n'){ // не уверен что это нужно , не дает спамить
                            offical = write(fd, this->arr_user[num]->getNickname().c_str(),
                                            this->arr_user[num]->getNickname().length()); // отправляем сообщеньку по фд
                            offical = write(fd, " say: ", 6 + 1);
//                    std::cout << "Write back: " << "Server: say" << std::endl << "bytes=" << offical << std::endl;
                            nbytes = write(fd, *buf, strlen(*buf) + 1);
                            std::cout << "Write back: " << *buf << "bytes=" << nbytes << std::endl;
                            if (offical < 0) {
                                perror("Server: write failure");
                            }
                        }
                    }
        }
        // выдаем разрешение на получение и отправку писем .
        if (this->arr_user[num_it]->getPassword_init() == true && this->arr_user[num_it]->getName_init() == true){
            this->setAccess(fd);
        }
        it_begin_new++; // двигаемся далее по списку клиентов
    }
}

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
        write_massage_to_client(fd, writefds, &buf);  // здесь отправляем сообщеники
        if (FD_ISSET(work_fd, &activfds)) {
            int num = find_numb_iter(work_fd);
            this->arr_user[num]->cleaner();
        }
        free(buf);
    }
}

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