#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define UNUSED(expr) (void)(expr)
#define FORMAT "%-15.15s %-17.17s %-25.25s %-25.25s %-10s\n"

/* Flag set by --verbose. */
static int verbose_flag;

uv_udp_t client;
uv_timer_t timer_req;

static void cl_recv_cb (uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr,
                        unsigned flags)
{
    UNUSED(handle);
    UNUSED(flags);
    if (addr == NULL)
        return;
    if (nread<0)
        return;

    printf(FORMAT, "Name", "MAC ID", "Address", "In Use Address", "Full Response");
    fwrite(buf->base, sizeof(char), (size_t) nread, stdout);
    putchar('\n');

    uv_timer_again(&timer_req);
}

static void alloc_cb (uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    UNUSED(handle);
    UNUSED(suggested_size);

    static char slab[65536];

    buf->base = slab;
    buf->len  = sizeof(slab);
}


static void timeout_cb(uv_timer_t* handle){
    UNUSED(handle);

    uv_stop(uv_default_loop());
}

static void cl_send_cb (uv_udp_send_t *req, int status)
{
    UNUSED(req);
    UNUSED(status);

    uv_timer_start(&timer_req, timeout_cb, 500, 500);
}

int main (int argc, char **argv)
{
    int c;

    while (1)
    {
        static struct option long_options[] = {
                /* These options set a flag. */
                {"verbose", no_argument, &verbose_flag, 1}, {"brief", no_argument, &verbose_flag, 0},
                /* These options don't set a flag.
                   We distinguish them by their indices. */
                {"add", no_argument, 0, 'a'}, {"append", no_argument, 0, 'b'}, {"delete", required_argument, 0, 'd'}, {"create", required_argument, 0, 'c'}, {"file", required_argument, 0, 'f'}, {0, 0, 0, 0}};
        /* getopt_long stores the option index here. */
        int                  option_index   = 0;

        c = getopt_long(argc, argv, "abc:d:f:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'a':
                puts("option -a\n");
                break;

            case 'b':
                puts("option -b\n");
                break;

            case 'c':
                printf("option -c with value `%s'\n", optarg);
                break;

            case 'd':
                printf("option -d with value `%s'\n", optarg);
                break;

            case 'f':
                printf("option -f with value `%s'\n", optarg);
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort();
        }
    }

    /* Instead of reporting --verbose
       and --brief as they are encountered,
       we report the final status resulting from them. */
    if (verbose_flag)
        puts("verbose flag is set");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
    }


    /*********************************************
    MAIN PROGRAM
    *********************************************/

    struct sockaddr_in addr;
    uv_udp_send_t      req;
    uv_buf_t           buf;


    uv_udp_init(uv_default_loop(), &client);

    uv_timer_init(uv_default_loop(), &timer_req);

    uv_ip4_addr("255.255.255.255", 30303, &addr);
    buf = uv_buf_init("D", 4);

    // LISTEN
    uv_udp_recv_start(&client, alloc_cb, cl_recv_cb);
    uv_udp_set_broadcast(&client, 1);

    // SEND PACKET
    uv_udp_send(&req, &client, &buf, 1, (const struct sockaddr *) &addr, cl_send_cb);

    return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}