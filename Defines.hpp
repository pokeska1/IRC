#pragma once

#define VEC_STR std::vector<std::string>
#define VEC_ITER_STR std::vector<std::string>::iterator
#define VEC_ITER_STR_ADR std::vector<std::string *>::iterator
#define VEC_ITER_CHANEL_ADR std::vector<Channel *>::iterator
#define VEC_ITER_USER_ADR std::vector<User *>::iterator


#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHSERVER "402"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_WASNOSUCHNICK "406"
#define ERR_TOOMANYTARGETS "407"
#define ERR_NOORIGIN "409"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_NOTOPLEVEL "413"
#define ERR_WILDTOPLEVEL "414"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NOMOTD "422"
#define ERR_NOADMININFO "423"
#define ERR_FILEERROR "424"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NICKCOLLISION "436"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"
#define ERR_NOLOGIN "444"
#define ERR_SUMMONDISABLED "445"
#define ERR_USERSDISABLED "446"
#define ERR_NOTREGISTERED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_NOPERMFORHOST "463"
#define ERR_PASSWDMISMATCH "464"
#define ERR_YOUREBANNEDCREEP "465"
#define ERR_KEYSET "467"
#define ERR_CHANNELISFULL "471"
#define ERR_UNKNOWNMODE "472"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BANNEDFROMCHAN "474"
#define ERR_BADCHANNELKEY "475"
#define ERR_NOPRIVILEGES "481"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_CANTKILLSERVER "483"
#define ERR_NOOPERHOST "491"
#define ERR_UMODEUNKNOWNFLAG "501"
#define ERR_USERSDONTMATCH "502"

#define RPL_NONE "300"
#define RPL_USERHOST "302"
#define RPL_ISON "303"
#define RPL_AWAY "301"
#define RPL_UNAWAY "305"
#define RPL_NOWAWAY "306"
#define RPL_WHOISUSER "311"
#define RPL_WHOISSERVER "312"
#define RPL_WHOISOPERATOR "313"
#define RPL_WHOISIDLE "317"
#define RPL_ENDOFWHOIS "318"
#define RPL_WHOISCHANNELS "319"
#define RPL_WHOWASUSER "314"
#define RPL_ENDOFWHOWAS "369"
#define RPL_LISTSTART "321"
#define RPL_LIST "322"
#define RPL_LISTEND "323"
#define RPL_CHANNELMODEIS "324"
#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define RPL_INVITING "341"
#define RPL_SUMMONING "342"
#define RPL_VERSION "351"
#define RPL_WHOREPLY "352"
#define RPL_ENDOFWHO "315"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"
#define RPL_LINKS "364"
#define RPL_ENDOFLINKS "365"
#define RPL_BANLIST "367"
#define RPL_ENDOFBANLIST "368"
#define RPL_INFO "371"
#define RPL_ENDOFINFO "374"
#define RPL_MOTDSTART "375"
#define RPL_MOTD "372"
#define RPL_ENDOFMOTD "376"
#define RPL_YOUREOPER "381"
#define RPL_REHASHING "382"
#define RPL_TIME "391"
#define RPL_USERSSTART "392"
#define RPL_USERS "393"
#define RPL_ENDOFUSERS "394"
#define RPL_NOUSERS "395"
#define RPL_TRACELINK "200"
#define RPL_TRACECONNECTING "201"
#define RPL_TRACEHANDSHAKE "202"
#define RPL_TRACEUNKNOWN "203"
#define RPL_TRACEOPERATOR "204"
#define RPL_TRACEUSER "205"
#define RPL_TRACESERVER "206"
#define RPL_TRACENEWTYPE "208"
#define RPL_TRACELOG "261"
#define RPL_STATSLINKINFO "211"
#define RPL_STATSCOMMANDS "212"
#define RPL_STATSCLINE "213"
#define RPL_STATSNLINE "214"
#define RPL_STATSILINE "215"
#define RPL_STATSKLINE "216"
#define RPL_STATSYLINE "218"
#define RPL_ENDOFSTATS "219"
#define RPL_STATSLLINE "241"
#define RPL_STATSUPTIME "242"
#define RPL_STATSOLINE "243"
#define RPL_STATSHLINE "244"
#define RPL_UMODEIS "221"
#define RPL_LUSERCLIENT "251"
#define RPL_LUSEROP "252"
#define RPL_LUSERUNKNOWN "253"
#define RPL_LUSERCHANNELS "254"
#define RPL_LUSERME "255"
#define RPL_ADMINME "256"
#define RPL_ADMINLOC1 "257"
#define RPL_ADMINLOC2 "258"
#define RPL_ADMINEMAIL "259"

