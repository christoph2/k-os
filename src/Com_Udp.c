/*
 *   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <fcntl.h>
#include <WinSock2.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <wspiapi.h>

/* #define MCASTADDRV4    "234.5.6.7" */

#define MCASTADDRV4     "234.5.6.7" /* "239.255.255.250" */
#define MCASTADDRV6     "ff12::1"
#define MCASTPORT       "1968"      /* "25000" */
#define BUFSIZE         1024
#define DEFAULT_COUNT   500
#define DEFAULT_TTL     8

/* char *g_Port = DEFAULT_PORT; */

char * gInterface  = NULL,          /* Local address to bind to */
* gMulticast       = NULL,          /* Multicast group to join */
* gPort            = MCASTPORT;     /* Port number to use */
int gSocketType    = SOCK_DGRAM,    /* datagram */
    gProtocol      = IPPROTO_UDP,   /* UDP */
    gTtl           = DEFAULT_TTL;
char * gBindAddr   = NULL;

static SOCKET sock = INVALID_SOCKET;


int SetMulticastTtl(SOCKET s, int af, int ttl)
{
    char *  optval = NULL;
    int     optlevel,
            option,
            optlen,
            rc;

    rc = NO_ERROR;

    if (af == AF_INET) {
        /* Set the options for V4 */
        optlevel   = IPPROTO_IP;
        option     = IP_MULTICAST_TTL;
        optval     = (char *)&ttl;
        optlen     = sizeof(ttl);
    } else if (af == AF_INET6) {
        /* Set the options for V6 */
        optlevel   = IPPROTO_IPV6;
        option     = IPV6_MULTICAST_HOPS;
        optval     = (char *)&ttl;
        optlen     = sizeof(ttl);
    } else {
        fprintf(stderr, "Attemtping to set TTL for invalid address family!\n");
        rc = SOCKET_ERROR;
    }

    if (rc != SOCKET_ERROR) {
        /* Set the TTL value */
        rc = setsockopt(s, optlevel, option, optval, optlen);

        if (rc == SOCKET_ERROR) {
            fprintf(stderr, "SetMulticastTtl: setsockopt failed: %d\n", WSAGetLastError());
        } else {
            printf("Set multicast ttl to: %d\n", ttl);
        }
    }

    return rc;
}


struct addrinfo * ResolveAddress(char * addr, char * port, int af, int type, int proto)
{
    struct addrinfo hints,
    * res = NULL;
    int rc;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags     = ((addr) ? 0 : AI_PASSIVE);
    hints.ai_family    = af; /* ((addr) ? AF_UNSPEC : af); */
    hints.ai_socktype  = type;
    hints.ai_protocol  = proto;

    rc = getaddrinfo(addr, port, &hints, &res);

    if (rc != 0) {
        fprintf(stderr, "Invalid address %s, getaddrinfo failed: %d\n", addr, rc);
        return NULL;
    }

    return res;
}


