#include "Seagull_Ui.h"

float UI::lampLocation[3] = {2.13f, 0.0f, 0.0f};
float UI::lampIntensity = 0.0f;

void UI::init(GLFWwindow *window)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    ImGui::StyleColorsDark();
    // ImGui::SetWindowFontScale(2.0f);
    ImGuiIO &io = ImGui::GetIO();
    // io.Fonts->AddFontFromFileTTF(FileSystem::getPath("seagull/resources/FiraCodeRegularNerdFontMono.ttf").c_str(), 14.0f);
    io.Fonts->AddFontFromFileTTF(FileSystem::getPath("seagull/resources/Helvetica.ttf").c_str(), 14.0f);
}

void UI::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::RenderUi()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::ShowMainMenuBar()
{

    static bool live_stats = false;
    static bool lamp_config = false;
    static bool scene_heiarchy = false;
    static bool exit_app = false;

    if (live_stats)         ShowLiveStats(&live_stats);
    if (lamp_config)        ShowLampConfiguration(&lamp_config);
    if (scene_heiarchy)     ShowSceneHeiarchy(&scene_heiarchy);
    if (exit_app)           ExitApp();

    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Exit", NULL, &exit_app)) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug Tools"))
        {
            if (ImGui::MenuItem("Debug Stats", NULL, &live_stats)) {}
            if (ImGui::MenuItem("Lamp Config", NULL, &lamp_config)) {}
            if (ImGui::MenuItem("Scene Heiarchy", NULL, &scene_heiarchy)) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void UI::ShowLiveStats(bool *p_open)
{
    if(!ImGui::Begin("Debug Stats", p_open))
    {
        ImGui::End();
        return;
    }
    
    ImGui::Text("Debug Stats:");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("OpenGL Version:     %s\n"
                "GLFW Version:       %s\n"
                "Dear ImGui Version: %s\n"
                "Assimp:             %u.%u.%u\n", 
                glGetString(GL_VERSION), glfwGetVersionString(), ImGui::GetVersion(),
                aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionPatch());
    ImGui::End();
}

void UI::ShowLampConfiguration(bool *p_open)
{
    if(!ImGui::Begin("Lamp Config", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::SliderFloat3("Lamp Location", lampLocation, -10.0f, 10.0f);
    ImGui::DragFloat("Lamp Intensity", &lampIntensity, 0.005f, 0.0f, 1.0f);

    ImGui::End();
}

void UI::ShowSceneHeiarchy(bool *p_open)
{
    static float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    // static float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    if (!ImGui::Begin("Scene Heiarchy", p_open))
    {
        ImGui::End();
        return;
    }

    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("", 3, flags))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        ImGui::TableHeadersRow();

        struct SceneNode
        {
            const char *Name;
            const char *Type;
            int Size;
            int ChildIdx;
            int ChildCount;
            static void DisplayNode(const SceneNode *node, const SceneNode *all_nodes)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                const bool is_parent = (node->ChildCount > 0);
                if (is_parent)
                {
                    bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
                    ImGui::TableNextColumn();
                    if (node->Size == -1)
                        ImGui::TextDisabled("Unknown");
                    else if (node->Size == -2)
                        ImGui::TextDisabled("--");
                    else
                        ImGui::TextDisabled("%d", node->Size);
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(node->Type);
                    if (open)
                    {
                        for (int child_n = 0; child_n < node->ChildCount; child_n++)
                        {
                            DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
                        }
                        ImGui::TreePop();
                    }
                }
                else
                {
                    ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                    ImGui::TableNextColumn();
                    if (node->Size == -1)
                        ImGui::Text("Unknown");
                    else if (node->Size == -2)
                        ImGui::TextDisabled("--");
                    else
                        ImGui::Text("%d", node->Size);
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(node->Type);
                }
            }
        };

        /*
         * Should be able to fill nodes with data from scene manager
         * which will store all model, texture, and other scene related
         * information.
         */

        static SceneNode nodes[1024];
        int oldId = 1; // Last used ID by a node
        int oldObjectChildrenSize = (int)ResourceManager::Models.size();
        int nodeIndex = 1;
        SceneNode rootNode = {"Root", "--", -2, oldId, oldObjectChildrenSize};
        nodes[0] = rootNode;

        // Cycle through loaded_models and add them to the top of the scene_heiarchy
        for (auto i = ResourceManager::Models.begin(); i != ResourceManager::Models.end(); i++)
        {
            int Id = oldId + oldObjectChildrenSize;
            int objectChildrenSize = 2; // Meshes and Textures are the only "children" of a model currently
            struct stat stat_buf;
            stat(i->second.path.c_str(), &stat_buf);
            SceneNode node = {i->first.c_str(), "Model", (int)stat_buf.st_size, Id, objectChildrenSize};
            nodes[nodeIndex] = node;
            oldId = Id;
            nodeIndex++;
        }

        // Add enough Mesh and Texture "parents/folder" for each model
        for (auto i = ResourceManager::Models.begin(); i != ResourceManager::Models.end(); i++)
        {
            int meshId = oldId + oldObjectChildrenSize;
            SceneNode nodeMesh = {"Meshes", "Mesh", -2, meshId, (int)i->second.meshes.size()};
            nodes[nodeIndex] = nodeMesh;
            nodeIndex++;

            int textureId = meshId + (int)i->second.meshes.size();
            SceneNode nodeTexture = {"Textures", "Texture", -2, textureId, (int)i->second.textures_loaded.size()};
            nodes[nodeIndex] = nodeTexture;
            nodeIndex++;

            oldId = textureId;
            oldObjectChildrenSize = (int)i->second.textures_loaded.size();

        }

        // Cycle through the models meshes and add them to the heiarchy.
        for (auto i = ResourceManager::Models.begin(); i != ResourceManager::Models.end(); i++)
        {
            for (auto j = i->second.meshes.begin(); j != i->second.meshes.end(); j++)
            {
                SceneNode node = {j->name.c_str(), "Mesh", -1, -1, -1};
                nodes[nodeIndex] = node;
                nodeIndex++;
            }
            for (auto x = i->second.textures_loaded.begin(); x != i->second.textures_loaded.end(); x++)
            {
                SceneNode node = {x->name.c_str(), "Texture", -1, -1, -1};
                nodes[nodeIndex] = node;
                nodeIndex++;
            }
        }

        SceneNode::DisplayNode(&nodes[0], nodes);

        ImGui::EndTable();
    }

    ImGui::End();
}

void UI::ExitApp()
{
    exit(0);
}