#define RPL_TRACECLASS "209"
#define RPL_SERVICEINFO "231"
#define RPL_SERVICE "233"
#define RPL_SERVLISTEND "235"
#define RPL_WHOISCHANOP "316"
#define RPL_CLOSING "362"
#define RPL_INFOSTART "373"
#define ERR_YOUWILLBEBANNED "466"
#define ERR_NOSERVICEHOST "492"
#define RPL_STATSQLINE "217"
#define RPL_ENDOFSERVICES "232"
#define RPL_SERVLIST "234"
#define RPL_KILLDONE "361"
#define RPL_CLOSEEND "363"
#define RPL_MYPORTIS "384"
#define ERR_BADCHANMASK "476"

////////////// MESSAGES ///////////////////////////////////////////////////////

//324
#define MSG_CHANNELMODEIS ":" + this->getHost() + " 324 " \
+ this->arr_user[num]->getNickname() + " #" + cur_chan->getName()

//331
#define MSG_NOTOPIC ":" + this->getHost() + " " + RPL_NOTOPIC + " " \
+ this->arr_user[num]->getNickname() + " #" + cur_chan->getName() \
+ " :" +  "No topic is set" + "\r\n"

//332
#define MSG_TOPIC ":" + this->getHost() + " " + RPL_TOPIC + " " \
+ this->arr_user[num]->getNickname() + " #" + cur_chan->getName() \
+ " :" + cur_chan->getTopic() + " \r\n"

//001 если имя хоста не задано
#define MSG_WEL_COME_DEFAULT ":" + this->getHost() + " 001 " + this->arr_user[num]->getNickname() \
+ " :Welcome to the Internet Relay Network " \
+ this->arr_user[num]->getNickname() + \
"!" + this->arr_user[num]->getUsername() + "@" \
+ this->arr_user[num]->getHostname() + "\r\n";
//303 #define RPL_ISON "303"
//#define MSG_RPL_ISON ":"
#define MSG_RPL_ISON ":" + this->getHost() + " " + RPL_ISON + " :"
//315 #define RPL_ENDOFWHO "315"
#define MSG_ENDOFWHO ":" + this->getHost() + " 315 " + arr_user[num]->getNickname() + " " + channel + " :End of /WHO list"
//332
#define MSG_HELLO_AND_JOIN ":" + this->getHost() + " 332 " + arr_user[num]->getNickname() + " #" + topic + " :No topic is set" + "\r\n"
//332
#define MSG_HELLO_AND_JOIN_THITH_TOPIC ":" + this->getHost() + " 332 " + arr_user[num]->getNickname() + " #" + topic \
+ " :" + (*it_b_channel)->getTopic() + "\r\n"
//341
#define MSG_INVITING ":" + this->getHost() + " " + RPL_INVITING + " " \
+ arr_user[num]->getNickname() + " :" +  "inviting" + "\r\n"
//352     RPL_WHOREPLY
#define MSG_WHOREPLY ":" + this->getHost() + " 352 " + arr_user[num]->getNickname() + " = " + channel + " :"
//353
#define MSG_LIST_USER_IN_CHANELL ":" + this->getHost() + " 353 " + arr_user[num]->getNickname() + " = #" + topic \
+ " :";
//366
#define MSG_END_OF_USER_LIST ":" + this->getHost() + " 366 " + arr_user[num]->getNickname() + " #" + topic \
+ " :END of /NAMES list\r\n";
//401
#define MSG_NOSUCHNICK ":" + this->arr_user[num]->getServername() + " " + ERR_NOSUCHNICK \
+ " " + this->arr_user[num]->getHostname() + " " + this->arr_user[num]->getNickname() \
+ " Нет такого челика \""
//403
#define MSG_NOSUCHCHANNEL ":" + this->arr_user[num]->getServername() + " " \
+ ERR_CANNOTSENDTOCHAN + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname()  +  " <channel name> :No such channel\n"
//404
#define MSG_CANNOTSENDTOCHAN ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NOSUCHCHANNEL + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + \
" :To send messages, JOIN the channel" + " #" + name +" \n"
//404 KICK
#define MSG_CANNOTSENDTOCHAN_KICK ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NOSUCHCHANNEL + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + \
" :you can't kick yourself out, use the PART command\n"
//405 #define
#define MSG_TOOMANYCHANNELS ":" + this->arr_user[num]->getServername() + " " \
+ ERR_TOOMANYCHANNELS + " " + this->arr_user[num]->getHostname() + " " \
+  " :You're have too many channel.\n"

