#include "main.hpp"
#include "irc/Client.hpp"
void    startMessage(){
    printLog("", "___________________________________________________________|\n"
                 "|_________________________IRC START_________________________|\n"
                 "|___________________________________________________________", GREEN);
}
void    mainPrint(Irc & Irc)
{
    std::string code;
    write(1,"\E[H\E[2J",7);
    std::cout << " --------------------------------------------\n";
    std::cout << "|" << std::setw(10) << " nickname " << "|" << std::setw(4) << " fd " << "|" << std::setw(10) << "  method  " << "|" << std::setw(6) << " code " <<"|"<<" received " << "|\n";
    for(std::vector<Client>::iterator it = Irc.getClients().begin();it != Irc.getClients().end(); it++)
    {
//        if(it->getLastCode() > 0 && it->getLastCode() < 2000)
            code = std::to_string(it->getLastCode());
        std::cout << "|" << std::setw(10) << it->getNickName() << "|" << std::setw(4) << it->getSocketFd() << "|"<< std::setw(10) << it->getLastMethod() << "|" <<  std::setw(6) << code << "|" << std::setw(10) << it->getRequest().getBytesReceieved() << "|\n";
    }
    std::cout << " --------------------------------------------\n";
    std::cout << " --------------------------------------------\n";
    std::cout << "|" << std::setw(8) << "  chan  " << "|" << std::setw(9) << " #joined " << "|" << std::setw(10) << " #invited " << "|" << std::setw(9) << " #banned " << "|" << std::setw(4) << " +i "  << "|\n";
    for(std::vector<Chan>::iterator it=Irc.getChannels().begin();it!=Irc.getChannels().end();it++){
        std::cout << "|" << std::setw(8) << it->getName() << "|" << std::setw(9) << it->getChanClientsCount() << "|" << std::setw(10) << it->getChanInvitedCount() << "|" << std::setw(9) << it->getChanBannedCount() << "|" << std::setw(4) << it->getChanInviteFlag()  << "|\n";
    }
    std::cout << " --------------------------------------------\n";
}
void    printIrcServers(Irc &Irc)
{
    std::cout << Irc.getPortServer().getIp() << " " << Irc.getPortServer().getPort() << "\n";
}

int     main(int argc, char ** argv)
{
    validateArgv(argc, argv);

	Irc Irc(atoi(argv[1]), "0.0.0.0", argv[2]);
	startMessage();
    listen(Irc.getPortServer().getSocketFD(), 1000);
	while(42)
	{
		// preparing for SELECT
		int largestFD = 0;
		fd_set readfds, writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		// adding listen sockets to SELECT's read events catching array
        FD_SET(Irc.getPortServer().getSocketFD(), &readfds);
        if(Irc.getPortServer().getSocketFD() > largestFD)
            largestFD = Irc.getPortServer().getSocketFD();
		// adding client sockets to SELECT's read/write events catching array
		for(std::vector<Client>::iterator it = Irc.getClients().begin();it != Irc.getClients().end(); it++)
		{
            if(it->getStatus() != CLOSING)
            {
                FD_SET(it->getSocketFd(), &readfds);
                if (it->getStatus() == WRITING)
                    FD_SET(it->getSocketFd(), &writefds);
                if (it->getSocketFd() > largestFD)
                    largestFD = it->getSocketFd();
            }
		}
        mainPrint(Irc);
        // SELECT
//		std::cout << "\nselect:\n";
		if(select(largestFD + 1, &readfds, &writefds,0,0) < 0)
		{
			printLog("","Irc: select error",RED);
			exit(EXIT_FAILURE);
		}

		// checking all connections for closing
		for(std::vector<Client>::iterator it = Irc.getClients().begin();it != Irc.getClients().end(); it++){
			if(it->getStatus() == CLOSING)
            {
                std::cout << SOME << "close: " << it->getSocketFd() << WHITE << "\n";
				close(it->getSocketFd());
				Irc.getClients().erase(it);
				break;
			}
		}
       // finding an event in client sockets array
        for(std::vector<Client>::iterator it = Irc.getClients().begin();it != Irc.getClients().end(); it++)
        {
            if(Irc.getClients().empty())
                break;
            // finding a read event in client sockets array
            if (FD_ISSET(it->getSocketFd(), &readfds))
            {
//                std::cout << "read: " << it->getSocketFd() << "\n";
                it->readRequest();
                break;
            }
            // finding a write event in client sockets array
            else if(FD_ISSET(it->getSocketFd(), &writefds))
            {
//                std::cout << "write: " << it->getSocketFd() << "\n";
                it->sendResponse();
                break;
            }
        }
        // finding a new connection event in listen sockets array
        int fd;
        if(FD_ISSET(Irc.getPortServer().getSocketFD(), &readfds))
        {
            // creating new connection/client socket
            struct sockaddr_in adrAccept;
            bzero((void *)&adrAccept,sizeof adrAccept);
            socklen_t adrAcceptLen = sizeof adrAccept;
            fd = accept(Irc.getPortServer().getSocketFD(), (struct sockaddr *)&adrAccept, &adrAcceptLen);
            if (fd < 0){
                printLog("","accept error",RED);
                exit(EXIT_FAILURE);
            }
//				std::cout << SOME << "new client: " << fd << WHITE << "\n";
            fcntl(fd, F_SETFL, O_NONBLOCK);
            Irc.addClient(fd, &Irc,inet_ntoa(adrAccept.sin_addr));
            std::cout << "new client " << fd << " " << inet_ntoa(adrAccept.sin_addr) <<"\n";
        }
        for(std::vector<Client>::iterator it = Irc.getClients().begin();it != Irc.getClients().end(); it++)
        {
            if(Irc.getClients().empty())
                break;
            // checking all connections for closing
            if(it->getStatus() == CLOSING)
            {
                std::cout << SOME << "close: " << it->getSocketFd() << WHITE << "\n";
                close(it->getSocketFd());
                Irc.getClients().erase(it);
                break;
            }

        }
	}
	// very bad place:)
	return 0;
}
