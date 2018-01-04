#include "connect.h"

#define HEADER_S 1024

void socket_create(int port)
{
	int sdc = 0;
	int sdl = 0;
	int sds = 0;
	int sdr = 0;
	int on = 1;
	int S_PORT = port;
	int s_length = 0;

	int close_conn = 0;

	struct sockaddr_in s_server = {0};
	struct sockaddr_in s_client = {0};

	struct timeval timeout = {0};
	// in linux fd_set is a defined type
	fd_set m_set;
	fd_set w_set;

	// printf("hello server\n");
	// create the listening socket
	if((sdl = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Err creating socket");
	}
	// set reusable option on the listening socket
	// the so_reuseaddr allows rebinding of the addr after disconnected
	if(setsockopt(sdl, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1)
	{
		perror("Failed setting options SO_REUSEADDR on socket");
		close(sdl);
	}

	// try using nonblock flag for listening socket
	setNonBlock(sdl);

	s_server.sin_family = AF_INET;
	// converts host addr from IPv4 style to binary -> uint32
	s_server.sin_addr.s_addr = INADDR_ANY; //inet_addr("192.168.2.111");
	// converts a ip port from host order to network byte order
	s_server.sin_port = htons(S_PORT);

	// bind listening socket
	if(bind(sdl, (struct sockaddr*)&s_server, sizeof(struct sockaddr_in)) == -1)
	{
		perror("Err binding socket on host");
		close(sdl);
	}
	// listen on the port
	if(listen(sdl, 8) == -1)
	{
		perror("Err listening on port");
		close(sdl);
	}

	// set timeout limit for select to be 15.0 minutes
	timeout.tv_sec = 15 * 60;
	timeout.tv_usec = 0;

	s_length = sizeof(struct sockaddr_in);
	printf("Listening on port: %d\n", S_PORT);

	// initialize the master fd set to have 0 bits
	FD_ZERO(&m_set);
	// sets the bit of fd in the master fd set
	FD_SET(sdl, &m_set);

	do
	{
		// copy the mem of master set to the working set
		memcpy(&w_set, &m_set, sizeof(m_set));
		// wait for the selector to select out a socket with upcomming event
		printf("Waiting on select...\n");
		sds = select(sdl + 1, &w_set, NULL, NULL, NULL);

		if(sds < 0)
		{
			perror("Select process failed");
			break;
		}

		if(sds == 0)
		{
			perror("Select process timed out");
			break;
		}

		sdr = sds;
		// loop the sd set to check if any has upcomming event
		for(int i=0; (i<=sdl+1)&&(sdr>0); ++i)
		{
			if(FD_ISSET(i, &w_set))
			{
				sdr -= 1;
				// if the set sd is the listening sd, accept request
				if(i == sdl)
				{
					printf("Listening socket is readable now\n");
					do
					{
						sdc = accept(sdl, (struct sockaddr *)&s_client, &s_length);
						if(sdc == -1)
						{
							if(errno != EWOULDBLOCK)
							{
								perror("Accept request failed");
							}
							break;
						}
						// accept the request and prepare for the response
						printf("An upcoming http request accepted\n");
						acceptRequest(sdc);
						// close_conn = 1;
					}
					while(sdc != -1);
				}
				// if the set sd is not the listening sd, it must be some existing connection
				else
				{
					printf("Descripter [%d] is readable now\n", i);
				}
			}
		}
	}
	while(close_conn == 0);

	/*for (i=0; i <= sdl; ++i)
	{
	    if (FD_ISSET(i, &master_set))
	        close(i);
	}*/
	close(sdl);
}

void acceptRequest(int sd)
{
	FILE *f;
	char *res;
	// extract headers from the request
	// printf("Extracting http headers...\n");
	// extractHeader(sd);
	// send response back
	printf("Preparing http response...\n");

	f = fdopen(sd, "w+");
	char header_line[HEADER_S];

	do {
		res = fgets(header_line, HEADER_S, f);
		if (res != NULL) {
			printf("%s", res);
		}
	}
	while(res != NULL && strcmp(header_line, "\r\n") != 0);

	sendResponse(f);
	fclose(f);

	fflush(stdout);

	printf("Response sent successfully!\n");
}

// this fn should extract the method, http version and agent from headers
void extractHeader(int sd)
{
	int i = 0;
	int c = '\n';
	char header[HEADER_S];

	//for(i=0; (i<HEADER_S)&&(recv(sd,&c,1,0)>0); i++)
	//{
		//header[i] = c;
	//}

	//printf("Request Header: %s\n", header);
}

void sendResponse(FILE *f)
{
	fprintf(f, "HTTP/1.1 200 OK\r\n");
	fprintf(f, "Content-Type: application/json\r\n");
	fprintf(f, "\r\n");

	//fprintf(f, "HTTP/1.1 200 OK\r\n");
	//"Date: Wed, 20 Dec 2017 21:13:00 GMT\n"
	// fprintf(f, "Server: Duke's/1.0.0\r\n");
	//fprintf(f, "Content-Type: text/html\r\n");
	//fprintf(f, "\r\n");

	fprintf(f, "hello server");
}