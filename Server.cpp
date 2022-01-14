//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Server.hpp"

Server::Server()
{
    this->channel_exist = false;
    host = "localhost";
    time(&start_time);
}
// ############################################################################################################
                                                   //Сеторы
void Server::setAccess(int fd){
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    while (it_begin != it_end ){
        if (fd == (*it_begin)->getFd())
            (*it_begin)->setAccess(true);
        it_begin++;
    }
}
void Server::setPort(int port){ arr_port.push_back(port);}
void Server::setUser(User &user){ arr_user.push_back(&user);}
void Server::setHost(std::string &host){ this->host = host;}
void Server::setChannel(Channel &channel){ arr_channel.push_back(&channel);}
void Server::setPassword(std::string &password){ arr_password.push_back(&password);}
// ############################################################################################################
                                                    //Геторы
std::string &Server::getHost(){ return host; }
std::vector<Channel *> &Server::getChannel(){return (this->arr_channel);}
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
    VEC_ITER_USER_ADR it_begin = arr_user.begin();
    VEC_ITER_USER_ADR it_end = arr_user.end();
    while (it_begin != it_end ){
        if (fd == (*it_begin)->getFd())
            return((*it_begin)->getAccess());
        it_begin++;
    }
    return(false);
}

std::string const &Server::getPassword(int i){
    int j = 0;
    VEC_ITER_STR_ADR it_begin = arr_password.begin();
    VEC_ITER_STR_ADR it_end = arr_password.end();
    while (it_begin != it_end && j != i){ it_begin++; j++; }
    std::string *mimic = (*it_begin);
    return(*mimic);
}
std::vector<User *>     &Server::getUserVector() { return (this->arr_user);}

// ##################################################################################
                            // функции упрощающие жизнь и код
enum forms{NOT_DEFINED, \
		NICK,           \
		USER,           \
		OPER,           \
		PRIVMSG,        \
		NOTICE,         \
		JOIN,           \
		MODE,           \
		TOPIC,          \
		INVITE,         \
		KICK,           \
		PART,           \
		KILL,           \
		VERSION,        \
		INFO,           \
        ISON,           \
		QUIT};

