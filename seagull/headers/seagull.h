#ifndef SEAGULL_HEADER_FILE
#define SEAGULL_HEADER_FILE

#include "profiler.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/version.h>

#include "shaders.h"
#include "mesh.h"
#include "model.h"
#include "camera.h"
#include "Utils.h"
#include "Seagull_Ui.h"
#include "filesystem.h"
#include "ResourceManager.h"



// The virtual functions allow the functions to be overriden in another class.
namespace Seagull
{
    class SeagullEngine
    {
        public:

            std::string ApplicationName;
            static int CurrentWidth;
            static int CurrentHeight;
            int CurrentWindowWidth;
            int CurrentWindowHeight;

            GLFWwindow *window;

            double previousTime;

            UI seagullUi;

            static Camera *camera;
            static float lastX;
            static float lastY;
            static bool firstMouse;
            static bool mouseControl;

            // timing
            float deltaTime;
            double lastFrame;

            SeagullEngine(const char *ApplicationName)
            {
                this->ApplicationName = ApplicationName;
                GLFWwindow *window = InitGLFWWindow();
                InitializeGL(window);
            }
            virtual ~SeagullEngine() {}

            virtual void Init() {}
            virtual void OnUpdate(float deltaTime) {}
            virtual void OnRender() {}
            virtual void OnImGuiRender() {}

            virtual void ProcessSingleKeypress() {}
            virtual void ProcessInput() 
            {
                processInput(window);
            }

        private:

            /*
             * @Brief: Processes user keypress
             * This function takes user input and assigns actions to the keypresses.
             * This is useful for when a key should only be activated once while pressed down.
             *
             * NOTE: In future, make it so that this can be easily modified.
             */
            static void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods)
            {
                SGL_PROFILE_FUNCTION();
                if (action == GLFW_PRESS) {
                    switch (key) {
                        case GLFW_KEY_ESCAPE:
                            mouseControl = (mouseControl == false ? true : false);
                            if (!mouseControl)
                                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                            else
                            {
                                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                                glfwSetCursorPos(window, lastX, lastY);
                            }
                            break;
                    }
                }

            }

            void processInput(GLFWwindow *window)
            {
                SGL_PROFILE_FUNCTION();
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    camera->ProcessKeyboard(FORWARD, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    camera->ProcessKeyboard(BACKWARD, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    camera->ProcessKeyboard(LEFT, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    camera->ProcessKeyboard(RIGHT, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                    camera->ProcessKeyboard(UP, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                    camera->ProcessKeyboard(DOWN, deltaTime);
            }

            static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
            {
                SGL_PROFILE_FUNCTION();
                if (mouseControl)
                {
                    if (firstMouse)
                    {
                        lastX = xpos;
                        lastY = ypos;
                        firstMouse = false;
                    }

                    double xoffset = xpos - lastX;
                    double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                    lastX = xpos;
                    lastY = ypos;

                    camera->ProcessMouseMovement(xoffset, yoffset);
                }
            }

            /*
             * When window is resized, dimensions of OpenGL Viewport is set to the new
             * dimensions of the window.
             */
            static void windowResizeCallbackFunc(GLFWwindow *window, int width, int height)
            {
                SGL_PROFILE_FUNCTION();
                CurrentWidth = width;
                CurrentHeight = height;
                GLCall(glViewport(0, 0, CurrentWidth, CurrentHeight));
            }

            /*
             * Error callback function for GLFW.
             *
             * If an error occurs, it will be printed with the number and description.
             */
            static void GlfwErrorCallback(int error, const char *description)
            {
                SGL_PROFILE_FUNCTION();
                fprintf(stderr, "GLFW ERROR: [%d] %s\n", error, description);
            }


            /*
             * Creates a GLFW window ready for OpenGL
             * along with configuring some settings
             */
            GLFWwindow *InitGLFWWindow(void)
            {
                SGL_PROFILE_FUNCTION();

                /* Initialize the library */
                if (!glfwInit())
                    exit(EXIT_FAILURE);


                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                #ifdef __APPLE__
                    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                #endif

                /* Create a windowed mode window and its OpenGL context */
                this->window = glfwCreateWindow(CurrentWidth, CurrentHeight, ApplicationName.c_str(), NULL, NULL);
                if (!window)
                {
                    glfwTerminate();
                    exit(EXIT_FAILURE);
                }

                /* Make the window's context current */
                glfwMakeContextCurrent(window);
                fprintf(stdout, "INFO: \n"
                                "OpenGL Version: %s\n"
                                "GLFW Version: %s\n"
                                "Dear ImGui Version: %s\n"
                                "Assimp: %u.%u.%u\n", 
                                glGetString(GL_VERSION), glfwGetVersionString(), ImGui::GetVersion(),
                                aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionPatch());
                glfwSetFramebufferSizeCallback(window, windowResizeCallbackFunc);
                glfwSetErrorCallback(GlfwErrorCallback);
                glfwSetKeyCallback(window, processKeypress);
                glfwSetCursorPosCallback(window, mouse_callback);

                //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                //glfwSetCursorPos(window, lastX, lastY);

                previousTime = glfwGetTime();
                return window;
            }

            void InitializeGL(GLFWwindow *window)
            {

                SGL_PROFILE_FUNCTION();
                GLenum glewInitResult;


                glewExperimental = GL_TRUE;
                glewInitResult = glewInit();
                if (glewInitResult != GLEW_OK) {
                    fprintf(stdout, "GLEW ERROR: %s\n", glewGetErrorString(glewInitResult));
                    exit(EXIT_FAILURE);
                }

                GLCall(glViewport(0, 0, CurrentWidth, CurrentHeight));


                GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
                GLCall(glEnable(GL_DEPTH_TEST));
                GLCall(glDepthFunc(GL_LESS));

                GLCall(glEnable(GL_STENCIL_TEST));

                GLCall(glEnable(GL_CULL_FACE));

                seagullUi.init(window);
                fix_render_on_mac(window);
            }


    };

}
#endif
