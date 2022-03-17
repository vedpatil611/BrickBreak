#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    int m_Width;
    int m_Height;
    GLFWwindow* m_Window;
public:
    Window(int width, int height);
    ~Window();

    void clear();
    void update();
    bool shouldClose() const;
};
