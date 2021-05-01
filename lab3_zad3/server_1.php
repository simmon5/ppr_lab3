#!/usr/bin/php

<?php
	#===================================================================
	# Wersja z wywolaniami zblizonymi do C
	#===================================================================
	
	# zmienne predefiniowane -------------------------------------------
	$host = "127.0.0.1";
	$port = 8000;
	
	# tworzymy gniazdo -------------------------------------------------
	if( ! ( $server = socket_create( AF_INET, SOCK_STREAM, SOL_TCP ) ) ){
		print "socket_create(): " 		. socket_strerror( socket_last_error( $server ) ) . "\n";
		exit( 1 );
	}
	
	# ustawiamy opcje gniazda (REUSEADDR) ------------------------------
	if( ! socket_set_option($server, SOL_SOCKET, SO_REUSEADDR, 1) ) {
		print "socket_set_option(): " 	. socket_strerror(socket_last_error( $server ) ) . "\n";
		exit( 1 );
	}
	
	# mapujemy gniazdo (na port) ---------------------------------------
	if( ! socket_bind( $server, $host, $port ) ){
		print "socket_bind(): " 		. socket_strerror( socket_last_error( $server ) ) . "\n";
		exit( 1 );
	}
	
	# ustawiamy gniazdo w tryb nasluchiwania ---------------------------
	if( ! socket_listen( $server, 5 ) ){
		print "socket_listen(): " 		. socket_strerror( socket_last_error( $server ) ) . "\n";
		exit( 1 );
	}
	
	# obslugujemy kolejnych klientow, jak tylko sie podlacza -----------
	while( $client = socket_accept( $server ) ){
		
		# wyswietlamy informacje o polaczeniu  - - - - - - - - - - - - -
		socket_getpeername( $client, $addr, $port );
		print "Addres: $addr Port: $port\n";
		
		# przekazujemy informacje o biezacym czasie  - - - - - - - - - -
		$msg = ""; 
		#print("test");
        #while( $rcv = socket_read( $client, 1)){ 
        #    $msg .= $rcv;
        #    print("$msg\n");
        #}
        $msg = socket_read( $client, 256);
		print "Otrzymana wiadomosc: $msg\n";
		socket_close( $client );
	}
	#-------------------------------------------------------------------
	socket_close( $server );
	#===================================================================
?>
