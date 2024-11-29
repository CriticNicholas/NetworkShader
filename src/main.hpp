#include"File/File.hpp"

#include"Graphics/Graphics.hpp"
#include"Shaders/Shader.hpp"

#include"Network/Network.hpp"

class Main : public Window, Socket
{
public:
    Main(std::string_view WindowName, int Width, int Height, std::string_view IpV4, int Port) : 
        Window(Width, Height, WindowName), Socket(IpV4, Port), Width(Width), Height(Height)
    {

        File VertexFile("vertex.shader");
        File FragmentFile("fragment.shader");

        VertexString = VertexFile.ReadFile();
        FragmentString = FragmentFile.ReadFile();

        Shader* ActiveShader = new Shader(VertexString, FragmentString);

        KeyCallback('S', [this]() {std::thread([this] {
            this->Reader();
            }).detach();
            });

        KeyCallback('C', [this, &FragmentFile]() {std::thread([this, &FragmentFile] {
            this->Sender(&FragmentFile);
            }).detach();
            });

        KeyCallback(GLFW_KEY_ESCAPE, [this]() {AsioSocket.close(); std::cout << "TEST"; });

        Start([this, ActiveShader]() {
            this->DrawArrays(ActiveShader);
            });
    }

private:
    float Vertices[12]
    {
        -1, 1,
        -1, -1,
        1, 1,

        1, 1,
        1, -1,
        -1, -1,
    };

    void DrawArrays(Shader* ActiveShader)
    {
        UseVertives(Vertices);
        while (!glfwWindowShouldClose(ObjectWindow))
        {
            if(ReceivedStatus)
            {
                Shader NewShader(VertexString, FragmentString);

                Shader* OldShader = ActiveShader;

                ActiveShader = &NewShader;

                ActiveShader->Use();

                glUniform2f(glGetUniformLocation(ActiveShader->ProgramId, "iResolution"), this->Width, this->Height);
                OldShader->~Shader();

                ReceivedStatus = 0;
            }

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glUniform1f(glGetUniformLocation(ActiveShader->ProgramId, "iTime"), glfwGetTime());

            glfwSwapBuffers(ObjectWindow);
            glfwPollEvents();
        }
    }

    void Reader()
    {
           Host();

           Run();
           while (true)
           {
               FragmentString = Read();
               //VertexString = Read();

               ReceivedStatus = 1;

           }
    }

    void Sender(File* FragmentFile)
    {
        Connect();

        //Run();
        Send(FragmentFile->ReadFile() + '\0');

        while (true)
        {
            if (FragmentFile->FileChange())
            {
                Send(FragmentFile->ReadFile());

                FragmentString = FragmentFile->ReadFile();
                ReceivedStatus = 1;
            }

            Sleep(100);
        }
    }

    unsigned Width;
    unsigned Height;

    std::string VertexString;
    std::string FragmentString;

    std::atomic<bool> ReceivedStatus = 1;
};