void Server::deleteClient(int fd){
    VEC_ITER_USER_ADR it_begin = arr_user.begin();
    VEC_ITER_USER_ADR it_end = arr_user.end();
    while (it_begin != it_end)
    {
        if (fd == (*it_begin)->getFd()) {
			delete *it_begin;
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
        if (topic == (*it_begin)->getName()) {
            arr_channel.erase(it_begin);
            return;
        }
        it_begin++;
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

int Server::find_num_chan_by_name(std::string const &topic) {
    std::vector<Channel *>::iterator it_begin = arr_channel.begin();
    std::vector<Channel *>::iterator it_end = arr_channel.end();

    int i = 0;
    while (it_begin != it_end){
        if ((*it_begin)->getName() == topic)
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

int Server::many_or_solo_join(std::string const &arg)
{
    int pos = arg.find_first_of("#&");
    if (pos == -1)
        return (0);
    if ((pos = arg.find_first_of("#&", pos + 1)) == -1)
        return (1);
    else
        return (2);
    return (2);
}

// функция говорит есть ли у юзерa в списке данный канал
bool Server::can_user_talk_in_channel(int num, std::string &channel){
    std::vector<Channel *> meh;
    std::vector<Channel *>::iterator it_begin , it_end;

    meh = this->arr_user[num]->getVecChannel();
    it_begin = meh.begin();
    it_end = meh.end();
    while (it_begin != it_end){
        if ((*it_begin)->getName() == channel)
            return(true);
        it_begin++;
    }
    return(false);
}

// ############################################################################################################
                                 //команды реализация
                                 //PRIVMSG
//
void Server::privmisg_for_one_channel(int num, std::string &massage, std::string &channel){
    int pos = 0, num_channel = 0, nbytes = 0;
    std::string arg = "" ,msgcout = "";
    std::vector<User *>::iterator it_begin, it_end;

    if (can_user_talk_in_channel(num, channel) != true)
        return;
    pos = this->arr_user[num]->getMsgArgs().find_first_of(':');
    std::cout << "channel name =" << channel << "|massage =|" << massage << "|" << "\n";
    num_channel = find_num_chan_by_name(channel);
    it_begin = arr_channel[num_channel]->getUsersVector_red().begin();
    it_end = arr_channel[num_channel]->getUsersVector_red().end();
    arg = "#" + channel + " :" + massage;
    arr_user[num]->setMsgArgs(arg);
    while (it_begin != it_end){
        if ((*it_begin)->getNickname() != arr_user[num]->getNickname()) {
            std::string msg = MSG_PRIVMSG_CHANNEL;
            msgcout = msg.substr(0,(msg.length() - 2));
            std::cout << "|"<<  msgcout  << "|\n";
            nbytes = send((*it_begin)->getFd(), msg.c_str(), msg.length(), 0);
        }
        it_begin++;
    }
}

void Server::privmisg_for_one_person(int num,  std::string &name){
    int num_friend = 0, nbytes = 0;
    std ::string error = "", msg = "", msgcout = "";

    num_friend = find_num_by_nickname(name);
    if (num_friend < 0) {
        std::cout << "\x1b[31;1mНет такого челика\x1b[0m\n";
        error = MSG_NOSUCHNICK  + name + "\"\r\n";
        send(arr_user[num]->getFd(), error.c_str(), error.length(), 0);
        return;
    }
    if (num_friend == num)
        return;
    // отправляем сообщеньку по фд
    msg = MSG_PRIVMSG;
    nbytes = send(this->arr_user[num_friend]->getFd(), msg.c_str(), msg.length(), 0);
    std::cout << "Private massage: to "<< this->arr_user[num_friend]->getNickname()<< "->" << msg.c_str();
    if (nbytes < 0) {
        perror("Server: write failure");
    }
}

void Server::privmisg_work(int num) {
    int  pos = 0, pos_space = 0, pos_two = 0, pos_dot = 0, pos2 = 0;
    std::string name = "", massage = "", error = "" , msg = "", channel = "";
    std::vector<std::string> arr_channel_name;
    std::vector<std::string>::iterator it_begin, it_end;

    pos = this->arr_user[num]->getMsgArgs().find_first_of(' ');
    name = this->arr_user[num]->getMsgArgs().substr(0, pos);
    pos = this->arr_user[num]->getMsgArgs().find_first_of(':');
    pos_space = name.find_first_of(" ");
    massage = this->arr_user[num]->getMsgArgs().substr(pos + 1);

    //удаление лишних пробелов в начале строки
    while (massage.find_first_not_of(" ") > 0)
        massage.erase(0, 1);
    pos = this->arr_user[num]->getMsgArgs().find_first_of("#&") ;
    pos_two = this->arr_user[num]->getMsgArgs().find_first_of("#&", pos + 1);
    pos_dot  = name.find_first_of(",");
    //много каналов
    if (pos_two != -1 && pos_dot + 1 == pos_two)
    {
        arr_channel_name.push_back(name.substr(pos + 1, pos_two - 2));
        pos = pos_two;
        while ((pos_two = name.find_first_of("#&", pos +1)) != -1){
            arr_channel_name.push_back(name.substr(pos + 1, pos_two - (pos + 2)));
            pos = pos_two;
        }
        arr_channel_name.push_back(name.substr(pos + 1, name.length() - (pos + 1)));
        it_begin = arr_channel_name.begin();
        it_end = arr_channel_name.end();
        while (it_begin != it_end) {
            if (can_user_talk_in_channel(num, (*it_begin)) == true) {//нужно проверить состоит ли юзер в канале
                privmisg_for_one_channel(num, massage, (*it_begin));
            }
            it_begin++;
        }
    }//если сообщения для одного канала
    else if (pos != -1) {
        name = name.substr(1, name.length() - 1);
        if (can_user_talk_in_channel(num, name) == true) {
            pos2 = this->arr_user[num]->getMsgArgs().find_first_of(' ');
            channel = arr_user[num]->getMsgArgs().substr(pos + 1, pos2 - 1);
            privmisg_for_one_channel(num, massage, channel);
        }else{
            msg = MSG_CANNOTSENDTOCHAN;
            send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
        }
    }else {//если сообщение персоне или ссписку персон
        if (name.find_first_of(",") != std::string::npos || (name.find_first_of(",") > (unsigned long)pos_space )) {
            std::vector<std::string> arr_name;

            name = this->arr_user[num]->getMsgArgs();
            pos = name.find_first_of(",");
            pos_space = name.find_first_of(" ");
            pos_two = name.find_first_of(",", pos + 1);
            arr_name.push_back(name.substr(0, pos));
            if (pos_two > pos_space || pos_two == -1) {
                arr_name.push_back(name.substr(pos + 1, pos_space - (pos + 1)));
            } else {
                while (pos_two < pos_space && pos_two != -1) {
                    arr_name.push_back(name.substr(pos + 1, pos_two - (pos + 1)));
                    pos = pos_two;
                    pos_two = name.find_first_of(",", pos + 1);
                }
                arr_name.push_back(name.substr(pos + 1, pos_space - (pos + 1)));
            }

            it_begin = arr_name.begin();
            it_end = arr_name.end();
            while (it_begin != it_end)
            {
                privmisg_for_one_person(num, (*it_begin));
                it_begin++;
            }
        }else { //если приватка отправляется одной персоне
            privmisg_for_one_person(num, name);
        }
    }
}
                                        //JOIN
//
void Server::say_hello_to_new_in_channel(int num, std::vector<Channel *>::iterator &it_b_channel, std::string topic){
    //отправляем что он в канале
    std::string msg = MSG_ACCESS_JOIN, msgcout = "";
    msgcout = msg.substr(0,(msg.length() - 2));
    std::cout << "|"<<  msgcout  << "|\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
    // отправляем всех кто есть на канале новичку
    std::vector<User *>::iterator it_begin = (*it_b_channel)->getUsersVector_red().begin();
    std::vector<User *>::iterator it_end = (*it_b_channel)->getUsersVector_red().end();

    //3 сообщение
    if ((*it_b_channel)->getTopic() == "") {
        msg = MSG_HELLO_AND_JOIN;
        msgcout = msg.substr(0,(msg.length() - 2));
        std::cout << "|"<<  msgcout  << "|\n";
        send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
    }
    else{
        msg = MSG_HELLO_AND_JOIN_THITH_TOPIC;
        msgcout = msg.substr(0,(msg.length() - 2));
        std::cout << "|"<<  msgcout  << "|\n";
        send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
    }
    //4а сообщение
    msg = MSG_LIST_USER_IN_CHANELL;
    while (it_begin != it_end) {
        if (isOper((*it_begin),  (*it_b_channel)) == true)
            msg += "@" + (*it_begin)->getNickname() + " ";
        else
            msg += (*it_begin)->getNickname() + " ";
        it_begin++;
    }
    msg.erase(msg.length() - 1, 1);
    msg += "\r\n";
    msgcout = msg.substr(0,(msg.length() - 2));
    std::cout << "|"<<  msgcout  << "|\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
    //4b
    msg = MSG_END_OF_USER_LIST;
    msgcout = msg.substr(0,(msg.length() - 2));
    std::cout << "|"<<  msgcout  << "|\n";
    send(arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
}

void       Server::parser_of_join_chanel(std::string &arg, std::vector<std::string> &name_chan){
    int pos_til = arg.find_first_of("#&");
    int pos_dot = arg.find_first_of(",");
    int pos_space = arg.find_first_of(" ");

    while (pos_til != -1 && pos_dot != -1) {
        if (pos_space != -1 && pos_space < pos_dot)
            pos_dot = pos_space;
        name_chan.push_back(arg.substr(pos_til, pos_dot - (pos_til)));
        pos_til = arg.find_first_of("#&", pos_til + 1);
        pos_dot = arg.find_first_of(",", pos_dot + 1);
    }

    if (pos_til != -1 && pos_dot == -1)
        name_chan.push_back(arg.substr(pos_til, pos_space - (pos_til)));
}

//1) если после пробела нет запятых
//2 ) там одна запятая
//3) там много запятых
void Server::parser_of_join_chanel_key(std::string &arg, std::vector<std::string> &key_chan){
    int pos_space = 0, pos_dot = 0, pos_dot_two = 0;

    pos_space = arg.find_first_of(" ");
    if (pos_space != -1) {
        pos_dot = arg.find_first_of(",", pos_space);

        if (pos_dot == -1){
            key_chan.push_back(arg.substr(pos_space + 1, arg.length() - (pos_space + 1)));
            return ;
        }
        key_chan.push_back(arg.substr(pos_space + 1, pos_dot - (pos_space + 1)));
        pos_dot_two = arg.find_first_of(",", pos_dot + 1);
        if (pos_dot_two == -1) {
            key_chan.push_back(arg.substr(pos_dot + 1, pos_dot_two - (pos_dot + 1)));
            return ;
        }
        while (pos_dot_two != -1) {
            key_chan.push_back(arg.substr(pos_dot + 1, pos_dot_two - (pos_dot + 1)));
            pos_dot = pos_dot_two;
            pos_dot_two = arg.find_first_of(",", pos_dot + 1);
        }
        key_chan.push_back(arg.substr(pos_dot + 1, arg.length() - (pos_dot + 1)));
    }
    return ;
}

//создаем канал, заполняем topic, пароль если есть, опера , и юзера в список, вносим канал в список сервера
void    Server::create_new_channel(int num, std::string &key, std::string &topic){
    std::vector<Channel *>::iterator it_b_channel, it_e_channel;
    int i = 0;

    Channel *channel = new Channel(topic);
    if (!key.empty())
        channel->setPassword(key);
    channel->setOper(arr_user[num]);
    channel->addUser(arr_user[num]);
    setChannel(*channel);
    //закидываем канал в список юзера;
    this->arr_user[num]->setVecChannel(*channel);
    it_b_channel = this->arr_channel.begin();
    it_e_channel = this->arr_channel.end();
    i = find_num_chan_by_name(channel->getName());
    it_b_channel += i;
    say_hello_to_new_in_channel(num, it_b_channel, topic);
    std::cout <<"\x1b[32;1m Create a channel :|\x1b[0m" << topic <<  "\x1b[32;1m|\x1b[0m\n";
}

int     Server::check_how_many_channel_have_user(int num){
    if (this->arr_user[num]->getVecChannel().empty() == true)
        return(0);

    int i = 0;
    std::vector<Channel *>::iterator it_begin, it_end;

    it_begin = this->arr_user[num]->getVecChannel().begin();
    it_end = this->arr_user[num]->getVecChannel().end();
    while(it_begin != it_end){
        it_begin++;
        i++;
    }
   return(i);
}

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
    std::vector<Channel *>::iterator it_b_channel, it_e_channel, it_b_u_channel, it_e_u_channel;
    std::vector<User *>::iterator it_begin, it_end;
    std::string msg = "", error = "", topic = "", key = "", limit = "";
    int pos = 0, pos2 = 0, nbytes = 0;

    msg = this->arr_user[num]->getMsgArgs();
    pos = msg.find_first_of(" ");
    it_b_channel = this->arr_channel.begin();
    it_e_channel = this->arr_channel.end();
    if (check_how_many_channel_have_user(num) <= 9) {
        if ((pos2 = msg.find_first_of(" ", pos + 1)) != -1) {
            error = MSG_NEEDMOREPARAMS;
            send(arr_user[num]->getFd(), error.c_str(), error.length(), 0);
            return;
        }
        topic = msg.substr(1, pos - 1);
        std::cout << topic <<"\n";
        if (check_invite_join(num, topic) == false)
            return;
        if (pos != -1)
            key = msg.substr(pos + 1, msg.length() - (pos + 1));
        //2*
        if (this->arr_channel.empty())
            create_new_channel(num, key, topic);
        else {//3*
            while (it_b_channel != it_e_channel) {
                if ((*it_b_channel)->getName() == topic)//3.1
                {
                    it_b_u_channel = this->arr_user[num]->getVecChannel().begin();
                    it_e_u_channel = this->arr_user[num]->getVecChannel().end();
                    while (it_b_u_channel != it_e_u_channel) {
                        if ((*it_b_u_channel)->getName() ==
                            topic)//3.1.1.1
                            return;
                        it_b_u_channel++;
                    }
                    int limit_int = (*it_b_channel)->getModeParams()->limit, size = (*it_b_channel)->getUsersVector().size();
                    if (limit_int != -1 &&  size >= limit_int) {
                        limit += std::to_string((*it_b_channel)->getModeParams()->limit);
                        errPrint(arr_user[num]->getFd(), MSG_CHANNELISFULL);
                        return;
                    }
                    if ((*it_b_channel)->getPassword() == "" || (*it_b_channel)->getPassword() == key) {
                        //3.1.1.2  отправляем всем кто присоединился
                        this->arr_user[num]->setVecChannel(*(*it_b_channel));
                        (*it_b_channel)->addUser(arr_user[num]);
                        it_begin = (*it_b_channel)->getUsersVector_red().begin();
                        it_end = (*it_b_channel)->getUsersVector_red().end();
                        std::string msg = MSG_ACCESS_JOIN;
                        while (it_begin != it_end) { //2 второе сообщение после JOIN #channel
                            nbytes = send((*it_begin)->getFd(), msg.c_str(), msg.length(), 0);
                            std::cout << "Private massage: to " << (*it_begin)->getNickname() << "->" << msg.c_str();
                            it_begin++;
                        }
                        say_hello_to_new_in_channel(num, it_b_channel, topic);
                        break;
                    } else { // 3.1.1.2.2
                        error = MSG_BADCHANNELKEY;
                        send(arr_user[num]->getFd(), error.c_str(), error.length(), 0);
                        std::cout << "\x1b[31;1mLebowski where is the key ?!"
                                     " You need a password to enter the channel!\x1b[0m\n";
                        return;
                    }
                }
                it_b_channel++;
            }
            if (it_b_channel == it_e_channel)// точная копия 2*  3.2
                create_new_channel(num, key, topic);
        }
    } else
        errPrint(arr_user[num]->getFd(), MSG_TOOMANYCHANNELS);
}

bool Server::find_user_by_name_in_invited(std::string &name, int num_channel)
{
    std::vector<User *>::iterator it_begin, it_end;

    it_begin = arr_channel[num_channel]->getInvitedVector().begin();
    it_end = arr_channel[num_channel]->getInvitedVector().end();
    while (it_begin != it_end){
        if((*it_begin)->getNickname() == name)
            return(true);
        it_begin++;
    }
    return(false);
}

bool Server::check_invite_join(int num,std::string &name_channel){
    std::string name ="", send_msg = "";
    int num_channel = 0;

    num_channel = find_num_chan_by_name(name_channel);
    if (num_channel >= 0){
        bool i_flag = arr_channel[num_channel]->getModeParams()->i;
        name = arr_user[num]->getNickname();
        if (i_flag == true)
            if (find_user_by_name_in_invited(name, num_channel) != true) {
                send_msg = MSG_INVITEONLYCHAN;
                send(arr_user[num]->getFd(), send_msg.c_str(), send_msg.length(), 0);
                return(false);
            }
    }
    return(true);
}

void Server::join_pre_work(int num){
    std::string command = "", arg = "", send_msg = "", valid_buf = "", name = "";
    int  i = 0;
    std::vector<std::string> name_channel, key_channel;
    std::vector<std::string>::iterator it_begin, it_begin_key, end_begin_key;

    if (many_or_solo_join(arr_user[num]->getMsgArgs()) == 2) {
        parser_of_join_chanel(arr_user[num]->getMsgArgs(), name_channel);
        parser_of_join_chanel_key(arr_user[num]->getMsgArgs(), key_channel);
        i = name_channel.size();
        it_begin = name_channel.begin();
        it_begin_key = key_channel.begin();
        end_begin_key = key_channel.end();
        while (i > 0) {
            if (key_channel.size() == 0 || end_begin_key == it_begin_key)
                valid_buf = (*it_begin);
            else
                valid_buf = (*it_begin) + " " + (*it_begin_key);
            arr_user[num]->setMsgArgs(valid_buf);
            join_work(num);
            i--;
            it_begin++;
            if (it_begin_key != end_begin_key)
                it_begin_key++;
        }
        name_channel.clear();
        key_channel.clear();
    }
    else if (many_or_solo_join(arr_user[num]->getMsgArgs()) == 1)
        join_work(num);
    else
        std::cout << "JOIN #namechanel\n";
}

                                    //USER
//
void    Server::user_work(std::string &arg, int num)
{
    int pos = 0, pos2 = 0;
    std::string user_name = "", host_name = "", server_name = "", real_name = "";

    pos = arg.find_first_of(" ");
    user_name = arg.substr(0, pos);
    pos2 = arg.find(" ", pos + 1);
    host_name = arg.substr(pos + 1, pos2 - pos - 1);
    pos = pos2;
    pos2 = arg.find(" ", pos + 1);
    server_name = arg.substr(pos + 1, pos2 - pos - 1);
    pos = arg.find_first_of(":");
    real_name = arg.substr(pos + 1, arg.length() - (pos + 1));
    pos = real_name.find_first_of(" ");
    while (pos != 0 && pos != -1 ) {
        real_name.erase(0, 1);
        pos = real_name.find_first_of(" ");
    }
    this->arr_user[num]->setUsername(user_name);
    this->arr_user[num]->setHostname(host_name);
    this->arr_user[num]->setServername(server_name);
    this->arr_user[num]->setRealname(real_name);
    std::cout << "\x1b[32;1mOK useraname\x1b[0m\n";
}
                                //ISON 638
//
void        Server::ison_work(int num)
{
    int pos_sp = 0, pos_sec_sp = 0, i = 0;
    std::string msg = "", reply = MSG_RPL_ISON, name_in_base = "", nick_name = "";
    std::vector<std::string> arr_nickname;
    std::vector<std::string>::iterator mimic_begin, mimc_end;
    std::vector<User *>::iterator it_begin, it_end;

    msg = arr_user[num]->getMsgArgs();
    pos_sp = msg.find_first_of(" ");
    if (pos_sp != -1){
        if(pos_sp != pos_sec_sp - 1) {
            arr_nickname.push_back(msg.substr(0, pos_sp));
            pos_sec_sp = msg.find_first_of(" ", pos_sp + 1);
            while (pos_sec_sp != -1) {
                arr_nickname.push_back(msg.substr(pos_sp + 1, pos_sec_sp - (pos_sp + 1)));
                pos_sp = pos_sec_sp;
                pos_sec_sp = msg.find_first_of(" ", pos_sp + 1);
                if(pos_sp == pos_sec_sp - 1)
                    break;
            }
            if(pos_sp != pos_sec_sp - 1)
                arr_nickname.push_back(msg.substr(pos_sp + 1, msg.length() - (pos_sp + 1)));
        }
    }else
        arr_nickname.push_back(msg.substr(pos_sp + 1, msg.length() - (pos_sp + 1)));

    it_begin = this->getUserVector().begin();
    it_end = this->getUserVector().end();
    mimic_begin = arr_nickname.begin();
    mimc_end = arr_nickname.end();
    while (mimic_begin != mimc_end){
        while(it_begin != it_end) {
            nick_name = (*mimic_begin);
            name_in_base = (*it_begin)->getNickname();
            if (nick_name == name_in_base) {
                if (i > 0)
                    reply += " ";
                reply += (*mimic_begin);
                i++;
                break;
            }
            it_begin++;
        }
        it_begin = this->getUserVector().begin();
        mimic_begin++;
    }
    reply += "\r\n";
    std::cout << "\x1b[32;1mUser\x1b[0m \""<< arr_user[num]->getNickname()
        << "\"\x1b[32;1m Send ISON. Reply is \x1b[0m" << reply ;
    send(arr_user[num]->getFd(), reply.c_str(), reply.length(), 0);
}

// ############################################################################################################
                                    // Парсер
//

int Server::name_verification(std::string &buf)
{
	if (!isNickCorrect(buf))
	{
		std::cout << "\x1b[31;1mThis name is incorrect(not more 9 symbols: letters,numbers)\x1b[0m\n";
        return (-1);
	}
    std::vector<User *>::iterator it_begin = arr_user.begin();
    std::vector<User *>::iterator it_end = arr_user.end();
    int fd;
    if (it_begin != it_end){  // идем по массиву указателей клиентов с помощью итераторов
        fd = (*it_begin)->getFd();  // вытаскием по адресу определенного клиента его фд
        while (it_begin != it_end){
            fd = (*it_begin)->getFd();
            if (buf == (*it_begin)->getNickname() || buf == "" ){
                std::cout << "\x1b[31;1mThis name is taken\x1b[0m\n";
                return (-1);
            }
            it_begin++;
        }
    }
    return(1);
}

int Server::password_verification(std::string &buf, int num){
    if (buf != this->getPassword(0) ) {
        std::cout << "\x1b[31;1mWrong password\x1b[0m\n";
        return (-1);
    }else{
            std::cout << "\x1b[32;1mOK password\x1b[0m\n";
            num = find_numb_iter(num);
        this->arr_user[num]->setPassword_init(true);
    }
    return (0);
}

//первая часть парсераа проверяет пароль , ник , и юзер команды
void Server::parser_check_pas_nick_user(int num, int fd) {
    std::string send_msg = "";

    if (this->arr_user[num]->getPassword_init() == false) {
        if (arr_user[num]->getMsgCom() == "PASS") {// проверяем ввел ли сейчас он корректный пароль
            if (password_verification(arr_user[num]->getMsgArgs(), fd) == -1)
                return;
        } else {
            std::cout << "\x1b[31;1mFirst write PASS_WORD\x1b[0m\n";
            return;
        }
        return;
    }
    if (this->arr_user[num]->getName_init() == false) {
        if (arr_user[num]->getMsgCom() == "NICK") {
            if (name_verification(arr_user[num]->getMsgArgs()) == -1)//проверяем вводил ли он не занятый ник
                return;
            else {
                this->arr_user[num]->setName_init(true);
                this->arr_user[num]->setNickname(arr_user[num]->getMsgArgs()); // вносим в объект имя
                std::cout << "\x1b[32;1mOK nickname\x1b[0m\n";
            }
        }
        return;
    }
    if (arr_user[num]->getMsgCom() == "USER") {
		std::string	args = arr_user[num]->getMsgArgs();
        user_work(args, num);
        send_msg = MSG_WEL_COME_DEFAULT;
        send(fd, send_msg.c_str(), send_msg.length(), 0);
        this->arr_user[num]->setAccess(true);
       return;
    }
    if (this->arr_user[num]->getPassword_init() == false)
        std::cout << "\x1b[31;1mNO password NO\x1b[0m\n";
    else if (this->arr_user[num]->getName_init() == false)
        std::cout << "\x1b[31;1mNO nickname NO\x1b[0m\n";
    else
        std::cout << "\x1b[31;1mNO username NO\x1b[0m\n";
    return;
}

//вторая часть парсера отвечает за выполнение команд
void Server::parser_switch(int num ,int fd, fd_set &writefds){
    std::map<std::string, forms> map_forms;
    map_forms["NICK"] = NICK;
    map_forms["ISON"] = ISON;
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
	map_forms["QUIT"] = QUIT;

    switch (map_forms[arr_user[num]->getMsgCom()]) {
        case NICK:
            nick(num, arr_user[num]->getMsgArgs());// EPILAR
            break;
        case ISON:
            ison_work(num);
            break;
        case USER:
            user_work(arr_user[num]->getMsgArgs(), num);
            break;
        case OPER:
            break;
        case PRIVMSG:
            privmisg_work(num);
            break;
        case NOTICE:
            privmisg_work(num);
            break;
        case JOIN:
            join_pre_work(num);
            break;
        case MODE:
            mode_chan(num);
            break;
        case TOPIC:
            topic(num);
            break;
        case INVITE:
            invite(num);
            break;
        case KICK:
            kick(num);
            break;
        case PART:
            part(num, arr_user[num]->getMsgArgs(), false, fd);
            break;
        case KILL: // предлагаю исключить
            break;
        case VERSION:
            version(num, arr_user[num]->getMsgArgs());// EPILAR
            break;
        case INFO:
            info(num, arr_user[num]->getMsgArgs());// EPILAR
            break;
		case QUIT:
			quit(num, arr_user[num]->getMsgArgs());// EPILAR
			break;
        default:
            FD_SET(fd, &writefds);
            std::cout << "\x1b[31;1mWe do nothing\x1b[0m\n";
            break;
    }
}


void Server::parser(int num , std::string buf_str, int fd, fd_set &writefds) {
    arr_user[num]->make_msg(buf_str);
    if (getAccess(fd) != true)
        parser_check_pas_nick_user(num,  fd);
    else if (getAccess(fd) == true)
        parser_switch(num, fd, writefds);
}

// ############################################################################################################
                                                //разбивка основного цикла
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
        FD_SET(fd, &activfds);// добавляем новичка к активным портам которые мы будем слушать
        User *arr_user = new User(fd); // создаем для этого клиента объект  класса со своим блэк-джеком и fd
        this->setUser(*arr_user); // запоминаем его адрес в массив указателей
    }
}

// ловим что написал старый клиент , проверяем на доступ и распределяем, что с ним делать.
int Server::get_old_client_massage(int &fd, fd_set &activfds, fd_set &writefds, char **buf){
    int num = 0, nbytes = 0;
    std::string buf_str = "", buf_str_bad = "", new_args = "", error = "";

    if (FD_ISSET(fd, &activfds)) {//если фд есть в списке акитвистов то начнем с ним работать
        num = find_numb_iter(fd);// найдем порядковый номер этого клиента в массиве
        nbytes = recv(fd, *buf, 512, 0);// прочтем в массив чаров его сообщение (не обробатывал переполнение буфера)
                                        // std::cout << "\n\x1b[37;1mrecv catch :\x1b[0m" << *buf;
        buf_str_bad = *buf;
        if ((buf_str_bad.find_first_of("\n") == std::string::npos) || (arr_user[num]->getMsgFrom() != "")) {
            arr_user[num]->setFullMassage(false);
            new_args = arr_user[num]->getMsgFrom() + buf_str_bad;
            arr_user[num]->setMsgFrom(new_args);
            if (buf_str_bad.find_first_of("\n") != std::string::npos) {
                arr_user[num]->setFullMassage(true);
                buf_str = arr_user[num]->getMsgFrom().substr(0, arr_user[num]->getMsgFrom().length() - 1);
            }
        } else {
            arr_user[num]->setFullMassage(true);
            size_t pos = buf_str_bad.find_first_of("\r\n");
            buf_str = buf_str_bad.substr(0, pos);
        }
        if (nbytes < 0) { perror("Server: meh nbytes < 0\n"); }
        else if (nbytes == 0) {
///////////////////

///////////////////
            quit_c(num, error, fd);
            perror("\x1b[31;1mServer: meh nbytes == 0, and delete Client byby =^_^=\x1b[0m\n");
        } else if (arr_user[num]->getFullMassage() == true) {
            std::cout << "\n\x1b[36;1mServ got massage: \x1b[0m|" << buf_str << "|\n";
            parser(num, buf_str, fd, writefds);
        }
        return (fd);
    }
    return 0;
}


// ############################################################################################################
                                                    //Основной цикл
//
void Server::work(int ls) {
    fd_set writefds, activfds;
    int fd = 0, work_fd = 0, res = 0, num = 0;
    int max_d = ls; // считаем что макс сокет это нынешний слушающий
    char *buf;

    for (;;) {
        buf = (char *) calloc(sizeof(char), BUFLEN + 1);
        FD_ZERO(&writefds);
        FD_ZERO(&activfds);
        FD_SET(ls, &activfds);//вводим fd слушашего сокета в множество
        // а теперь цикл по сокетам заносим все fd из списка клиентов в множество;
        create_many_active_fd(fd, activfds, max_d);
        res = select(FD_SETSIZE, &activfds, NULL, NULL, NULL);// проверяем на активность
        if (res < 1 && errno != EINTR) {
                perror("Server: cannot creat socket");
                exit(EXIT_FAILURE);
        }
        for (int i = 0; i <= max_d; i++) { //поочереди проверим все айди
            if (i == ls)  // если сигнал пришел со слышашего порта то это новое подключение!
                 get_new_client(ls, fd, activfds);
            else   // если с любого другого , то это уже старечки что то пишут и нужно посмотреть что там
                work_fd = get_old_client_massage(i, activfds, writefds, &buf);
        }
        if (FD_ISSET(work_fd, &activfds)) {
            num = find_numb_iter(work_fd);
            if (arr_user[num]->getFullMassage() == true)
                this->arr_user[num]->cleaner();
        }
        free(buf);
        usleep(1);
    }
}

int		Server::errPrint(const int fd, std::string msg) const
{
	send(fd, msg.c_str(), msg.length(), 0);
	return 1;
}

int		Server::rplPrint(const int fd, std::string msg) const
{
	send(fd, msg.c_str(), msg.length(), 0);
	return 0;
}

void	Server::sendToChanUsers(std::string msg, Channel *chan)
{
	std::vector<User *>::const_iterator	first = chan->getUsersVector().begin();
	std::vector<User *>::const_iterator	last = chan->getUsersVector().end();
	for (; first != last; ++first)
		send((*first)->getFd(), msg.c_str(), msg.length(), 0);
}

std::string					Server::fillModes(std::string msg, ModeChan *flags)
{
	msg += " +";
	if (flags->p == 1)
		msg += "p";
	if (flags->s == 1)
		msg += "s";
	if (flags->i == 1)
		msg += "i";
	if (flags->t == 1)
		msg += "t";
	if (flags->n == 1)
		msg += "n";
	if (flags->m == 1)
		msg += "m";
	msg += "\r\n";
	return msg;
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

std::vector<std::string>	Server::splitStr(std::string str, std::string delimiter)
{
	std::vector<std::string>	res;
	size_t pos = 0;
	std::string tmp;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		tmp = str.substr(0, pos);
		res.push_back(tmp); //action
		str.erase(0, pos + delimiter.length());
	}
	tmp = str.substr(0, pos);
	res.push_back(tmp); //action
	return res;
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

bool    Server::isOper(User *usr, Channel *chan)
{
    if (usr == chan->getOperModer())
        return true;
	std::vector<User *> tmp = chan->getOpersVector();
	std::vector<User *>::iterator	first = tmp.begin();
	std::vector<User *>::iterator	last = tmp.end();
	for (; first != last; ++first)
	{
		if (usr == *first)
			return true;
	}
    return false;
}

User*		Server::findUser(std::string str)
{
	for (int i = 0; (unsigned long)i < arr_user.size(); ++i)
	{
		if (arr_user[i]->getNickname() == str)
			return arr_user[i];
	}
	return NULL;
}

int		Server::part(int num, std::string& arguments, bool it_ctrl_c, int fd)
{
	std::vector<std::string> args = splitStr(arguments);
	std::cout << this->arr_user[num]->getMsgArgs() << "***" << args.size() << std::endl;
	if (this->arr_user[num]->getMsgArgs() == "") //проверка нет аргументов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NEEDMOREPARAMS));
	std::vector<std::string> exitChans = splitStr(args[0], ",");
	for(size_t i = 0; i < exitChans.size(); ++i)
	{
		if (is_chan(exitChans[i]) == false) //проверка: не канал (exitChans[i] - храниться имя канала)
			return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
		(exitChans[i]).erase(0,1); // удаляем символ #/&
		if (chan_in_list(exitChans[i], arr_channel) == false) //проверка: нет в списке каналов
			return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
		Channel *cur_chan = find_chan(exitChans[i]);
		if (cur_chan->findUserByName(this->arr_user[num]->getNickname()) == NULL) //проверка: юзер не состоит в канале
			return (errPrint(this->arr_user[num]->getFd(), MSG_NOTONCHANNEL));
		else
		{
			std::string mg = ":" + this->arr_user[num]->getNickname() + \
			"!" + this->arr_user[num]->getUsername() + "@" + this->arr_user[num]->getHostname() + \
			" " + "MODE" + " #" + cur_chan->getName() + " +o ";
			if (this->arr_user[num] == cur_chan->getOperModer() && cur_chan->getUsersVector().size() > 1) //уходит модератороператор
			{
				if (cur_chan->getOpersVector().empty()) //there are no operusers
				{
					cur_chan->setOper(cur_chan->getUsersVector()[1]);
					mg += (cur_chan->getUsersVector()[1])->getNickname() + "\r\n";
					sendToChanUsers(mg, cur_chan);
				}
				else
				{
					cur_chan->setOper(cur_chan->getOpersVector()[0]);
					cur_chan->eraseOperUser(cur_chan->getOpersVector()[0]);
					mg += (cur_chan->getOpersVector()[0])->getNickname() + "\r\n";
					sendToChanUsers(mg, cur_chan);
				}
			}

			std::string	msg(MSG_PARTSUCCESS);// отправка уведомление всем пользователям
											//	канал
			std::vector<User *>::const_iterator itb = cur_chan->getUsersVector().begin();
			std::vector<User *>::const_iterator ite = cur_chan->getUsersVector().end();
			for (; itb != ite; ++itb) {
                if (it_ctrl_c == true){
                    if (fd != (*itb)->getFd()) {
                        rplPrint((*itb)->getFd(), msg);
                    }
                }else
                    rplPrint((*itb)->getFd(), msg);
            }

			cur_chan->eraseUser(this->arr_user[num]);
			cur_chan->eraseVoteUser(this->arr_user[num]);
			cur_chan->eraseOperUser(this->arr_user[num]);
			this->arr_user[num]->eraseChannel(cur_chan);
			if (cur_chan->getUsersVector().empty())
				deleteChannel(cur_chan->getName());
		}
	}
	return 0;
}

int 	Server::invite(int num) //добавить ответы
{
	std::vector<std::string> args = splitStr(this->arr_user[num]->getMsgArgs());
	std::cout << this->arr_user[num]->getMsgArgs() << "***" << args.size() << std::endl;
	if (this->arr_user[num]->getMsgArgs() == "" || args.size() == 1) //проверка нет аргументов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NEEDMOREPARAMS));
	if (is_chan(args[1]) == false) //проверка: не канал (args[1] - храниться имя канала)
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	(args[1]).erase(0,1); // удаляем символ #/&
	if (chan_in_list(args[1], arr_channel) == false) //проверка: нет в списке каналов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	Channel *cur_chan = find_chan(args[1]); //указатель на текущий канал
	User * user_speaking = cur_chan->findUserByName(this->arr_user[num]->getNickname());
	if (user_speaking == NULL) 	//проверка: говорящий юзер не состоит в канале
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOTONCHANNEL));
	if (!isOper(this->arr_user[num], cur_chan) && cur_chan->getModeParams()->i == 1 ) //проверка: не хватает прав
		return (errPrint(this->arr_user[num]->getFd(), MSG_CHANOPRIVSNEEDED));
	User * user_to_invite = findUser(args[0]);
	User * user_allready_in = cur_chan->findUserByName(args[0], cur_chan->getInvitedVector());
	if (user_to_invite == NULL) //проверка: не существует такого юзера
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHNICK + args[0] + "\"\r\n"));
	if (user_allready_in != NULL) //проверка: юзер уже в канале
		return (errPrint(this->arr_user[num]->getFd(), MSG_USERONCHANNEL));
	else
	{
		cur_chan->addInvitedUser(user_to_invite);
		std::string msg = ":" + this->arr_user[num]->getNickname() + \
	"!" + this->arr_user[num]->getUsername() + "@" + this->arr_user[num]->getHostname() + \
	" " + "INVITE" + " " + args[0] + " #" + cur_chan->getName() + "\r\n";
		std::cout << "current msg is:" << msg << std::endl;
		errPrint(this->arr_user[find_num_by_nickname(args[0])]->getFd(), msg);
	}
	return 0;
}

