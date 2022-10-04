#include <stdio.h>
#include <miniSystem.hpp>
#include <console.hpp>
#include <stream.hpp>
#include <string>

static void fault_handler(void *, const char *msg)
{
    fprintf(stderr, "FATAL: %s\n", (msg ? msg : "no message."));
    fflush(stderr);
    exit(1);
}

class ConsoleIf : public StreamIf<std::string>
{
public:
    bool readable = false;
    bool writable = true;

    ConsoleIf() {}
    ~ConsoleIf() {}

    void write(const std::string &src)
    {
        printf("%s\n", src.c_str());
    }

    void read(std::string &dst)
    {
    }
};

int main()
{

    consoleStream.init(new ConsoleIf());

    MiniSystem ms(fault_handler);

    printf("Result: %d.\n", ms.eval("console.log('Hello world!')"));
}