#include <ituGL/application/Application.h>

// For breaking execution in debug when an unexpected condition is found
#include <cassert>
// For accurate application time
#include <chrono>
// For error messages
#include <iostream>

// DeviceGL and main Window are constructed in the correct order because they were declared like that!
Application::Application(int width, int height, const char* title)
    : m_exitCode(0)
{
}

Application::~Application()
{
    // If something didn't go as expected, display an error message
    if (m_exitCode)
    {
        std::cout << "Error (" << m_exitCode << "): " << m_errorMessage << std::endl;
    }
}

int Application::Run()
{
    // If the application is not in error state, run
    if (!m_exitCode)
    {
        Initialize();

        // current time when the application started
        auto startTime = std::chrono::steady_clock::now();

        // Main loop
        while (IsRunning())
        {
            // set current time relative to start time
            std::chrono::duration<float> duration = std::chrono::steady_clock::now() - startTime;
            UpdateTime(duration.count());

            Update();

            Render();
        }

        Cleanup();
    }

    // return the exit code
    return m_exitCode;
}

void Application::Initialize()
{
}

void Application::Update()
{
}

void Application::Render()
{
}

void Application::Cleanup()
{
}

void Application::Terminate(int exitCode, const char* errorMessage)
{
    m_exitCode = exitCode;
    if (errorMessage)
    {
        // Ensure that error messages come with exit code different from 0
        assert(exitCode);
        m_errorMessage = errorMessage;
    }

    // If termination is requested and main window is still valid, request to close


    // Force assert here to detect error termination
    assert(!exitCode);
}

void Application::UpdateTime(float newCurrentTime)
{
    m_deltaTime = newCurrentTime - m_currentTime;
    m_currentTime = newCurrentTime;
}

bool Application::IsRunning() const
{
    // Run while the window is valid and it has not been requested to close
    return false;
}