//442
#define MSG_NOTONCHANNEL ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NOTONCHANNEL + " " + this->arr_user[num]->getHostname() + " " \
+ cur_chan->getName() + " :You're not on that channel\n"
//443
#define MSG_USERONCHANNEL ":" + this->arr_user[num]->getServername() + " " \
+ ERR_USERONCHANNEL + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + " " + cur_chan->getName() + " :is already on channel\n"
//461
#define MSG_NEEDMOREPARAMS ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NEEDMOREPARAMS + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname()  +  " <command> :Not enough parameters\n"
//#define ERR_CHANNELISFULL "471"
#define MSG_CHANNELISFULL ":" + this->arr_user[num]->getServername() + " 471 "\
+ this->arr_user[num]->getHostname() + " " \
+  " :This channel can have maximum :" + limit + " Users, and you are not one of them Hehehehhehehe :D\n"
//482
#define MSG_CHANOPRIVSNEEDED ":" + this->arr_user[num]->getServername() + " " \
+ ERR_CHANOPRIVSNEEDED + " " + this->arr_user[num]->getHostname() + " " \
+ cur_chan->getName() + " :You're not channel operator\n"
#define MSG_CHANOPRIVSNEEDED_KICK ":" + this->arr_user[num]->getServername() + " " \
+ ERR_CHANOPRIVSNEEDED + " " + this->arr_user[num]->getHostname() + " " \
+ cur_chan->getName() + " :You're not channel operator. And you dare to think about KIСK someone ?!\n"
//473
#define MSG_INVITEONLYCHAN ":" + this->arr_user[num]->getServername() + " " + ERR_INVITEONLYCHAN \
+ " " + this->arr_user[num]->getHostname() + " " + this->arr_user[num]->getNickname() +          \
" :This channel (#" + this->arr_channel[num_channel]->getName() +  ") only for invite\r\n"
//475
#define MSG_BADCHANNELKEY ":" + this->arr_user[num]->getServername() + " " \
+ ERR_BADCHANNELKEY + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + " where is the key ?!" \
+ " You need a password to enter the channel!\n"
//501 
#define MSG_UMODEUNKNOWNFLAG ":" + this->arr_user[num]->getServername() + " " \
+ ERR_UMODEUNKNOWNFLAG + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname()  +  ":Unknown MODE flag\n"
//ZASLUSHKA
#define MSG_ZAGLUSHKA ":" + this->arr_user[num]->getServername() + " " \
+ ERR_UMODEUNKNOWNFLAG + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname()  + cur_chan->getTopic() + ":ZAGLUSHKA VMESTO RPL_TOPIC\n"

