#include "Server.h"

// will run the whole server
void Server::run()
{
	std::string exitOption; // if this string will equal "EXIT" the program will stop running

	std::thread t_connector(&Communicator::startHandleRequests, std::ref(m_communicator)); // run communicator as thread
	t_connector.detach();

	// get value into exitOption forever
	while (true)
	{
		std::cin >> exitOption;
		if (exitOption == "EXIT")
		{
			exit(0);
		}
	}
}
