
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    /*INICIALIZIZAMOS con nuestras variables para el intercambio de comunicacion
    */
    int client, server;
    int portNum = 1500;// Este puerto lo utilizaremos par el cliente y servidor
    bool isExit = false;//variable para saber si la conversacion termino
    int bufsize = 1024;//TAmaño de los datos de entrada
    char buffer[bufsize];//la direccion de memoria de la maquina a conectar
	
    struct sockaddr_in server_addr;//estructura que almacenara lso datos para poder cmunicarnos 
    socklen_t size;

    //Establecemos coneccion
    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError al establecer con el socket..." << endl;
        exit(1);
    }

    //Verificamos que el socket haya sio creado correctamente
    cout << "\n=> Servidor del socket ha sido creado..." << endl;

    

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    //Vinculamos el cleinte y el servidor

    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error en la coneccion, el socket ha sido establecido..." << endl;
        return -1;
    }


    size = sizeof(server_addr);
    cout << "=> Buscando clientes..." << endl;

    listen(client, 1);

	//El listen nos permite cer si hay un cliente esperando    
    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // first check if it is valid or not
    if (server < 0) 
        cout << "=> Error al aceptar..." << endl;

    
    //Proceso de comunciaion entre cliente y servidor
    while (server > 0) 
    {
        strcpy(buffer, "=> Servidor conectado...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Conectado con el cleinte #" << clientCount << ", empieze..." << endl;
        cout << "\n=> Entrar # para finalizar la coneccion \n" << endl;

        cout << "Cliente: ";
        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        //Recivimos la respuesta del cliente    
        } while (*buffer != '*');

        do {
            cout << "\nServidor: ";
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }//Enviamos mesajes
            } while (*buffer != '*');

            cout << "Cliente: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

        
        cout << "\n\n=> Coneccion terminada con IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nAdios..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;
}
