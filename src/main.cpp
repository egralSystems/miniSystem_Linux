#include <stdio.h>
#include <string>

#include <miniSystem.hpp>
#include <stream.hpp>
#include <fs.hpp>

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

class FSIf : public FileIf
{
private:
    FILE *file;

public:
    FSIf(){};
    ~FSIf(){};

    void open(std::string path, std::string accessType)
    {
        file = fopen(path.c_str(), accessType.c_str());
    }

    void close()
    {
        if (file)
            fclose(file);
    }

    std::string read(int charCount)
    {
        std::string buf(charCount, '\0');

        fread(&buf[0], sizeof(char), charCount, file);

        return buf;
    }

    void write(std::string toWrite)
    {
        fwrite(toWrite.c_str(), toWrite.size(), 0, file);
    }

    int size()
    {
        int size = 0;

        if (file)
        {
            fseek(file, 0L, SEEK_END);
            size = ftell(file);
            fseek(file, 0L, SEEK_SET);
        }

        return size;
    }

    void seek(int pos)
    {
        fseek(file, pos, SEEK_SET);
    }
};

int main()
{
    MiniSystem<ConsoleIf, FSIf> ms = MiniSystem<ConsoleIf, FSIf>();

    printf("Result: %d.\n", ms.eval("System.print(\"Hello world!\")"));
}