#include <thread>
#include <iostream>
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtkmm.h>
#include <iostream>
#include <glib.h>
#include <fstream>

#include <string>
#include <cstring>

using namespace std;

char informacion[BUFFER_SIZE];
char infoLista[BUFFER_SIZE];
ListHandler<InfoContainer> linkedDataBase;
ListHandler<InfoContainer> cacheDataBase;
std::vector<std::string> container;
bool isAlive = false;


Server::Server()
{
    //linkedDataBase = new ListHandler<InfoContainer>();
}

void Server::init()
{
    isAlive = true;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        std::cout << "Could not create socket" << std::endl;
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 10599 );

    if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        exit(EXIT_FAILURE);
    }

    puts("bind done");

    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    std::thread collectManager(&Server::collect, this);
    collectManager.detach();
}

void Server::run()
{
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        puts("Connection accepted");

        new_sock = (int*) malloc(sizeof *new_sock);
        *new_sock = client_sock;

        std::thread clientManager(&Server::manage, this, std::ref(new_sock));
        clientManager.detach();
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

}

void Server::manage(int* socket_desc) {
    int sock = *socket_desc;
    int read_size;
    char client_message[BUFFER_SIZE];
    

    while ((read_size = recv(sock, client_message, BUFFER_SIZE, 0)) > 0 && isAlive)
    {
        std::string msg(decrypt(client_message));
        //std::string msg((client_message));
        container = split(msg, ',');

        sendSwitcher(&sock, container);
        //Interfaz();
        std::cout << msg << std::endl;
        print();

        container.clear();
        memset(client_message, 0, strlen(client_message));
    }

    free(socket_desc);
    close(sock);
    shutdown(sock, SHUT_RDWR);

    pthread_exit(NULL);

    //exit(EXIT_SUCCESS);

}

void Server::sendSwitcher(int* socket_desc, std::vector<std::string>& message)
{
    int sock = *socket_desc;
    char callBackBuffer[BUFFER_SIZE] = {0};

    //memset(callBackBuffer, 0, strlen(callBackBuffer));

    InfoContainer package;
    package.set_key(message[1]);
    package.set_size(atoi(message[2].c_str()));
    package.set_value(message[3]);

    std::string callBackMessage = "";

    switch (message[0][0])
    {
        case '1':

            if (linkedDataBase.isInList(message[1]) || cacheDataBase.isInList(message[1]))
            {
                callBackMessage = "-1,";
            }
            else
            {
                add(package);
                callBackMessage = "Added";
            }
            break;
        case '2':
            if (!linkedDataBase.isInList(message[1]) && !cacheDataBase.isInList(message[1]))
            {
                callBackMessage = "-1,";
            }
            else
            {
                callBackMessage = find(message[1]).to_string();
            }
            break;
        case '3':
            if (!linkedDataBase.isInList(message[1]) && !cacheDataBase.isInList(message[1]))
            {
                callBackMessage = "EXCEPTION_NOT_DELETED,1";
            }
            else
            {
                erase(message[1]);
                callBackMessage = "Removed";
            }
            break;
        default:
            callBackMessage = "Unspecified action";
    }

    strcpy(callBackBuffer, encrypt(callBackMessage).c_str());
    //strcpy(callBackBuffer, callBackMessage.c_str());
    write(sock, callBackBuffer, strlen(callBackBuffer));
}

const std::vector<std::string> Server::split(const std::string& split, const char& limiter)
{
    std::string buff;
    std::vector<std::string> resultVector;

    for(auto i : split)
    {
        if(i != limiter)
            buff += i;
        else if(i == limiter && buff != "")
        {
            resultVector.push_back(buff);
            buff = "";
        }
    }

    if(buff != "")
        resultVector.push_back(buff);

    return resultVector;
}

void Server::collect()
{
    while (isAlive)
    {
        for (int i = 0; i < linkedDataBase.size(); ++i)
        {
            if (linkedDataBase[i].get_reference() == 0)
            {
                erase(linkedDataBase[i].get_key());
                print();
            }
        }
        std::this_thread::sleep_for (std::chrono::seconds(10));
    }

}

void Server::add(InfoContainer container)
{

    if (cacheDataBase.ssize() < 5)
    {
        cacheDataBase.add(container);
        cacheDataBase.more_ssize();
    }
    else
    {
        InfoContainer temp = cacheDataBase.getPosition(0);
        cacheDataBase.erase(temp.get_key());
        linkedDataBase.add(temp);
        cacheDataBase.add(container);
    }
}

void Server::erase(std::string key)
{
    if (cacheDataBase.isInList(key))
    {
        cacheDataBase.erase(key);
        cacheDataBase.less_ssize();
    }
    else if (linkedDataBase.isInList(key))
    {
        linkedDataBase.erase(key);
    }
    else
    {
        std::cout << "NODO NO ELIMINADO" << std::endl;
    }
}

InfoContainer Server::find(std::string key)
{
    InfoContainer exception;
    exception.set_key("-1");

    if (cacheDataBase.isInList(key))
    {
        return cacheDataBase.find(key);
    }
    else if (linkedDataBase.isInList(key))
    {
        return linkedDataBase.find(key);
    }
    else
    {
        return exception;
    }
}

void Server::kill()
{
    isAlive = false;
    std::cout << "KILL" << std::endl;
    close(*new_sock);
    //exit(EXIT_SUCCESS);
}

