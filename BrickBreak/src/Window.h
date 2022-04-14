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

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }

    void clear();
    void update();
    bool shouldClose() const;
private:
    static void windowSizeCallback(GLFWwindow* window, int w, int h);
};
