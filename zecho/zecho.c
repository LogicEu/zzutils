#include <unistd.h>
#include <errno.h>

#define EXIT_SUCCESS 0

extern char* strerror(int err);

static char* zprogname;

static size_t zstrlen(const char* str)
{
    size_t i;
    for (i = 0; str[i]; ++i);
    return i;
}

static void zstrrmchar(char* str)
{
    while (*str) {
        str[0] = str[1];
        ++str;
    }
}

static size_t zpreecho(char* str)
{
    size_t i;
    
    for (i = 0; str[i]; ++i) {
        if (str[i] == '\\') {
             switch (str[i + 1]) {
                case 'r': zstrrmchar(str + i); str[i] = '\r'; break;
                case 'n': zstrrmchar(str + i); str[i] = '\n'; break;
                case 't': zstrrmchar(str + i); str[i] = '\t'; break;
                case '\\': zstrrmchar(str + i + 1); break;
                default: break;
             }
        }
    }

    return i;
}

static int zerror(void)
{
    ssize_t ret;
    const char* errstr = strerror(errno);
    ret = write(STDERR_FILENO, zprogname, zstrlen(zprogname));
    if (ret == -1) {
        return errno;
    }

    ret = write(STDERR_FILENO, ": ", 2);
    if (ret == -1) {
        return errno;
    }

    ret = write(STDERR_FILENO, errstr, zstrlen(errstr));
    if (ret == -1) {
        return errno;
    }

    ret = write(STDERR_FILENO, "\n", 1);
    if (ret == -1) {
        return errno;
    }

    return errno;
}

static ssize_t zecho(char* str)
{
    size_t len = zpreecho(str);
    return write(STDOUT_FILENO, str, len);
}

int main(int argc, char** argv)
{
    int i;
    
    zprogname = argv[0];

    for (i = 1; i < argc; ++i) {
        if (i > 1 && write(STDOUT_FILENO, " ", 1) == -1) {
            return zerror();
        }

        if (zecho(argv[i]) == -1) {
            return zerror();
        }
    }

    if (write(STDOUT_FILENO, "\n", 1) == -1) {
        return zerror();
    }

    return EXIT_SUCCESS;
}

