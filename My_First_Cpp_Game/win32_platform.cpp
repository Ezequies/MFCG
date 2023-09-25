#include <windows.h>

bool running = true;

struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmapinfo;
};

Render_State render_state;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	LRESULT result = 0;

	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		}break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(unsigned int);

			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
			render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
			render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
			render_state.bitmapinfo.bmiHeader.biPlanes = 1;
			render_state.bitmapinfo.bmiHeader.biBitCount = 32;
			render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;

		}break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}break;
	}

	return result;
}

int WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd)
{
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;

	RegisterClass(&window_class);

	HWND window = CreateWindow(
		window_class.lpszClassName,
		L"My First Game!",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		0,
		0,
		hInstance,
		0);

	HDC hdc = GetDC(window);

	while (running) 
	{
		MSG message;

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		unsigned int* pixel = (unsigned int*)render_state.memory;
		for (int y = 0; y < render_state.height; y++)
		{
			for (int x = 0; x < render_state.width; x++)
			{
				*pixel++ = x*y;
			}
		}

		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	}
} 