int		Server::kick(int num)
{
	std::vector<std::string> args = splitStr(this->arr_user[num]->getMsgArgs());
	std::cout << this->arr_user[num]->getMsgArgs() << "***" << args.size() << std::endl;
	if (this->arr_user[num]->getMsgArgs() == "" || args.size() == 1) //проверка нет аргументов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NEEDMOREPARAMS));
	if (is_chan(args[0]) == false) //проверка: не канал (args[0] - храниться имя канала)
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	(args[0]).erase(0,1); // удаляем символ #/&
	if (chan_in_list(args[0], arr_channel) == false) //проверка: нет в списке каналов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	Channel *cur_chan = find_chan(args[0]); //указатель на текущий канал
	User * user_speaking = cur_chan->findUserByName(this->arr_user[num]->getNickname());
	if (user_speaking == NULL) 	//проверка: говорящий юзер не состоит в канале
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOTONCHANNEL));
	if (args.size() == 1) //only channel name passed
		return (errPrint(this->arr_user[num]->getFd(), MSG_NEEDMOREPARAMS));
	else
	{
		if (!isOper(this->arr_user[num], cur_chan))//check if user is oper
            return (errPrint(this->arr_user[num]->getFd(), MSG_CHANOPRIVSNEEDED_KICK));
        if(user_speaking->getNickname() == args[1]) {// проверка что чел не пытается кикнуть сам себя
            return (errPrint(this->arr_user[num]->getFd(), MSG_CANNOTSENDTOCHAN_KICK));
        }
		User * user_to_kick = cur_chan->findUserByName(args[1]);
		User * user_to_ = cur_chan->getOperModer();
		if (user_to_kick == user_to_)
			return (errPrint(this->arr_user[num]->getFd(), MSG_CHANOPRIVSNEEDED_KICK));
		if (user_to_kick == NULL) //проверка: не существует такого юзера
			return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHNICK + args[1] + "\"\r\n"));
		else
		{
            std::string msg = MSG_PRIVMSG_CHANNEL;
            for (VEC_ITER_USER_ADR it = cur_chan->getUsersVector_red().begin();
            it != cur_chan->getUsersVector_red().end(); ++it) {
                send((*it)->getFd(), msg.c_str(), msg.length(), 0);
            }
			cur_chan->eraseUser(user_to_kick);
			cur_chan->eraseVoteUser(user_to_kick);
			cur_chan->eraseOperUser(user_to_kick);
			cur_chan->eraseInvitedUser(user_to_kick);
            user_to_kick->eraseChannel(cur_chan);
		}
	}
	return 0;
}

