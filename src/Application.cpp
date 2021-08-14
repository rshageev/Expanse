#include "Application.h"

#include "backends/imgui_impl_sdl.h"

#include "Render/SpriteBatch.h"
#include "Utils/Random.h"

#include "Game/Terrain/Systems/GenerateTerrain.h"
#include "Game/Terrain/Systems/RenderTerrain.h"

#include "Game/Player/ScrollCameraSystem.h"

namespace Expanse
{
    Application::Application()
    {
        systems = std::make_unique<Game::SystemCollection>(world);
    }

    void Application::Init(Point window_size, Point framebuffer_size)
    {
        // Create renderer
        renderer = Render::CreateOpenGLRenderer(window_size, framebuffer_size);
        renderer->SetBgColor({0.0f, 0.3f, 0.2f, 1.0f});

        // Camera control
        systems->AddSystem<Game::Player::ScrollCamera>();

        // Init systems
        auto terrain_systems = systems->AddSystem<Game::SystemCollection>();
        terrain_systems->AddSystem<Game::Terrain::GenerateCells>(GetRandomSeed());
        terrain_systems->AddSystem<Game::Terrain::RenderCells>(renderer.get());
 
        // init ImGui backend
        imgui_render = std::make_unique<ImGuiRenderer>(renderer.get());
    }

    void Application::Tick()
    {
        world.dt = timer.Elapsed(true);

        renderer->ClearFrame();

        const auto window_rect = FRect{ renderer->GetWindowRect() };
        const auto view_rect = Centralized(window_rect) / world.camera_scale + world.camera_pos;
        renderer->Set2DMode(view_rect);

        systems->Update();

        //ImGuiFrame(world.dt);

        Input::UpdateState(world.input);
    }

    void Application::ProcessSystemEvent(const SDL_Event& evt)
    {
        ProcessInputEvent(evt);

        ImGui_ImplSDL2_ProcessEvent(&evt);
    }

    void Application::ImGuiFrame(float dt)
    {
        ImGui_ImplSDL2_NewFrame(renderer->GetWindowSize(), renderer->GetFramebufferSize(), dt);

        imgui_render->StartFrame();

        ImGui::ShowDemoWindow();

        imgui_render->EndFrame();
    }

    void Application::ProcessInputEvent(const SDL_Event& event)
    {
        using namespace Input;

        auto& input = world.input;

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode < input.keyboard_state.size()) {
                input.keyboard_state[event.key.keysym.scancode] = ButtonState::Pressed;
                input.key_state_changed = true;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode < input.keyboard_state.size()) {
                input.keyboard_state[event.key.keysym.scancode] = ButtonState::Released;
                input.key_state_changed = true;
            }
        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            input.mouse_wheel = event.wheel.y;
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            input.mouse_pos = Point{ event.motion.x, event.motion.y };
            input.mouse_pos_rel += Point{ event.motion.xrel, -event.motion.yrel };
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            input.mouse_state[event.button.button] = ButtonState::Pressed;
            input.mouse_key_state_changed = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            input.mouse_state[event.button.button] = ButtonState::Released;
            input.mouse_key_state_changed = true;
        }
    }
}