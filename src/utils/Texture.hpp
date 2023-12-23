#include <string>
#include <glad/glad.h>

class Texture{
private:
    unsigned int id;
    std::string  path;
    std::string  type;

public:
    Texture(const std::string& path, const std::string& type="");
    ~Texture();

    void bind(unsigned int slot) const;
    void unbind() const;    

    int getId() const {return id;}
    unsigned int& getId() {return id;}
    std::string getPath() const {return path;}
    std::string getType() const {return type;}
    std::string getName() const {return path.substr(path.find_last_of('/')+1);}

    static void flipVertically(bool flip=false);
};