int		Server::topic(int num)
{
	std::vector<std::string> args = splitStr(this->arr_user[num]->getMsgArgs());
	std::cout << this->arr_user[num]->getMsgArgs() << "***" << args.size() << std::endl;
	if (this->arr_user[num]->getMsgArgs() == "") //проверка нет аргументов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NEEDMOREPARAMS));
	if (is_chan(args[0]) == false) //проверка: не канал (args[0] - храниться имя канала)
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	(args[0]).erase(0,1); // удаляем символ #/&
	if (chan_in_list(args[0], arr_channel) == false) //проверка: нет в списке каналов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	Channel *cur_chan = find_chan(args[0]); //указатель на текущий канал
	User * user_speaking = cur_chan->findUserByName(this->arr_user[num]->getNickname());
	if (user_speaking == NULL) 	//проверка: говорящий юзер не состоит в кана
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOTONCHANNEL));
	if (args.size() == 1) //only channel name passed
    {
        if (cur_chan->getTopic() == "")
			return (rplPrint(this->arr_user[num]->getFd(), MSG_NOTOPIC));
		return (rplPrint(this->arr_user[num]->getFd(), MSG_TOPIC));
    }
    else
    {
		if (!isOper(this->arr_user[num], cur_chan) && cur_chan->getModeParams()->t == 1 ) //check if user is oper
			return (errPrint(this->arr_user[num]->getFd(), MSG_CHANOPRIVSNEEDED));
        if ((args[1])[0] != ':')
			return (errPrint(this->arr_user[num]->getFd(), MSG_NEEDMOREPARAMS));
        (args[1]).erase(0,1);
        cur_chan->setTopic(args[1]);
		sendToChanUsers(MSG_TOPIC, cur_chan);
    }
	return 0;
}