int JoinMulticastGroup(SOCKET s, struct addrinfo * group, struct addrinfo * iface)
{
    struct ip_mreq      mreqv4;
    struct ipv6_mreq    mreqv6;
    char *              optval = NULL;
    int                 optlevel,
                        option,
                        optlen,
                        rc;

    rc = NO_ERROR;

    if (group->ai_family == AF_INET) {
        /* Setup the v4 option values and ip_mreq structure */
        optlevel   = IPPROTO_IP;
        option     = IP_ADD_MEMBERSHIP;
        optval     = (char *)&mreqv4;
        optlen     = sizeof(mreqv4);

        mreqv4.imr_multiaddr.s_addr    = ((SOCKADDR_IN *)group->ai_addr)->sin_addr.s_addr;
        mreqv4.imr_interface.s_addr    = ((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr;

    } else if (group->ai_family == AF_INET6) {
        /* Setup the v6 option values and ipv6_mreq structure */
        optlevel   = IPPROTO_IPV6;
        option     = IPV6_ADD_MEMBERSHIP;
        optval     = (char *)&mreqv6;
        optlen     = sizeof(mreqv6);

        mreqv6.ipv6mr_multiaddr    = ((SOCKADDR_IN6 *)group->ai_addr)->sin6_addr;
        mreqv6.ipv6mr_interface    = ((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id;
    } else {
        fprintf(stderr, "Attemtping to join multicast group for invalid address family!\n");
        rc = SOCKET_ERROR;
    }

    if (rc != SOCKET_ERROR) {
        /* Join the group */
        rc = setsockopt(s, optlevel, option, optval, optlen);

        if (rc == SOCKET_ERROR) {
            fprintf(stderr, "JoinMulticastGroup: setsockopt failed: %d\n", WSAGetLastError());
        } else {

        }
    }

    return rc;
}


int SetMulticastLoopBack(SOCKET s, int af, int loopval)
{
    char *  optval = NULL;
    int     optlevel, option, optlen, rc;

    rc = NO_ERROR;

    if (af == AF_INET) {
        /* Set the v4 options */
        optlevel   = IPPROTO_IP;
        option     = IP_MULTICAST_LOOP;
        optval     = (char *)&loopval;
        optlen     = sizeof(loopval);
    }  else if (af == AF_INET6) {
        /* Set the v6 options */
        optlevel   = IPPROTO_IPV6;
        option     = IPV6_MULTICAST_LOOP;
        optval     = (char *)&loopval;
        optlen     = sizeof(loopval);
    } else {
        fprintf(stderr, "Attemtping to set multicast loopback for invalid address family!\n");
        rc = SOCKET_ERROR;
    }

    if (rc != SOCKET_ERROR) {
        /* Set the multpoint loopback */
        rc = setsockopt(s, optlevel, option, optval, optlen);

        if (rc == SOCKET_ERROR) {
            fprintf(stderr, "SetMulticastLoopBack: setsockopt failed: %d\n", WSAGetLastError());
        } else {
            printf("Setting multicast loopback to: %d\n", loopval);
        }
    }

    return rc;
}


int SetSendInterface(SOCKET s, struct addrinfo * iface)
{
    char *  optval = NULL;
    int     optlevel,
            option,
            optlen,
            rc;

    rc = NO_ERROR;

    if (iface->ai_family == AF_INET) {
        /* Setup the v4 option values */
        optlevel   = IPPROTO_IP;
        option     = IP_MULTICAST_IF;
        optval     = (char *)&((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr;
        optlen     = sizeof(((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr);
    } else if (iface->ai_family == AF_INET6) {
        /* Setup the v6 option values */
        optlevel   = IPPROTO_IPV6;
        option     = IPV6_MULTICAST_IF;
        optval     = (char *)&((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id;
        optlen     = sizeof(((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id);
    } else {
        fprintf(stderr, "Attemtping to set sent interface for invalid address family!\n");
        rc = SOCKET_ERROR;
    }

    /* Set send IF */
    if (rc != SOCKET_ERROR) {
        /* Set the send interface */
        rc = setsockopt(s, optlevel, option, optval, optlen);

        if (rc == SOCKET_ERROR) {
            fprintf(stderr, "setsockopt failed: %d\n", WSAGetLastError());
        } else {

        }
    }

    return rc;
}


int     gCount = DEFAULT_COUNT, gLoopBack = 0;
char    buf[BUFSIZE];

void StartCOMExtension(void)
{
    WSADATA             wsdata;
    SOCKET              rs         = INVALID_SOCKET;
    struct addrinfo *   reslocal   = NULL,
    * resmulti                     = NULL;
    struct addrinfo *   addrlocal  = NULL;
    int                 rc;
    char *              optval     = NULL;
    char                bSender    = TRUE;
    BOOL                bLoopBack  = FALSE;
    int                 i;
    char out[128];

    out[0] = 'A';
    out[1] = 'B';
    out[2] = 'C';
    out[3] = 'D';
    out[4] = 'E';
    out[5] = 'F';
    out[6] = 'G';
    out[7] = 'H';

#if 0
    SsdpSock   = socket(AF_INET, SOCK_DGRAM, 0);
    val        = fcntl(SsdpSock, F_GETFL, 0);
    fcntl(SsdpSock, F_SETFL, val | O_NONBLOCK);
#endif

    if ((rc = WSAStartup(MAKEWORD(2, 2), &wsdata)) != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", rc);
        return;
    }

    reslocal = ResolveAddress(gInterface, (bSender ? "0" : gPort), AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);

    if (reslocal == NULL) {
        fprintf(stderr, "Unable to resolve local interface: %s\n", gInterface);
        goto cleanup;
    }

    if (gMulticast == NULL) {
        if (reslocal->ai_family == AF_INET) {
            gMulticast = MCASTADDRV4;
        } else if (reslocal->ai_family == AF_INET6) {
            gMulticast = MCASTADDRV6;
        }
    }

    /* Create the socket - remember to specify the multicast flags */
    sock = WSASocket(reslocal->ai_family, reslocal->ai_socktype, reslocal->ai_protocol, NULL, 0,
                     WSA_FLAG_OVERLAPPED | WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF);

    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket(af = %d) failed: %d\n", reslocal->ai_family, WSAGetLastError());
        goto cleanup;
    }

    rc = bind(sock, reslocal->ai_addr, reslocal->ai_addrlen);

    if (rc == SOCKET_ERROR) {
        fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    /* Resolve the multicast address */
    resmulti = ResolveAddress(gMulticast, gPort, reslocal->ai_family, reslocal->ai_socktype, reslocal->ai_protocol);

    if (resmulti == NULL) {
        fprintf(stderr, "Unable to resolve multicast address: %s\n", gMulticast);
        goto cleanup;
    }

    /* Set the multicast TTL */
    rc = SetMulticastTtl(sock, resmulti->ai_family, gTtl);

    if (rc == SOCKET_ERROR) {
        fprintf(stderr, "Unable to set multicast TTL\n");
        goto cleanup;
    }

    /* Set the loopback value if indicated */
    if (bLoopBack) {
        rc = SetMulticastLoopBack(sock, resmulti->ai_family, gLoopBack);

        if (rc == SOCKET_ERROR) {
            fprintf(stderr, "Unable to set multicast loopback flag\n");
            goto cleanup;
        }
    }

    rs = WSAJoinLeaf(sock, resmulti->ai_addr, resmulti->ai_addrlen, NULL, NULL, NULL, NULL, JL_BOTH); /*(bSender ? JL_SENDER_ONLY : JL_RECEIVER_ONLY)*/

    if (rs == INVALID_SOCKET) {
        fprintf(stderr, "WSAJoinLeaf failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    /* Don't need these anymore */
    freeaddrinfo(reslocal);
    reslocal = NULL;

    freeaddrinfo(resmulti);
    resmulti = NULL;

    ZeroMemory(buf, BUFSIZE);

#if 0

    if (bSender) {
        for(i = 0; i < gCount; i++) {
            rc = send(sock, buf, BUFSIZE, 0);

            if (rc == SOCKET_ERROR) {
                fprintf(stderr, "send failed: %d\n", WSAGetLastError());
                break;
            } else {
                printf("wrote %d bytes\n", rc);
            }
        }
    } else {
        for(i = 0; i < gCount; i++) {
            rc = recv(sock, buf, BUFSIZE, 0);

            if (rc == SOCKET_ERROR) {
                fprintf(stderr, "recv failed: %d\n", rc);
                break;
            } else {
                printf("read %d bytes\n", rc);
            }
        }
    }

#endif
cleanup:

    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }

    if (reslocal) {
        freeaddrinfo(reslocal);
    }

    if (resmulti) {
        freeaddrinfo(resmulti);
    }

    WSACleanup();
}


int ComImpl_Send(char const * buffer, int size)
{
    int rc;

    rc = send(sock, buffer, size, 0);
    return rc;
}
