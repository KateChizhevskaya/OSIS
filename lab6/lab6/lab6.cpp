// lab6.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab6.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
int x;
int y;
int move_timer;
bool paused = false;// стоит на паузе
int startPosition; //сохранение начального положение текста
int windowLength; // ширина окна
PAINTSTRUCT *ps;
HDC hdc;
LPCWSTR text = L"Hello";
LPCWSTR font = L"Times New Roman";
LPCWSTR spases = L"     ";
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
LPCWSTR iconName  = L"IDI_ICON1";
LPCWSTR cursorName = L"IDC_CURSOR2";
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInstance, iconName);

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

void drow(HDC hdc)
{
	int length = strlen("Hello") + 3;
	TextOut(hdc, x, y, spases, length); //стираем старый текст кол-во пробелов равно кол-ву букв в слове 
	x = x + 10; //устанавливаем новые координаты 
	TextOut(hdc, x, y, text, length); //выводим текст
}

void getStartPosition(HWND hWnd)
{
	RECT window; // получаем размеры окна
	GetWindowRect(hWnd, &window);
	x = window.left;
	windowLength = window.right;
	startPosition = x;
	y = (window.bottom - window.top) / 2;
}

void endDrow(HWND hWnd, PAINTSTRUCT *ps)
{
	EndPaint(hWnd, ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

		case WM_CREATE:
		{
			WM_PAINT;
			getStartPosition(hWnd);
			move_timer = SetTimer(hWnd, 1, 100, NULL);
		}

		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				// Разобрать выбор в меню:
				switch (wmId)
				{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				case ID_CONTROL_START:
				{
					if (paused)
					{
						paused = false;
						move_timer = SetTimer(hWnd, 1, 100, NULL);
					}
					break;
				}
				case ID_CONTROL_STOP:
				{
					if (!paused)
					{
						paused = true;
						KillTimer(hWnd, move_timer);
					}
					break;
				}
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			break;

		case WM_TIMER:
		{
			if (x > windowLength - 250)
			{
				int length = strlen("Hello") + 3;
				TextOut(hdc, x, y, spases, length);
				x = startPosition;
			}
			drow(hdc);
			return 0;
		}

		case WM_PAINT:
		{
			ps = new PAINTSTRUCT;
			hdc = BeginPaint(hWnd, ps);			
			return 0;
		}
		break;
		case WM_DESTROY:
		{
			endDrow(hWnd, ps);
			PostQuitMessage(0);
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
}


// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
