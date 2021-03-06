/* ********************************************** */
/* Na podstawie: http://xmlrpc-c.sourceforge.net  */
/* ********************************************** */
#define WIN32_LEAN_AND_MEAN  /* wymagane przez xmlrpc-c/server_abyss.h */

#include <stdlib.h>
#include <stdio.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//======================================================================

int error( int ern, const char *err ){
	perror( err );
	return ern;
}
int funkcja(char * text)
{
    int  	port   = 8000;
	char 	host[] = "127.0.0.1";
	/* zmienne niezainicjowane ****************************************/
    int 	fd, n;
    struct 	sockaddr_in serv_addr;
    struct 	hostent *server;
    char 	buffer[256];
    
    /* tworzymy gniazdo ***********************************************/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (fd < 0) 
        return error( 1, "socket()");
  
	/* pobieramy informacje o serwerze ********************************/
    server = gethostbyname( host );
  
    if( server == NULL )
		return error( 2, "gethostbyname()" );
    
    /* zapelniamy strukture zerami (inicjujemy) ***********************/
    bzero((char *) &serv_addr, sizeof(serv_addr));
    bzero( buffer, 256 );
    //printf("%s", &buffer);
    
    /* przypisujemy parametry *****************************************/
    serv_addr.sin_family = AF_INET;			// typ gniazda 
    serv_addr.sin_port = htons(port);		// port
    
    bcopy( (char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length );
    
    /* nawiazujemy polaczenie *****************************************/
    //connect( fd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) );
    if( connect( fd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) 
        return error( 3, "connect()" );
    
    int i=0;
    while(text[i]!='\0')
    {
        buffer[i]=text[i];
        i++;
    }
	buffer[i++]='\0';
    n = write( fd, buffer, sizeof(buffer));
    if( n < 0 ) 	// sprawdzamy, czy wystapil blad ....
        perror( "write()" );
    
    return 1;
}
//======================================================================
static xmlrpc_value * sample_add(
			xmlrpc_env *   const envP,
			xmlrpc_value * const paramArrayP,
			void *         const serverInfo,
			void *         const channelInfo) {


    /* Parsujemy tablice z argumentami ********************************/
    /* ("ii") oznacza format danych, tutaj: 2 x int *******************/
    char * text;
    xmlrpc_decompose_value( envP, paramArrayP, "(s)", &text );
    if (envP->fault_occurred)
        return NULL;

    printf("Otrzymana wiadomosc: %s\n",text);
    
    char wynik[256];
    int i=0;
    int x =0;
    while(text[x] != '\0')
    {
        sprintf((char*)(wynik+i),"%02X", text[x]);
        x++;
        i+=2;
    }
    wynik[i++]='\0';
    printf("otrzymany wynik: %s\n",wynik);
    funkcja(wynik);
    /* Zwracamy wynik *************************************************/
    
    return xmlrpc_build_value(envP, "s", " ");
}



//======================================================================
int main( void ){

	/* zmienne predefiniowane *****************************************/
	int  port   = 12345;
    
	//------------------------------------------------------------------
    struct xmlrpc_method_info3 const methodInfo = {
        /* .methodName     = */ "sample.add",
        /* .methodFunction = */ &sample_add,
    };
    
	/* zmienne niezainicjowane ****************************************/
    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    /* inicjujemy serwer **********************************************/
    xmlrpc_env_init(&env);

	/* rejestrujemy zmienne srodowiskowe ******************************/
    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred){
        printf( "xmlrpc_registry_new(): %s\n", env.fault_string);
        exit(1);
    }

	/* rejestrujemy nasza metode/funkcje ******************************/
    xmlrpc_registry_add_method3( &env, registryP, &methodInfo );
    if (env.fault_occurred) {
        printf( "xmlrpc_registry_add_method3(): %s\n", env.fault_string );
        exit(1);
    }

	/* ustawiamy parametry serwera ************************************/
    serverparm.config_file_name = NULL;
    serverparm.registryP        = registryP;
    serverparm.port_number      = port;
    serverparm.log_file_name    = "/tmp/xmlrpc_log";

	/* uruchamiamy serwer *********************************************/
    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if( env.fault_occurred ){
        printf("xmlrpc_server_abyss(): %s\n", env.fault_string);
        exit(1);
    }

	/* konczymy dzialanie *********************************************/
    return 0;
}
//======================================================================


