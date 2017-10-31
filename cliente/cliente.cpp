#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
   /*INICIALIZIZAMOS con nuestras variables para el intercambio de comunicacion
    */

    int client;
    int portNum = 1500; // Este puerto lo utilizaremos par el cliente y servidor
    bool isExit = false;//variable para saber si la conversacion termino
    int bufsize = 1024;//TAmaño de los datos de entrada
    char buffer[bufsize];//la direccion de memoria de la maquina a conectar
    const char* ip = "127.0.0.1";//la direccion de memoria de la maquina a conectar

    struct sockaddr_in server_addr;//estructura que almacenara lso datos para poder cmunicarnos 

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError al establecer coneccion con el socket..." << endl;
        exit(1);
    }

    
    cout << "\n=> Socket del cliente ha sido creado..." << endl;
    
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
	inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Conectad con el servidor " << inet_ntoa(server_addr.sin_addr) << " con el numero de puerto: " << portNum << endl;


    
    cout << "=> Esperando confirmacion del servidor..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Coneccion confirmada, empezar...";

    cout << "\n\n=> Entrar # para terminar la coneccion\n" << endl;

    
    do {
        cout << "Cliente: ";
        do {
            cin >> buffer;
            send(client, buffer, bufsize, 0);
            if (*buffer == '#') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }//Enviamos un mensaje
        } while (*buffer != 42);

        cout << "Servidor: ";
        do {
            recv(client, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }//Recepionamos el mesnasaje del servidor

        } while (*buffer != 42);
        cout << endl;

    } while (!isExit);

    //finalizamos la coneccion cuando mandamso un #
    cout << "\n=> Coneccion terminada.\nAdios...\n";

    close(client);
    return 0;
}