int		Server::mode_chan(int num)
{
	std::cout << "arr_channel.size()=" << arr_channel.size() << std::endl;

	std::vector<std::string> args = splitStr(this->arr_user[num]->getMsgArgs());
	std::cout << this->arr_user[num]->getMsgArgs() << "***" << args.size() << std::endl;
	if (args.size() == 1)
		return 0;
	if (is_chan(args[0]) == false) //проверка: не канал (args[0] - храниться имя канала)
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	(args[0]).erase(0,1); // удаляем символ #/&
	if (chan_in_list(args[0], arr_channel) == false) //проверка: нет в списке каналов
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOSUCHCHANNEL));
	Channel *cur_chan = find_chan(args[0]); //указатель на текущий канал
	User * user_speaking = cur_chan->findUserByName(this->arr_user[num]->getNickname());
	if (user_speaking == NULL) 	//проверка: говорящий юзер не состоит в кана
		return (errPrint(this->arr_user[num]->getFd(), MSG_NOTONCHANNEL));
	if (!isOper(this->arr_user[num], cur_chan)) //check if user is oper
		return (errPrint(this->arr_user[num]->getFd(), MSG_CHANOPRIVSNEEDED));
	std::string msg = ":" + this->arr_user[num]->getNickname() + \
	"!" + this->arr_user[num]->getUsername() + "@" + this->arr_user[num]->getHostname() + \
	" " + "MODE" + " #" + cur_chan->getName();
	if ((args[1])[0] == '+') //флаги в true
	{
		(args[1]).erase(0,1);
		std::size_t found = (args[1]).find_first_not_of("opsitnmlvk");
		if (found!=std::string::npos)
			return (errPrint(this->arr_user[num]->getFd(), MSG_UMODEUNKNOWNFLAG));
		if (args.size() > 2)
			msg = cur_chan->setParamTrue(args[1], args[2], msg);
		else
			msg = cur_chan->setParamTrue(args[1], msg);
		if (msg == "MSG_NOSUCHNICK\r\n")
			return (errPrint(this->arr_user[num]->getFd(), (MSG_NOSUCHNICK + args[2] + "\"\r\n")));
		sendToChanUsers(msg, cur_chan);
	}
	if ((args[1])[0] == '-') //флаги в false
	{
		(args[1]).erase(0,1);
		std::size_t found = (args[1]).find_first_not_of("psitnm");
		if (found!=std::string::npos)
			return (errPrint(this->arr_user[num]->getFd(), MSG_UMODEUNKNOWNFLAG));
		msg = cur_chan->setParamFalse(args[1], msg);
		sendToChanUsers(msg, cur_chan);
	}
	std::string msg_final = MSG_CHANNELMODEIS;
	msg_final = fillModes(msg_final, cur_chan->getModeParams());
	sendToChanUsers(msg_final, cur_chan);
	return 0;
}

