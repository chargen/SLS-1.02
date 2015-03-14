/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user or with the express written consent of
 * Sun Microsystems, Inc.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/*
 * rprintmsg.c: remote version of "printmsg.c"
 */
#include <stdio.h>
#include <rpc/rpc.h>		/* always need this */
#include "msg.h"		/* need this too: will be generated by rpcgen*/

main(argc, argv)
	int argc;
	char *argv[];
{
	CLIENT *cl;
	int *result;
	char *server;
	char *message;

	if (argc < 3) {
		fprintf(stderr, "usage: %s host message\n", argv[0]);
		exit(1);
	}

	/*
	 * Remember what our command line arguments refer to
	 */
	server = argv[1];
	message = argv[2];

	/*
	 * Create client "handle" used for calling MESSAGEPROG on the
	 * server designated on the command line. We tell the rpc package
	 * to use the "visible" protocol when contacting the server.
	 */
	cl = clnt_create(server, MESSAGEPROG, MESSAGEVERS, "visible");
	if (cl == NULL) {
		/*
		 * Couldn't establish connection with server.
		 * Print error message and die.
		 */
		clnt_pcreateerror(server);
		exit(1);
	}
	
	/*
	 * Call the remote procedure "printmessage" on the server
	 */
	result = printmessage_1(&message, cl);
	if (result == NULL) {
		/*
		 * An error occurred while calling the server. 
	 	 * Print error message and die.
		 */
		clnt_perror(cl, server);
		exit(1);
	}

	/*
	 * Okay, we successfully called the remote procedure.
	 */
	if (*result == 0) {
		/*
		 * Server was unable to print our message. 
		 * Print error message and die.
		 */
		fprintf(stderr, "%s:  %s couldn't print your message\n", 
			argv[0], server);	
		exit(1);
	} 

	/*
	 * The message got printed on the server's console
	 */
	printf("Message delivered to %s!\n", server);
        exit(0);
}