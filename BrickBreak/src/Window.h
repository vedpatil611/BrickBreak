#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    int m_Width;
    int m_Height;
    GLFWwindow* m_Window;
    bool m_Input[1024];
public:
    Window(int width, int height);
    ~Window();

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
    inline const bool* getInputs() const { return m_Input; }
    
    void clear();
    void update();
    bool shouldClose() const;
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void windowSizeCallback(GLFWwindow* window, int w, int h);
};