//epilar
//nick
//431
#define MSG_NONICKNAME ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NONICKNAMEGIVEN + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + " " + ":No nickname given\r\n"

//433
#define MSG_NICKNAMEINUSE ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NICKNAMEINUSE + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + " " + nickname + " :Nickname is already in use\r\n"

//432
#define MSG_ERRNICKNAME ":" + this->arr_user[num]->getServername() + " " \
+ ERR_ERRONEUSNICKNAME + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + " " + nickname + \
" :Nickname is incorrect(not more 9 symbols: letters,numbers)\r\n"

//nick answer
#define MSG_NICKCHANGED ":" + this->arr_user[num]->getNickname() + " NICK " + nickname + "\r\n" 

//server version
#define SERVER_VERSION "1.0"
#define VERSION_COMMENTS "by jmarian/rmerrie/epilar"
#define MSG_SERVERVERSION ":" + this->getHost() + " 351 " + this->arr_user[num]->getNickname() \
+ " " + SERVER_VERSION + " " + this->getHost() + " :" + VERSION_COMMENTS + "\r\n"

//402
#define MSG_NOSUCHSERVER ":" + this->arr_user[num]->getServername() + " " \
+ ERR_NOSUCHSERVER + " " + this->arr_user[num]->getHostname() + " " \
+ this->arr_user[num]->getNickname() + " " + servername + " :No such server\r\n"

//server info
#define PATH_TO_SERVER_FILE "./ircserv"
#define MSG_SERVERINFO ":" + this->getHost() + " 371 " + this->arr_user[num]->getNickname() + \
" " + SERVER_VERSION + " " + this->getHost() + " :" + VERSION_COMMENTS + "\r\n" + \
":" + this->getHost() + " 371 " + this->arr_user[num]->getNickname() + \
" Created: " + statistic[0] + "\r\n" + \
":" + this->getHost() + " 371 " + this->arr_user[num]->getNickname() + \
" Modified: " + statistic[1] + "\r\n" + \
":" + this->getHost() + " 371 " + this->arr_user[num]->getNickname() + \
" Elapsed time: " + time_elapsed + "\r\n"
#define MSG_ENDOFINFO ":" + this->getHost() + " 374 " + this->arr_user[num]->getNickname() + " :End of /INFO list\r\n"

//quit
#define MSG_QUIT ":" + arr_user[num]->getNickname() + "!" + arr_user[num]->getUsername() \
+ "@" + arr_user[num]->getHostname() + \
" QUIT " + quit_msg + "\r\n"

//part answer
#define MSG_PARTSUCCESS ":" + this->arr_user[num]->getNickname() + "!" + \
this->arr_user[num]->getUsername() + "@" + this->arr_user[num]->getHostname() + \
" PART #" + cur_chan->getName() + " :" + this->arr_user[num]->getNickname() + "\r\n"

////////////// NON ERROR MESSAGES ///////////////////////////////////////////////////////
//Подтверждение что join произошел удачно
#define MSG_ACCESS_JOIN ":" + arr_user[num]->getNickname() + "!" + arr_user[num]->getUsername() + "@" \
+ arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " #" \
+ topic + "\r\n"
//собщение одному человеку
#define MSG_PRIVMSG ":" + arr_user[num]->getNickname() + "!" + arr_user[num]->getUsername() \
+ "@" + arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " " \
+ arr_user[num]->getMsgArgs() + "\r\n"  \
//+ arr_user[num_friend]->getNickname() + arr_user[num]->getMsgArgs() + "\r\n"
//сообщение по каналу
#define MSG_PRIVMSG_CHANNEL ":" + arr_user[num]->getNickname() + "!" + arr_user[num]->getUsername() \
+ "@" + arr_user[num]->getHostname() + " " + arr_user[num]->getMsgCom() + " " \
+ arr_user[num]->getMsgArgs() + "\r\n"

