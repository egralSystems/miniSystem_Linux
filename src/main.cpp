#include <stdio.h>
#include <string>

#include <miniSystem.hpp>
#include <stream.hpp>
#include <fs.hpp>

class LinuxConsole : public StreamIf<std::string>
{
public:
    bool readable = false;
    bool writable = true;

    LinuxConsole() {}
    ~LinuxConsole() {}

    void write(const std::string &src)
    {
        printf("%s", src.c_str());
    }

    void read(std::string &dst)
    {
    }
};

class LinuxFileIf : public FileIf
{
private:
    FILE *file;

public:
    LinuxFileIf(){};
    ~LinuxFileIf(){};

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

class LinuxFS : public FSIf
{
public:
    FileIf *open(std::string path, std::string options)
    {
        LinuxFileIf *file = new LinuxFileIf;
        file->open(path, options);

        return (FileIf *)file;
    }
};

int main()
{
    auto fileDef = "foreign class File {\nconstruct open(path, options) {}\nforeign close()\nforeign read(count)\n}";

    IO *io = new IO;

    io->registerIf(new LinuxConsole);
    io->registerIf(new LinuxFS);

    MiniSystem ms(io);

    printf("Result: %d.\n", ms.eval(fileDef));
    printf("Result: %d.\n", ms.eval("var file = File.open(\"../LICENSE\", \"r\")"));
    printf("Result: %d.\n", ms.eval("System.print(file.read(11))"));
    printf("Result: %d.\n", ms.eval("file.close()"));
}