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

class LinuxFile : public FileIf
{
private:
    FILE *file;

public:
    LinuxFile(){};
    ~LinuxFile(){};

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
        LinuxFile *file = new LinuxFile;
        file->open(path, options);

        return (FileIf *)file;
    }
};

class Greeter : public ModuleIf
{
public:
    Greeter(){};
    ~Greeter(){};

    bool bindClass(WrenForeignClassMethods *methods, std::string module, std::string name)
    {
        methods->allocate = nullptr;
        methods->finalize = nullptr;

        if (name == "Greeter")
            return true;
        else
            return false;
    }
    WrenForeignMethodFn bindMethods(std::string name, bool isStatic, std::string sig)
    {
        if (name == "Greeter" && isStatic && sig == "greet(_)")
        {
            return Greeter::greet;
        }

        return nullptr;
    }
    WrenLoadModuleResult loadModule(WrenVM *vm)
    {
        WrenLoadModuleResult result;

        result.source = "foreign class Greeter {\n"
                        "   foreign static greet(who)\n"
                        "}";

        return result;
    }

    static void greet(WrenVM *vm)
    {
        printf("Hello %s\n", wrenGetSlotString(vm, 1));
    }
};

int main()
{
    ModuleManager *mman = new ModuleManager;

    mman->registerIf(new LinuxConsole);
    mman->registerIf(new LinuxFS);

    MiniSystem ms(mman);

    printf("Result: %d.\n", ms.eval("import \"FS\""));
    printf("Result: %d.\n", ms.eval("var file = File.open(\"../LICENSE\", \"r\")"));
    printf("Result: %d.\n", ms.eval("System.print(file.read(11))"));
    printf("Result: %d.\n", ms.eval("file.close()"));
}