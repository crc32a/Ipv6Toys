 Lookup.c
        traditionally you would connect to a host
        via the gethostbyname or connect to a raw ip
       via gethostbyaddr call.
       With Ipv6 in the mix now you need to use getaddrinfo() and getnameinfo() to be
       more portable and compatible with Ipv6. 
       This program demonstrates lookups for both Ipv4 and Ipv6 hosts by using 
       int getaddrinfo(const char *node, const char *service,
 const struct addrinfo *hints,
                       
            struct addrinfo **res);


         I use null for service, but apparently the address returned by what ever resolver your using can use the servicename to deduce the address based on the servicename. How this would work in DNS is beyond my current understanding. :( so I pass in null since I'm still living in the world  where hostnames map to ip addresses with out knowledge of the port your connecting too.

     If you want the Ipv6 address  set hints.af_family to AF_INET6 or AF_INET for Ipv4. If you flat out don't care then use AF_UNSPEC.  I likewise don't know why the resolver woul need knowledge of weither your attempting to connect via UDP or TCP in terms of ai_socktype but its cool if giving those hints in the future helps guide the lookup.  In my case I hard coded it to SOCK_STREAM for TCP.  

// Finish this readme
