
#include "webserv.hpp"



void check_timeout_clients(std::map<int, Client*> *clients, struct pollfd *fds, int fds_size, bool *need_compress)
{
    std::stack<int> fds_for_delete;
    std::cout << "  Check clients timeout...\n";
    time_t now = time(0);
    for (std::map<int, Client*>::iterator first = (*clients).begin(); first != (*clients).end(); first++) {
        if (now - first->second->getTimeLastAction() >= (CLIENT_TIMEOUT / 1000)) {
            fds_for_delete.push(first->first);
        }
    }
    while (fds_for_delete.size()) {
        int fd = fds_for_delete.top();
        fds_for_delete.pop();
        for (int index = 0; index < fds_size; index++) {
            if (fds[index].fd  == fd) {
                close(fd);
                delete (*clients)[fd];
                clients->erase(fd);
                fds[index].fd = -1;
                *need_compress = true;
                break;
            }
        }
    }
}

void compress_array(struct pollfd *fds, int &fds_size)
{
    for (int i = 0; i < fds_size; i++)
    {
        if (fds[i].fd == -1)
        {
            for(int j = i; j < fds_size; j++) {
                fds[j].fd = fds[j+1].fd;
                fds[j].events = fds[j+1].events;
                fds[j].revents = fds[j+1].revents;
            }
            i--;
            fds_size--;
            bzero(&fds[fds_size], sizeof(fds[fds_size]));
        }
    }
}

void clear_servers(std::map<int, Server*> &servers)
{
    std::cout << "Clear server(s)..." << "\n";
    for (std::map<int, Server*> ::iterator server = servers.begin(); server != servers.end(); server++) {
        close(server->first);
        delete server->second;
    }
    servers.clear();
}

void clear_clients(std::map<int, Client*> &clients)
{
    std::cout << "Clear client(s)..." << "\n";
    for (std::map<int, Client*> ::iterator client = clients.begin(); client != clients.end(); client++) {
        close(client->first);
        delete client->second;
    }
    clients.clear();
}
bool must_exit = false;

int main(int argc, char **argv){
    std::map<int, Server*> servers;
    std::map<int, Client*> clients;
    struct pollfd fds[2048];
    int fds_size = 0;
    int port, sockfd;

    std::vector<ServerConf*> servers_pool = getConfigs(argc, argv);
    std::vector<ServerConf*>::iterator si_b = servers_pool.begin();
    std::vector<ServerConf*>::iterator si_e = servers_pool.end();
    Server *tmp = NULL;
    while (si_b != si_e)
    {
        port = si_b[0]->getPort();
        tmp = new Server(si_b[0]);
        sockfd = tmp->createSocket(port, si_b[0]->getIP());
        servers[sockfd] = tmp;
		servers[sockfd]->fixTime();
        if (listen(sockfd, 1000) == -1)
        {
            clear_servers(servers);
            printf("Error listen()\n");
            exit(1);
        }
        ++si_b;
    }

    memset(&fds, 0, sizeof(fds));
    for (std::map<int, Server*>::iterator first = servers.begin(); first != servers.end(); first++) {
        fds[fds_size].fd = first->first;
        fds[fds_size++].events = POLLIN;
    }

    while (servers.size())
	{
        std::cout << "N of clients: " << clients.size() << "\n";
        
        bool compress = false;
        if (must_exit == 1)
            break;
        printf("Waiting for event...\n");
		// ждем либо подключения нового клиента, либо сообщения от уже подключенного
        int code = 0;
		if ((code = poll(fds, fds_size, CLIENT_TIMEOUT)) == -1 )
		{
			std::cout << "Poll error\n";
			exit(1);
		}
		else
			printf("Poll triggered\n");

        //добавляем новых клиентов
        for (int i = 0; i < fds_size; i++)
		{
            if (fds[i].revents == 0) {
                continue;
			} else if (fds[i].revents == POLLIN) {
                int fd = fds[i].fd;
				if (servers.find(fd) != servers.end()) {
                    printf("New connection\n");
                    Client *new_client = servers[fd]->acceptClient();
                    if (new_client) {
                        int new_client_fd = new_client->getFd();
                        clients[new_client_fd] = new_client;
                        fds[fds_size].fd = new_client_fd;
                        fds[fds_size].events = POLLIN;
                        fds[fds_size++].revents = 0;
                    } else {
                        printf("Connection refused\n");
                    }
				} else if (clients.find(fd) != clients.end()) {
                    printf("Read client request\n");
                    int code = clients[fd]->readRequest();
                    if (code == BAD_READ) {
                        printf("Error while read request\n");
                        delete clients[fd];
                        clients.erase(fd);
                        fds[i].fd = -1;
                        compress = true;
                    } else if (code == END_READ) {
                        fds[i].events = POLLOUT;
                    } else if (code == CLOSE_CONNECTION) {
                        printf("Connection closed by client\n");
                        delete clients[fd];
                        clients.erase(fd);
                        fds[i].fd = -1;
                        compress = true;
                    }
                }
                fds[i].revents = 0;
			} else if (fds[i].revents == POLLOUT) {
                int fd = fds[i].fd;
                if (clients.find(fd) != clients.end()) {
                    printf("Write response\n");
                    int code = clients[fd]->sendResponse();
                    if (code == BAD_WRITE) {
                        printf("Error while write response\n");
                        delete clients[fd];
                        clients.erase(fd);
                        compress = true;
                        fds[i].fd = -1;
                    } else if (code == END_WRITE) {
                        fds[i].events = POLLIN;
                    } else if (code == CLOSE_CONNECTION) {
                        printf("Connection closed by client\n");
                        close(fd);
                        delete clients[fd];
                        clients.erase(fd);
                        compress = true;
                        fds[i].fd = -1;
                    }
                    fds[i].revents = 0;
                }
            } else {
                std::cerr << "fd:" << fds[i].fd << std::endl;
                std::cerr << "event:" << fds[i].events << std::endl;
                std::cerr << "revents:" << fds[i].revents << std::endl;
                if (servers.find(fds[i].fd) != servers.end()) {
                    delete servers[fds[i].fd];
                    servers.erase(fds[i].fd);
                }
                if (clients.find(fds[i].fd) != clients.end()) {
                    delete clients[fds[i].fd];
                    clients.erase(fds[i].fd);
                }
                close(fds[i].fd);
                compress = true;
                fds[i].fd = -1;
            }
		}
        check_timeout_clients(&clients, fds, fds_size, &compress);
        if (compress) {
            compress_array(fds, fds_size);
        }
    }
    clear_clients(clients);
    clear_servers(servers);
    std::cout << "Bye!\n";
    return 0;
}
