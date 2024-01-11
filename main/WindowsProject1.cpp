// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject1.h"
#include <Shellapi.h>

#define MAX_LOADSTRING 100
#define IDC_EDIT_INPUT 1001
#define IDC_STATIC_TEXT 1002
#define IDC_BUTTON_OK 1003


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    // 设置窗口标题
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    // 修改窗口标题
    wcscpy_s(szTitle, MAX_LOADSTRING, L"LOL欧服转中文");

    // 窗口宽度和高度不变
    int windowWidth = 400;
    int windowHeight = 400;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, 0, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}



//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//


void OpenProgramWithParameters(const wchar_t* programPath, const wchar_t* parameters) {
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpFile = programPath;
    sei.lpParameters = parameters;
    sei.nShow = SW_SHOWNORMAL;
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if (ShellExecuteEx(&sei)) {
        WaitForSingleObject(sei.hProcess, INFINITE);
        CloseHandle(sei.hProcess);
    }
    else {
        // 处理错误
        DWORD error = GetLastError();
        // 可以使用错误代码进行调试或显示适当的消息框
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_BUTTON_OK:
            // 处理按钮点击事件，获取输入框内容
        {
            WCHAR buffer[256]; // 用于存储输入框内容的缓冲区
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_INPUT), buffer, sizeof(buffer) / sizeof(buffer[0]));
            // 如果输入框内容为空，弹出提示框
            if (wcslen(buffer) == 0) {
				MessageBox(hWnd, L"请输入LOL安装目录！", L"提示", MB_OK | MB_ICONINFORMATION);
				break;
			}
            // buffer 后面添加 --locale=zh_CN
            // 直接将D:\Programa\Riot Games\League of Legends\LeagueClient.exe --locale=zh_CN 赋值给buffer

            // 打开程序
            HINSTANCE hInstance = NULL;  // 注意：此处不需要提前声明 HINSTANCE

            // 例如，打开 LeagueClient.exe 并传递参数 "--locale=zh_CN"
            OpenProgramWithParameters(buffer, L"--locale=zh_CN");

            // 处理 hInstance（如果需要）
            MessageBox(hWnd, buffer, L"输入框内容", MB_OK);
            // 打开程序
        }
        break;
        case 1005:
            // 填充默认文本到输入框
        {
			  	// 获取输入框句柄
			HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_INPUT);
			// 设置输入框默认文本
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)L"D:\\Programa\\Riot Games\\League of Legends\\LeagueClient.exe");
        }
        break;
        case 1006:
			// 填充默认文本到输入框
        {
            HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_INPUT);
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)L"D:\\Program Files\\Riot Games\\League of Legends\\LeagueClient.exe");
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
    {
        // 在主窗口创建时创建静态文本
        HWND hStaticText = CreateWindowEx(0, L"STATIC", L"请输入LOL安装目录:",
            WS_CHILD | WS_VISIBLE,
            10, 10, 180, 20, hWnd, (HMENU)IDC_STATIC_TEXT, GetModuleHandle(NULL), NULL);

        if (hStaticText == NULL)
        {
            MessageBox(hWnd, L"无法创建静态文本！", L"错误", MB_OK | MB_ICONERROR);
        }

        // 在主窗口创建时创建输入框
        HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            10, 30, 180, 20, hWnd, (HMENU)IDC_EDIT_INPUT, GetModuleHandle(NULL), NULL);

        if (hEdit == NULL)
        {
            MessageBox(hWnd, L"无法创建编辑框！", L"错误", MB_OK | MB_ICONERROR);
        }
        // 设置输入框最大长度
        SendMessage(hEdit, EM_SETLIMITTEXT, 256, 0);
        // 创建一个按钮来填充默认文本到输入框
        HWND hButtonFill = CreateWindowEx(0, L"BUTTON", L"默认LOL安装目录1",
            			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            			200, 30, 100, 20, hWnd, (HMENU)1005, GetModuleHandle(NULL), NULL);
        HWND hButtonFill2 = CreateWindowEx(0, L"BUTTON", L"默认LOL安装目录2",
            						WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            						200, 60, 100, 20, hWnd, (HMENU)1006, GetModuleHandle(NULL), NULL);

        SendMessage(hButtonFill, WM_COMMAND, 0, 0);
        // 设置输入框默认文本
        //SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)L"D:\\Programa\\Riot Games\\League of Legends\\LeagueClient.exe");

        // 在主窗口创建时创建按钮
        HWND hButton = CreateWindowEx(0, L"BUTTON", L"确认",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 60, 50, 20, hWnd, (HMENU)IDC_BUTTON_OK, GetModuleHandle(NULL), NULL);

        if (hButton == NULL)
        {
            MessageBox(hWnd, L"无法创建按钮！", L"错误", MB_OK | MB_ICONERROR);
        }

        // 获取屏幕宽度和高度
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // 计算窗口左上角的坐标使其居中
        int windowWidth = 400;
        int windowHeight = 150;
        int x = (screenWidth - windowWidth) / 2;
        int y = (screenHeight - windowHeight) / 2;

        // 设置窗口位置（不改变大小）
        SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)L"关于");

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
