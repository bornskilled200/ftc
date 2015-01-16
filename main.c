#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define UNUSED(expr) (void)(expr)
#define FORMAT "%-15.15s %-18.18s %-25.25s %-25.25s %-10s\n"

/* Flag set by --verbose. */
static int verbose_flag = 0;

static uv_udp_t   client;
static uv_timer_t timer_req;

static int timeout = 500;

static void cl_recv_cb (uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr,
                        unsigned flags)
{
    UNUSED(handle);
    UNUSED(flags);
    if (addr == NULL)
        return;
    if (nread < 0)
        return;

    uv_timer_again(&timer_req);

    char samantha_ip[45];
    uv_inet_ntop(addr->sa_family, &(((struct sockaddr_in *) addr)->sin_addr), samantha_ip, 29);

    char local[nread + 1]; // +1 for the null terminator
    memcpy(local, buf->base, nread);
    local[nread] = '\0';

    char *tok;
    tok = strtok(local, "\r\n");
    if (tok == NULL)
    {
        printf("error\n");
        return;
    }
    char *name=tok;

    tok = strtok(NULL, "\r\n");
    if (tok == NULL)
    {
        printf("error\n");
        return;
    }
    char *mac=tok;

    tok = strtok(NULL, "\r\n");
    if (tok == NULL)
    {
        putchar('\n');
        return;
    }
    // IF THERE IS A COLON AFTER THE STATUS CHARACTER, THERE IS A IP ADDRESS AFTER
    char *owner_ip=tok[1]==':'?&tok[2]:NULL;

    tok[1]='\0';
    char *status=tok;

    printf(FORMAT, name, mac, samantha_ip, owner_ip, status);
}

static void alloc_cb (uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    UNUSED(handle);
    UNUSED(suggested_size);

    static char slab[65536];

    buf->base = slab;
    buf->len  = sizeof(slab);
}


static void timeout_cb (uv_timer_t *handle)
{
    UNUSED(handle);

    uv_stop(uv_default_loop());
}

static void cl_send_cb (uv_udp_send_t *req, int status)
{
    UNUSED(req);
    UNUSED(status);

    uv_timer_start(&timer_req, timeout_cb, timeout, timeout);
}

int main (int argc, char **argv)
{
    int optionCharacter;
    int help_flag = 0;
    int port = 30303;

    while (1)
    {
        static struct option long_options[] = {
                /* These options set a flag. */
                {"verbose",   no_argument,       0, 'v'},
                {"help",      no_argument,       0, 'h'},
                {"port",      required_argument, 0, 'p'},
                {"timeout",   required_argument, 0, 't'},
                {"csv",       optional_argument, 0, 'c'},
                {"no-header", no_argument,       0, 'n'},
                {0,           0,                 0, 0}};
        /* getopt_long stores the option index here. */
        int                  option_index   = 0;

        optionCharacter = getopt_long(argc, argv, "vhpn:t:c::", long_options,
                                      &option_index); //: after a character makes it's argument required

        /* Detect the end of the options. */
        if (optionCharacter == -1)
            break;

        switch (optionCharacter)
        {
            case 0:
                break;

            case 'v':
                verbose_flag = 1;
                break;

            case 'p':
                port = atoi(optarg);
                break;

            case 'n':
                break;

            case 't':
                timeout = atoi(optarg);
                break;

            case 'c':
                break;

            case 'h':
                help_flag = 1;
                break;

            case '?':
                help_flag=1;
                break;

            default:
                abort();
        }
    }

    if (verbose_flag)
    {
        printf("Verbose %s\n", verbose_flag?"TRUE":"FALSE");
        printf("Help %s\n", help_flag?"TRUE":"FALSE");
        printf("Port %d\n", port);
        printf("Timeout %d\n", port);
    }

    if (help_flag)
    {
        printf("%-20.20s %s\n", "-v, --verbose", "verbose output, outputs all flags/variables that are modifiable");
        printf("%-20.20s %s\n", "-h, --help", "prints this message, also prints when given arguments are malformed");
        printf("%-20.20s %s\n", "-p, --port (default: 30303)", "port that Samantha modules listen for a discovery packet");
        printf("%-20.20s %s\n", "-t, --timeout (default: 500)", "stops listening after the timeout, in milliseconds, after the last packet that was recieved. If 0 then listen forever.");
        //printf("%-20.20s %s\n", "-c, --csv", "prints out as as csv with a comma as a delimitor as default");
        //printf("%-20.20s %s\n", "-n, --no-header", "does not print out the header (Name MAC ID Address...)");
        exit(0);
    }

    /*********************************************
    MAIN PROGRAM
    *********************************************/

    struct sockaddr_in addr;
    uv_udp_send_t      req;
    uv_buf_t           buf;


    uv_udp_init(uv_default_loop(), &client);

    uv_timer_init(uv_default_loop(), &timer_req);

    uv_ip4_addr("255.255.255.255", port, &addr);
    buf = uv_buf_init("D", 4);

    // LISTEN
    uv_udp_recv_start(&client, alloc_cb, cl_recv_cb);
    uv_udp_set_broadcast(&client, 1);

    // SEND PACKET
    printf(FORMAT, "Name", "MAC ID", "Address", "In Use Address", "Status");
    uv_udp_send(&req, &client, &buf, 1, (const struct sockaddr *) &addr, cl_send_cb);

    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}