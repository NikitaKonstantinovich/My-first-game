/*

wnd - window
proc - process
h - handler

UINT - unsigned integer
param - parameter
l - long
rect - rectangle >> пр€моугольник
alloc - allocate - размечать пам€ть

default - по умолчанию
use - использовать
destroy - разрушить
close - закрыть

	 top
left      right
	bottom
*/

#include <Windows.h>
#include <iostream>

#include "utils.cpp"

globalVariable bool running = true;

struct RenderState
{
	void* memory;
	int width;
	int height;
	BITMAPINFO BitMapInfo;
};

globalVariable RenderState renderState;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"


LRESULT CALLBACK windowClick(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;

		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			renderState.width = rect.right - rect.left;
			renderState.height = rect.bottom - rect.top;

			int bufferSize = renderState.width * renderState.height * sizeof(u32);

			if (renderState.memory)
				VirtualFree(renderState.memory, 0, MEM_RELEASE);
			renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			renderState.BitMapInfo.bmiHeader.biSize = sizeof(renderState.BitMapInfo.bmiHeader);
			renderState.BitMapInfo.bmiHeader.biWidth = renderState.width;
			renderState.BitMapInfo.bmiHeader.biHeight = renderState.height; 
			renderState.BitMapInfo.bmiHeader.biPlanes = 1;
			renderState.BitMapInfo.bmiHeader.biBitCount = 32;
			renderState.BitMapInfo.bmiHeader.biCompression = BI_RGB;
		} break;
		default: 
		{
			result = DefWindowProc(hWnd, uMsg, wParam, lParam);
		} break;
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// создать класс окна
	WNDCLASS WindowClass = {};
	WindowClass.style = CS_HREDRAW | CS_VREDRAW; // задаем когда будет переррисовыватьс€ окно
	WindowClass.lpszClassName = TEXT("Game Window Class"); // название класса окна
	WindowClass.lpfnWndProc = windowClick; // прив€зали к классу функцию обработчика кликов по окну

	// зарегать этот класс
	RegisterClass(&WindowClass);

	// создать окно

	HWND window = CreateWindow(
		WindowClass.lpszClassName, TEXT("my GaMe"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Input input = {};

	while (running)
	{
		// ввод
		MSG message;

		for (size_t i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed = false;
		}
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				u32 vkCode = (u32)message.wParam;
				bool isDown = ((message.lParam & (1 << 31)) == 0);

#define processButton(b, vk)\
case vk: {\
	input.buttons[b].isDown = true;\
	input.buttons[b].changed = true;\
} break;

				switch (vkCode)
				{
					processButton(BUTTON_UP, VK_UP);
					processButton(BUTTON_DOWN, VK_DOWN);
					processButton(BUTTON_LEFT, VK_LEFT);
					processButton(BUTTON_RIGHT, VK_RIGHT);
				}
			} break;
			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			}
			// симул€ци€ -- процесс игры
			simulateGame(&input);
			// перерисовка
			StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
}