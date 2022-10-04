#include <stdio.h>
#include <miniSystem.hpp>
#include <stream.hpp>
#include <string>

class ConsoleIf : public StreamIf<std::string>
{
public:
    bool readable = false;
    bool writable = true;

    ConsoleIf() {}
    ~ConsoleIf() {}

    void write(const std::string &src)
    {
        printf("%s", src.c_str());
    }

    void read(std::string &dst)
    {
    }
};

int main()
{
    MiniSystem ms(new ConsoleIf());

    printf("Result: %d.\n", ms.eval("System.print(\"Hello world!\")"));
}