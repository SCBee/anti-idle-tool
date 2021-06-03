#include <windows.h>
#include <iostream>

// Sends the keystrokes, named for producing 'effort' by never going afk/away on popular team-based applications
void produce(const unsigned int& keyCode)
{
    // Sends keystrokes to windows device input buffer
    // Can use a lot of cpu if delay is too low; it would be better to expand the number of keystrokes
    // per send rather than doing multiple calls with fewer keystrokes.

    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = keyCode;
    
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keyCode;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    // Send to buffer & log (Used only for debugging purposes)
    unsigned int sent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (sent != ARRAYSIZE(inputs)) { std::cout << "Bad\n"; }
    else { std::cout << "Good\n"; }
}

// Threadable obj
struct State
{
    bool exit;
    bool idle;
    int delay;

    State()
    {
        exit = false;
        idle = false;
        delay = 6000;
    }

    void changeDelay()
    {
        // No error checking (basic functionality)
        std::cout << "Enter the delay in ms: ";
        std::cin >> delay;
    }

    void toggle()
    {
        idle = !idle;

        if (idle)
        {
            std::cout << "Now idling...\n";
        }
        else
        {
            std::cout << "Standby...\n";
        }
    }

    void EXIT()
    {
        exit = true;
    }

}state;

// Idle driver
void driver()
{
    while (!state.exit)
    {
        if (state.idle)
        {
            produce(VK_NUMLOCK);
            Sleep(state.delay);
        }
    }
}

// Handles toggling logic
void helper()
{
    while (!state.exit)
    {
        if (GetAsyncKeyState(VK_F1) & 0x8000)
        {
            state.toggle();
        }
        if (GetAsyncKeyState(VK_F2) & 0x8000)
        {
            std::cout << "Exiting Application\n";
            state.EXIT();
        }
        if (GetAsyncKeyState(VK_F3) & 0x8000)
        {
            state.changeDelay();
        }

        Sleep(100);
    }
}

#include <thread>

int main()
{
    // Creating function threads
    std::thread thread1(driver);
    std::thread thread2(helper);

    // Waits until completion then exits
    thread1.join();
    thread2.join();

    return 0;
}