void Server::revive()
{
    isAlive = true;
}


void* deserialize(std::string encoded)
{
    char* data = (char*) encoded.c_str();

    return (void*) data;
}




void Server::print()

{
	char aux[BUFFER_SIZE];
   	informacion[0] = '\0';
   	infoLista[0] = '\0';

    //std::cout << "/////////////////////////////////////" << std::endl;
    for (int i = 0; i < linkedDataBase.size(); i++)
    {
    	/*void* temp = deserialize(linkedDataBase[i].get_value());
    	std::string *sp = static_cast<std::string*>(temp);
    	string s = *sp;*/
        //std::cout << linkedDataBase[i].get_key() << "," << linkedDataBase[i].get_size() << std::endl;
        strcat(infoLista, linkedDataBase[i].get_key().c_str());
        sprintf(aux, "%d", linkedDataBase[i].get_size());
        strcat(infoLista," , ") ;
        strcat(infoLista, aux);
        strcat(infoLista," , ") ;
       /* linkedDataBase[i].set_value(s);
        strcat(infoLista, linkedDataBase[i].get_value().c_str());
        strcat(infoLista," , ") ;
        delete sp;
        */
    }
    //std::cout << "////////////////////////////////////////" << std::endl;
    //std::cout << "Tamaño: " << linkedDataBase.size() << std::endl;

    //std::cout << "////////////////////CACHE///////////////" << std::endl;
    for (int i = 0; i < cacheDataBase.size(); i++)
    {
      //  std::cout << cacheDataBase[i].get_key() << "," << cacheDataBase[i].get_size() << "," << cacheDataBase[i].get_value() <<std::endl;
    	/*void* temp = deserialize(cacheDataBase[i].get_value());
    	std::string *sp = static_cast<std::string*>(temp);
    	string s = *sp;*/

        strcat(informacion, cacheDataBase[i].get_key().c_str());
        sprintf(aux, "%d", cacheDataBase[i].get_size());
        strcat(informacion," , ") ;
        strcat(informacion, aux);
        strcat(informacion," , ") ;
      /*  cacheDataBase[i].set_value(s);
        strcat(informacion, cacheDataBase[i].get_value().c_str());
        strcat(informacion," , ") ;
        delete sp;*/

    }
    //std::cout << "////////////////////////////////////////" << std::endl;
    //std::cout << "Tamaño: " << cacheDataBase.size() << std::endl;
    ///std::cout << "Vector: " <<endl;


   // std::cout << informacion;


    
       
}






//////////////////interfaz////////////////////////////////



void runServer(Server& server);
void runCollector(Server& server);


void on_button_clicked_cache (GtkButton *button, gpointer data)
{
	GtkLabel *label = GTK_LABEL (data);
	gtk_label_set_text (label, informacion);

	
}


void on_button_clicked_lista (GtkButton *button, gpointer data)
{
	GtkLabel *label = GTK_LABEL (data);
	gtk_label_set_text (label, infoLista);

}

GtkWidget *construct_window (void)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *button, *button1;
	GtkWidget *labelCache, *labelLista, *titulo1, *titulo2, *titulo;
	GdkColor color;
	color.red = 0xBDF7;
	color.green = 0xBDF7;
	color.blue = 0x79E0;
	


	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title (GTK_WINDOW (window),
	"Control de Memoria");
	
	vbox = gtk_vbox_new (1, 1);
	button = gtk_button_new_with_label ("Mostrar elementos de la cache");
	button1 = gtk_button_new_with_label ("Mostrar elementos de la lista");
	labelCache = gtk_label_new ("");
	labelLista = gtk_label_new("");
	titulo1 = gtk_label_new(NULL);
	titulo2 = gtk_label_new(NULL);
	titulo = gtk_label_new(NULL);


	gtk_label_set_markup(GTK_LABEL(titulo),"<big><b><span foreground='#334D49' >\ Memoria remota del servidor</span></b></big>");
	gtk_label_set_markup(GTK_LABEL(titulo1),"<span foreground='#334D49' >\ Elementos contenidos en la cache:</span>");
	gtk_label_set_markup(GTK_LABEL(titulo2),"<span foreground='#334D49' >\ Elementos contenidos en la memoria (lista):</span>");



	gtk_box_pack_start (GTK_BOX (vbox), titulo, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox), titulo1, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox), labelCache, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox), titulo2, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox), labelLista, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox), button, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox), button1, 0, 0, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (on_button_clicked_cache),labelCache);
	g_signal_connect (G_OBJECT (button1), "clicked", G_CALLBACK (on_button_clicked_lista),labelLista);

	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);



	return window;
}


int main (gint argc, gchar **argv)
{

	Server server;
    server.init();

    std::thread serverThread(runServer, std::ref(server));
    serverThread.detach();

    std::thread collectorThread(runServer, std::ref(server));
    collectorThread.detach();

	gtk_init (&argc, &argv);

	GtkWidget *window = construct_window ();
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
	gtk_widget_show_all (window);
	g_signal_connect (G_OBJECT (window), "delete-event", gtk_main_quit, NULL);
	
	gtk_main ();
	return 0;
}

void runServer(Server& server)
{
    // The server starts
    server.run();
}

void runCollector(Server& server)
{
    server.collect();
}