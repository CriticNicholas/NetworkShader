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

        std::string VertexString = VertexFile.ReadFile();
        std::string FragmentString = FragmentFile.ReadFile();

        Shader* ActiveShader = new Shader(VertexString, FragmentString);

        KeyCallback('S', [this, &VertexString, &FragmentString]() {std::thread([this, &VertexString, &FragmentString] {
            this->Reader(&FragmentString, &VertexString);
            }).detach();
            });



        Start([this, ActiveShader, &FragmentString, &VertexString]() {
            this->DrawArrays(ActiveShader, &VertexString, &FragmentString);
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

    void DrawArrays(Shader* ActiveShader, std::string* VertexString, std::string* FragmentString)
    {
        UseVertives(Vertices);
        while (!glfwWindowShouldClose(ObjectWindow))
        {
            if(ReceivedStatus)
            {
                Shader NewShader(*VertexString, *FragmentString);

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

    void Reader(std::string *FragmentString, std::string *VertexString)
    {
           Host();

           Run();
           while (true)
           {
               *FragmentString = Read();
               //VertexString = Read();

               ReceivedStatus = 1;

           }
    }

    void Sender(File* FragmentFile)
    {
        Connect();

        Run();

        Send(FragmentFile->ReadFile());

        while (true)
        {
            if (FragmentFile->FileChange())
            {
                Send(FragmentFile->ReadFile());
            }
        }
    }

    unsigned Width;
    unsigned Height;

    std::atomic<bool> ReceivedStatus = 1;
};