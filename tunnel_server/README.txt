lookup.c Looks up the sockaddrinfo via getaddrinfo sys call. 
http_hello.c  spins up a non daemonized non complient HTTP server. that just spits the message specified on the command line. Usefull
              for listening on either IPv6 or IPv4.

tunnel_server.c spins up a man in the middle socket forwarder.
                You know the kind that when data is read from the client
                it is written to the forwarded server. It can listen on
                and send data on IPv6 or IPv4. It was mostly used for testing
                IPv6 if you for what ever reason can't get a Hurricain 
                electric IPv6 tunnel. (If your on AT&T Uverse you can't
                since Uverbose blocks IP protocol 41. :(

ringcharbuff.c  Just a datastructure that buffers read data from clients.

socktools.c Just prints out what protocol af_faimly or sock time an 
            addrinfo object is useing

echo_client.c and echo_server.c
    Just echos data back to the user. Not really usefull.