bool	Server::isNickUsed(const std::string& nickname)
{
	std::vector<User *>::iterator	it_begin = arr_user.begin();
	std::vector<User *>::iterator	it_end = arr_user.end();
	while (it_begin != it_end)
	{
		if ((*it_begin)->getNickname() == nickname)
			return true;
		it_begin++;
	}
	return false;
}

bool	Server::isLettersNumbers(std::string& nick)
{
	for (size_t i = 0; i < nick.length(); ++i)
	{
		if (!isalnum(nick[i]))
			return false;
	}
	return true;
}

bool	Server::isNickCorrect(std::string& nick)
{
	return (nick.length() <= 9 && isLettersNumbers(nick));
}

int		Server::nick(int num, std::string& args)
{
	if (args.empty())
	{
		std::string	msg(MSG_NONICKNAME);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 1;
	}
	std::string	nickname(splitStr(args)[0]); // вычленяем сам никнэйм из строки аргументов команды
	if (nickname.empty())
	{
		std::string	msg(MSG_NONICKNAME);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 1;
	}
	else if (!isNickCorrect(nickname))
	{
		std::string	msg(MSG_ERRNICKNAME);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 1;
	}
	else if (!Server::isNickUsed(nickname))
	{
		std::string	msg(MSG_NICKCHANGED);
		arr_user[num]->setNickname(nickname);

		std::vector<User *>::iterator	it_begin = arr_user.begin();
		std::vector<User *>::iterator	it_end = arr_user.end();
		for (; it_begin != it_end; it_begin++)
			send((*it_begin)->getFd(), msg.c_str(), msg.length(), 0);
	}
	else
	{
		std::string	msg(MSG_NICKNAMEINUSE);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 1;
	}
	return 0;
}

