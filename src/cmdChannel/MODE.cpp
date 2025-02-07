#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

int	giveOprivilege(Client &cl, std::vector<String> args, Channel &chan) {
	int check = 0;
	size_t i = 0;

	if (args.size() < 4) {
		cl.reply(ERR_NEEDMOREPARAMS(cl, "MODE"));
		return -1;
	}
	for (; i < chan.getClients().size(); i++)
	{
		if (chan.getClients().at(i).getNickname() == erasebr(args.at(3)))
		{
			std::cout << "je suis ici" << std::endl;
			check = 1;
			break ;
		}
	}
	if (check == 1)
		chan.setFdOp(chan.getClients().at(i).getFd());
	else
		cl.reply("401 " + cl.getNickname() + " " + erasebr(args.at(3)) + " :No such nick/channel");
	return 0;
}


int setPassword(String password, Channel &chan) {
	if (password.empty())
		return -1;
	chan.setPassword(password);
	return 0;
}

int removePassword(String password, Channel &chan) {
	if (password.empty())
		return -1;

	if (chan.getPassword() == password)
		chan.setPassword("");
	else
		std::cout << "Wrong pass" << std::endl;
	return 0;
}


size_t	parseLimit(std::string arg) {
	char	*buff;

	std::string tmp = erasebr(arg);
	if (tmp.empty())
		return (0);

	long int limit = strtol(tmp.c_str(), &buff, 10);

	if (*buff != '\0')
		return (0);
	return (limit);
}

int	setLimit(size_t limit, Channel &chan)
{
	if (limit < chan.getClients().size() && limit != 0)
	{
		std::cout << "to many people on chan to set this limit" << std::endl;
		return -1;
	}
	chan.setLimit(limit);
	return 0;
}

int	check_flag(std::vector<String> args, Client &cl, Channel &chan) {
	int i = 0;
	std::string flags[7] = {"+O","+o","-o","+l","-l","+k","-k"};

	if (args.size() < 3)	{
		cl.reply("482 " + cl.getNickname() + " " + chan.getName() + " :We need an operator");
		return -1;
	}
	String flag = erasebr(args[2]);
	while (flag != flags[i] && i < 7)
		++i;
	std::cout << "IN CHECK_FLAG" << std::endl;
	switch (i) {
		case 0:
			return(giveOprivilege(cl, args, chan));
		case 1:
			return (giveOprivilege(cl, args, chan));
		case 2:
			cl.reply("482 " + cl.getNickname() + " " + chan.getName() + " :We need an operator");
			return -1;
		case 3:
			if (args.size() < 4) {
				cl.reply(ERR_NEEDMOREPARAMS(cl, "MODE"));
				return -1;
			}
			return (setLimit(parseLimit(args[3]), chan));
		case 4:
			return (setLimit(0, chan));
		case 5:
			if (args.size() < 4) {
				cl.reply(ERR_NEEDMOREPARAMS(cl, "MODE"));
				return -1;
			}
			return (setPassword(erasebr(args[3]), chan));
		case 6:
			if (args.size() < 4) {
				cl.reply(ERR_NEEDMOREPARAMS(cl, "MODE"));
				return -1;
			}
			return (removePassword(erasebr(args[3]), chan));
		default:
			cl.reply("501 " + cl.getNickname() + " :Unknown MODE flag");
			return -1;
	}
	return 0;
}


int Server::cmdMode(std::vector<String> args, Client &cl) {
	std::cout << "in MODE->args = " << std::endl;
	for (unsigned int i = 0; i < args.size(); i++)
		std::cout << args[i] << std::endl ;
	std::cout << std::endl;

	if (args.size() < 2) //pas sur suivant les cmd qu'on implemente
	{
		cl.reply(ERR_NEEDMOREPARAMS(cl, "MODE"));
		return -1;
	}
	if (isChannel(args.at(1)) == false)
	{
		if (erasebr(args[1]).at(0) != '#')
			return -1;
		cl.reply(ERR_NOSUCHCHANNEL(cl, erasebr(args[1])));
		return -1;
	}
	if (cl.getFd() != findChannel(args.at(1)).getFdOp())
	{
		cl.reply(ERR_CHANOPRIVSNEEDED(cl, args.at(1)));
		return -1;
	}
	check_flag(args, cl, findChannel(args.at(1)));
	return 1;
}