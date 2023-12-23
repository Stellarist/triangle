#include <string>
#include <glad/glad.h>

class Texture{
private:
    unsigned int id;
    std::string  path;
    std::string  type;

    int width;
    int height;
    int nrComponents;

public:
    Texture(const std::string& path, const std::string& type="");
    ~Texture();

    void bind(unsigned int slot) const;
    void unbind() const;    

    int getId() const {return id;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    int getNrComponents() const {return nrComponents;}
    std::string getPath() const {return path;}
    std::string getType() const {return type;}

    static void flipVertically(bool flip=false);
friend class Mesh;
};