int		Server::version(int num, std::string& args)
{
	if (args.empty())
	{
		std::string	msg(MSG_SERVERVERSION);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 0;
	}
	std::string	servername(splitStr(args)[0]);
	if (this->getHost() != servername)
	{
		std::string	msg(MSG_NOSUCHSERVER);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 1;
	}
	else
	{
		std::string	msg(MSG_SERVERVERSION);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 0;
	}
	return 0;
}

std::vector<std::string>	Server::getSrvStat(void)
{
	struct stat					all_info;
	std::vector<std::string>	ret;
	char*						clock_create;
	char*						clock_modif;

	if (stat(PATH_TO_SERVER_FILE, &all_info) == -1)
	{
		ret.push_back("");
		ret.push_back("");
		return ret;
	}
	clock_create = ctime(&all_info.st_ctimespec.tv_sec);
	clock_modif = ctime(&all_info.st_mtimespec.tv_sec);
	ret.push_back(clock_create);
	ret.push_back(clock_modif);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//                              LIBFT ITOA                              //
//////////////////////////////////////////////////////////////////////////
static unsigned int	get_sym_num(long nl, int sign)
{
	unsigned int	i;

	i = 0;
	if (nl == 0)
		return (1);
	while (nl > 0)
	{
		nl /= 10;
		i++;
	}
	if (sign == -1)
		i++;
	return (i);
}

static void	converter(char *s, long nl, unsigned int sym_num, int sign)
{
	s[sym_num] = '\0';
	sym_num--;
	if (nl == 0)
		s[0] = '0';
	while (nl > 0)
	{
		s[sym_num] = nl % 10 + '0';
		sym_num--;
		nl /= 10;
	}
	if (sign == -1)
		s[0] = '-';
}

char	*ft_itoa(int n)
{
	char			*s;
	long			nl;
	unsigned int	sym_num;
	int				sign;

	sign = 1;
	if (n < 0)
	{
		nl = (long)n * -1;
		sign = -1;
	}
	else
		nl = (long)n;
	sym_num = get_sym_num(nl, sign);
	s = (char *)malloc(sizeof(char) * (sym_num + 1));
	if (s == NULL)
		return (NULL);
	converter(s, nl, sym_num, sign);
	return (s);
}
//////////////////////////////////////////////////////////////////////////

std::string		Server::getTimeElapsed()
{
	time_t	current_time = time(NULL);

	double	elapsed = difftime(current_time, this->start_time);
	int		minutes = floor(elapsed / 60);
	int 	seconds = static_cast<int>(elapsed - minutes * 60);

	char	*min_str = ft_itoa(minutes);
	char	*sec_str = ft_itoa(seconds);

	std::string	min(min_str);
	std::string	sec(sec_str);
	std::string	ret(min + " minutes, " + sec + " seconds.");

	free(min_str);
	free(sec_str);
	return ret;
}

int				Server::info(int num, std::string& args)
{
	if (args.empty())
	{
		std::vector<std::string>	statistic = getSrvStat();
		std::string					time_elapsed(getTimeElapsed());
		std::string					msg_info(MSG_SERVERINFO);
		std::string					msg_endinfo(MSG_ENDOFINFO);

		send(this->arr_user[num]->getFd(), msg_info.c_str(), msg_info.length(), 0);
		send(this->arr_user[num]->getFd(), msg_endinfo.c_str(), msg_endinfo.length(), 0);
		return 0;
	}
	std::string	servername(splitStr(args)[0]);
	if (this->getHost() != servername)
	{
		std::string	msg(MSG_NOSUCHSERVER);
		send(this->arr_user[num]->getFd(), msg.c_str(), msg.length(), 0);
		return 1;
	}
	else
	{
		std::vector<std::string>	statistic = getSrvStat();
		std::string					time_elapsed(getTimeElapsed());
		std::string					msg_info(MSG_SERVERINFO);
		std::string					msg_endinfo(MSG_ENDOFINFO);

		send(this->arr_user[num]->getFd(), msg_info.c_str(), msg_info.length(), 0);
		send(this->arr_user[num]->getFd(), msg_endinfo.c_str(), msg_endinfo.length(), 0);
		return 0;
	}
	return 0;
}

int		Server::quit_c(int num, std::string& args, int fd)
{
    // разослать уведомление о QUIT всем остальным пользователям канала
    // удалить пользователя из его каналов (PART)
    // закрыть fd
    // удалить пользователя

    std::string	quit_msg(args.length() ? args : this->arr_user[num]->getNickname());
    std::vector<Channel *>	userChannels = this->arr_user[num]->getVecChannel();
    std::string	part_arg;

    if (!userChannels.empty())
    {
        std::vector<Channel *>::iterator itb = userChannels.begin();
        std::vector<Channel *>::iterator ite = userChannels.end();
        for (; itb != ite; ++itb)
        {
            part_arg += "#";
            part_arg += (*itb)->getName();
            if ((*itb)->getName() != userChannels[userChannels.size() - 1]->getName()) // если
                part_arg += ","; //не последнее имя канала в списке
        }
    }
    arr_user[num]->setMsgArgs(part_arg);
    part(num, part_arg, true, fd);


    if (!userChannels.empty())
    {
        std::vector<Channel *>::iterator itb = userChannels.begin();
        std::vector<Channel *>::iterator ite = userChannels.end();
        for (; itb != ite; ++itb)
        {
            Channel				*chn = *itb;
            std::vector<User *>	chn_users = chn->getUsersVector();

            std::vector<User *>::iterator	it_begin = chn_users.begin();
            std::vector<User *>::iterator	it_end = chn_users.end();
            for (; it_begin != it_end; ++it_begin) {
                if (fd != (*it_begin)->getFd())
                    this->rplPrint((*it_begin)->getFd(), MSG_QUIT);
            }
        }
    }

    close(this->arr_user[num]->getFd());
    this->deleteClient(this->arr_user[num]->getFd());

    return 0;
}

int		Server::quit(int num, std::string& args)
{
	// разослать уведомление о QUIT всем остальным пользователям канала
	// удалить пользователя из его каналов (PART)
	// закрыть fd
	// удалить пользователя
    int fd = 0;
	std::string	quit_msg(args.length() ? args : this->arr_user[num]->getNickname());

	std::vector<Channel *>	userChannels = this->arr_user[num]->getVecChannel();
    fd = arr_user[num]->getFd();
	if (!userChannels.empty())
	{
		std::vector<Channel *>::iterator itb = userChannels.begin();
		std::vector<Channel *>::iterator ite = userChannels.end();
		for (; itb != ite; ++itb)
		{
			Channel				*chn = *itb;
			std::vector<User *>	chn_users = chn->getUsersVector();
			
			std::vector<User *>::iterator	it_begin = chn_users.begin();
			std::vector<User *>::iterator	it_end = chn_users.end();
			for (; it_begin != it_end; ++it_begin)
                this->rplPrint(chn_users[0]->getFd(), MSG_QUIT);
		}
	}

	std::string	part_arg;

	if (!userChannels.empty())
	{
		std::vector<Channel *>::iterator itb = userChannels.begin();
		std::vector<Channel *>::iterator ite = userChannels.end();
		for (; itb != ite; ++itb)
		{
			part_arg += "#";
			part_arg += (*itb)->getName();
			if ((*itb)->getName() != userChannels[userChannels.size() - 1]->getName()) // если
				part_arg += ","; //не последнее имя канала в списке
		}
	}

	part(num, part_arg, false, fd);
	
	close(this->arr_user[num]->getFd());
	this->deleteClient(this->arr_user[num]->getFd());

	return 0;
}
