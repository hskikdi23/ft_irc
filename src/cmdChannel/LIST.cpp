#include "Server.hpp"

int Server::cmdList(std::vector<String> args, Client &cl) {
    if (args.size() == 1 || (args.size() == 2 && args.at(1) == "\r"))
    {
        if (_channels.size() == 0)
        {

            cl.reply(":NO SUCHCHANNEL"); // je reply 403???
            return -1;
        }
        for (size_t i = 0; i < _channels.size(); i++)
        {
            std::stringstream ss;
            ss << _channels.at(i).getClients().size();
            String size = ss.str();
            cl.reply("322 " + cl.getNickname() + " " + _channels.at(i).getName() + " " + size + " :" + _channels.at(i).getTopic());
        }
		cl.reply("323 " + cl.getNickname() + " :End of /LIST");
        return 0;
    }
	char	*tmp = strdup(args.at(1).c_str());
	std::vector<String> channel_name = split(tmp, ",");
    if (args.size() > 1)
    {
        for (size_t i = 0; i < channel_name.size(); i++)
        {
			int check = 0;
            size_t j = 0;
            for (; j < _channels.size() ; j++)
            {
                if (erasebr(channel_name.at(i)) == _channels.at(j).getName())
                {
                    std::stringstream ss;
                    ss << _channels.at(j).getClients().size();
                    String size = ss.str();
                    cl.reply("322 " + cl.getNickname() + " " + _channels.at(j).getName() + " " + size + " :" + _channels.at(j).getTopic());
					check = 1;
					break ;

                }
            }
            if (check == 0)
            {
                cl.reply("403 " + cl.getNickname() + " " + erasebr(channel_name.at(i)) + " :No such channel");
                return -1;
            }
        }
    }
    return 0;
}