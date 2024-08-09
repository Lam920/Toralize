#include "toralize.h"

/*
./toralized + IP: will connect to proxy server
*/

Req *request(const char *dstip, const int dstport)
{
    Req *req;
    req = (Req *)malloc(reqsize);

    req->vn = 4;
    req->cd = 1;
    req->dstport = htons(dstport);
    req->dstip = inet_addr(dstip);
    strncpy(req->userid, USERNAME, 8);

    return req;
}

int main(int argc, char *argv[])
{
    char *host; // host we will connect to
    int port;

    // socket fd
    int s;

    struct sockaddr_in sock;

    Req *req;
    Res *res;

    char buf[ressize];
    int success = 0;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    // SOCK_STREAM : TCP connection
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        perror("socket");
        return -1;
    }

    printf("Try to init socket\n");

    // init sock to connect
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // connect to server through socket, represent by fd: s
    if (connect(s, (struct sockaddr *)&sock, sizeof(sock)))
    {
        perror("connect");
        return -1;
    }

    printf("Connected to proxy\n");

    // Make request to our proxy
    req = request(host, port);

    // send REQUEST
    write(s, (struct req *)req, reqsize);

    memset(buf, 0, ressize);

    //read SOCKS4
    if (read(s, buf, ressize) < 1){
        perror("read");
        free(req);
        close(s);
        return -1;
    }

    res = (Res *)buf;

    success = (res->cd == 90);
    if (!success){
        fprintf(stderr, "Unable to traverse the proxy, error code: %d", res->cd);
        close(s);
        free(req);
        return -1;
    }

    printf("Successfully connected through the proxy %s:%d\n", host, port);

    char tmp[512];
    memset(tmp, 0, 512);
    snprintf(tmp, 511, "HEAD / HTTP/1.0\r\n"
                        "Host: www.networktechnology\r\n"
                        "\r\n");
    write(s, tmp, strlen(tmp));

    memset(tmp, 0, 512);
    read(s, tmp, 511);

    printf("%s", tmp);

    // close socket
    close(s);
    free(req);

    return 0;
}