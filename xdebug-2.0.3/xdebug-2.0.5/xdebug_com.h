/*
   +----------------------------------------------------------------------+
   | Xdebug                                                               |
   +----------------------------------------------------------------------+
   | Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007 Derick Rethans      |
   +----------------------------------------------------------------------+
   | This source file is subject to version 1.0 of the Xdebug license,    |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://xdebug.derickrethans.nl/license.php                           |
   | If you did not receive a copy of the Xdebug license and are unable   |
   | to obtain it through the world-wide-web, please send a note to       |
   | xdebug@derickrethans.nl so we can mail you a copy immediately.       |
   +----------------------------------------------------------------------+
   | Authors:  Derick Rethans <derick@xdebug.org>                         |
   +----------------------------------------------------------------------+
 */

#ifndef __HAVE_XDEBUG_COM_H__
#define __HAVE_XDEBUG_COM_H__

#ifdef PHP_WIN32
int inet_aton(const char *cp, struct in_addr *inp);
#endif

#if WIN32|WINNT
# define SOCK_ERR INVALID_SOCKET
# define SOCK_CONN_ERR SOCKET_ERROR
# define SOCK_RECV_ERR SOCKET_ERROR
#else
# define SOCK_ERR -1
# define SOCK_CONN_ERR -1
# define SOCK_RECV_ERR -1
#endif

#if WIN32|WINNT
#define SCLOSE(a) closesocket(a)
#define SSENDL(a,b,c) send(a,b,c,0)
#define SSEND(a,b) send(a,b,strlen(b),0)
#define SREAD(a,b,c) recv(a,b,c,0)
#else
#define SCLOSE(a) close(a)
#define SSENDL(a,b,c) write(a,b,c)
#define SSEND(a,b) write(a,b,strlen(b))
#define SREAD(a,b,c) read(a,b,c)
#endif


#define SENDMSG(socket, str) {  \
	char *message_buffer;       \
                                \
	message_buffer = str;       \
	SSEND(socket, message_buffer); \
	xdfree(message_buffer);     \
}


int xdebug_create_socket(const char *hostname, int dport);
void xdebug_close_socket(int socket);

#endif
