#include "StdAfx.h"
#include "App.h"

//Provides for the initiation and procedures of two windows, a main sketching window
//and a color bar window filled with buttons that change the pen's color

App::App(void) : m_pRenderTarget(NULL) , color(D2D1::ColorF::Black)
{
}


App::~App(void)
{
}

bool App::Init(HINSTANCE instance, int cmd)
{
	hInstance = instance;  //Initializes global variable hInstance

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOREMOREDRAWINGSTUFF, szWindowClass, MAX_LOADSTRING);

	RegisterClass();

	// Perform application initialization:
	if (!InitInstance (cmd))
	{
		return false;
	}

	return true;
}

int App::RunMessageLoop()
{
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOREMOREDRAWINGSTUFF));


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM App::RegisterClass()
{
	//registers main window class
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOREMOREDRAWINGSTUFF));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOREMOREDRAWINGSTUFF);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	//registers color bar class
	WNDCLASSEX c_wcex;

	c_wcex.cbSize = sizeof(WNDCLASSEX);

	c_wcex.style			= CS_HREDRAW | CS_VREDRAW;
	c_wcex.lpfnWndProc		= C_WndProc;
	c_wcex.cbClsExtra		= 0;
	c_wcex.cbWndExtra		= 0;
	c_wcex.hInstance		= hInstance;
	c_wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOREMOREDRAWINGSTUFF));
	c_wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	c_wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	c_wcex.lpszMenuName		= NULL;
	c_wcex.lpszClassName	= L"Color Bar";
	c_wcex.hIconSm			= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&c_wcex);
}

BOOL App::InitInstance(int nCmdShow)
{
   HRESULT hr = CreateDeviceIndependentResources();

   if FAILED(hr)
   {
	   return FALSE;
   }

   HWND hWnd;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      600, 300, 600, 600, NULL, NULL, hInstance, this);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//wndproc for main window
LRESULT CALLBACK App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	App* pApp;

	if (message == WM_CREATE)
	{
		//initiates pApp to its correct instance
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        pApp = (App *)pcs->lpCreateParams;
        ::SetWindowLongPtrW(hWnd,GWLP_USERDATA,PtrToUlong(pApp));

		//creates the Color Bar window
		HWND child_hwnd = CreateWindow(L"Color Bar", L"Colors", WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
									400, 400, 100, 400, hWnd, NULL, (HINSTANCE)GetModuleHandle(NULL), pApp);
		pApp->LoadButtons(child_hwnd); 
        return TRUE;
	}
	else
	{
        pApp = reinterpret_cast<App *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
		if (!pApp)
           return DefWindowProc(hWnd, message, wParam, lParam);
    }

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(pApp->getInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, pApp->About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		pApp->OnRender(hWnd);
		EndPaint(hWnd, &ps);
		break;

	case WM_DISPLAYCHANGE:
        pApp->OnRender(hWnd);
	    break;

	case WM_LBUTTONDOWN:
    {
		pApp->onLeftButtonDown(wParam, lParam, hWnd);
		float x = LOWORD(lParam);
		float y = HIWORD(lParam);
		pApp->paintRectangle(x, y);
    }
	break;

	case WM_LBUTTONUP:
	{	
		pApp->m_pRenderTarget->EndDraw();
		pApp->DiscardDeviceResources();
	}
	break;

	case WM_MOUSEMOVE:

		if (wParam & MK_LBUTTON)
		{
			pApp->OnMouseDown(wParam, lParam, hWnd);
		}
		break;

	case WM_CHAR:
    {
        char c = (TCHAR) wParam; 
        if (c == 'c')
        {
            pApp->ClearScreen(hWnd);
        }
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

//wndproc for the side color bar
LRESULT CALLBACK App::C_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	App* pApp;

	if (message == WM_CREATE)
	{
		//same as main wndproc, gives pApp its correct and current instance
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        pApp = (App *)pcs->lpCreateParams;
        ::SetWindowLongPtrW(hWnd,GWLP_USERDATA,PtrToUlong(pApp));
        return TRUE;
	}
	else
	{
        pApp = reinterpret_cast<App *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd,GWLP_USERDATA)));
		if (!pApp)
           return DefWindowProc(hWnd, message, wParam, lParam);
    }

	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
			case IDB_AQUA:
				pApp->SetColor(D2D1::ColorF::Aqua);
				break;
			case IDB_BLUE:
				pApp->SetColor(D2D1::ColorF::Blue);
				break;
			case IDB_BROWN:
				pApp->SetColor(D2D1::ColorF::Brown);
				break;
			case IDB_BLACK:
				pApp->SetColor(D2D1::ColorF::Black);
				break;
			case IDB_GREEN:
				pApp->SetColor(D2D1::ColorF::Green);
				break;
			case IDB_ORANGE:
				pApp->SetColor(D2D1::ColorF::Orange);
				break;
			case IDB_PEACH:
				pApp->SetColor(D2D1::ColorF::PeachPuff);
				break;
			case IDB_PINK:
				pApp->SetColor(D2D1::ColorF::Pink);
				break;
			case IDB_PURPLE:
				pApp->SetColor(D2D1::ColorF::Purple);
				break;
			case IDB_RED:
				pApp->SetColor(D2D1::ColorF::Red);
				break;
			case IDB_YELLOW:
				pApp->SetColor(D2D1::ColorF::Yellow);
				break;
			case IDB_WHITE:
				pApp->SetColor(D2D1::ColorF::White);
				break;
			case IDB_GREY:
				pApp->SetColor(D2D1::ColorF::Gray);
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
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





HRESULT App::CreateDeviceIndependentResources()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	return hr;
}

HRESULT App::CreateDeviceResources(HWND hwnd)
{
    HRESULT hr = S_OK;
    if (!m_pRenderTarget)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
        // Create a Direct2D render target. 
		hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &m_pRenderTarget
            );

        if (SUCCEEDED(hr))
        {
			//SetColor(D2D1::ColorF::Aqua);
            // Create a brush of the specified color
            hr = m_pRenderTarget->CreateSolidColorBrush(
                color,//D2D1::ColorF(D2D1::ColorF::Aqua),
                &m_pBrush
                );
        }
	}
    return hr;
}

void App::DiscardDeviceResources()
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pBrush);
}


//creates a render target that will stay active until
//the left mouse button is released
void App::onLeftButtonDown(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
	float x = LOWORD(lParam); 
    float y = HIWORD(lParam);
	tempPoints.first = x;
	tempPoints.second = y;
	//creates device resources that will be reused until left
	//mouse button is released again
	HRESULT hr;
	hr = CreateDeviceResources(hwnd);
	m_pRenderTarget->BeginDraw();
}

//adds a rectangle to the specified point, defined as inline
//to account for the fact that it is called a large number of times
//for the painting logic
inline void App::paintRectangle(float x, float y)
{
	//m_pRenderTarget->BeginDraw();
	D2D1_RECT_F rectangle = D2D1::RectF(x - 1, y - 1, x + 1, y + 1);
	m_pRenderTarget->DrawRectangle(rectangle, m_pBrush, 5.0f, NULL);
	//m_pRenderTarget->EndDraw();
}


//use of the firstRender variable stops the screen from clearing when we
//don't want it to
static bool firstRender = true;
HRESULT App::OnRender(HWND hwnd)
{
    HRESULT hr;
    hr = CreateDeviceResources(hwnd);

	if (SUCCEEDED(hr)) 
	{
		if ( !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
		{
			m_pRenderTarget->BeginDraw();
			if (firstRender)
			{
				m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
				firstRender = false;
			}
			hr = m_pRenderTarget->EndDraw();
		}
	}
	DiscardDeviceResources();
    return hr;
}

void App::OnMouseDown(WPARAM wParam, LPARAM lParam, HWND hwnd)
{
	float x = LOWORD(lParam);
	float y = HIWORD(lParam);
	DrawOnMovement(tempPoints, x, y);
}

//because windows will only capture so many coordinates on mouse movement, we need
//this algorithm to fill in the gaps created (there is definitely a better algorithm to use
//this one will create jagged edges on very fast movement)
void App::DrawOnMovement(std::pair<float, float> oldCoords, float targetX, float targetY)
{
	float tempX = oldCoords.first;
	float tempY = oldCoords.second;
	paintRectangle(tempX, tempY);

	while (fabs(targetX - tempX) > 2 || fabs(targetY - tempY) > 2)
	{
		if (fabs(targetX - tempX) > fabs( targetY - tempY))
		{
			//shifts tempX in the correct direction (towards targetX)
			if (tempX > targetX)
			{
				tempX -= 2;
		}
		else
			{
				tempX += 2;
			}
		}
		else
		{
			if (tempY > targetY)
			{
				tempY -= 2;
			}
			else 
			{
				tempY += 2;
			}
		}
		paintRectangle(tempX, tempY);
	}
	
	tempPoints.first = targetX;
	tempPoints.second = targetY;
}


void App::ClearScreen(HWND hwnd)
{
	HRESULT hr;
    hr = CreateDeviceResources(hwnd);
	if (SUCCEEDED(hr)) 
	{
		if ( !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
			m_pRenderTarget->EndDraw();
		}
	}
	DiscardDeviceResources();
}




INT_PTR CALLBACK App::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

void App::LoadButtons(HWND hwnd)
{
	//sorry for this monstrosity :3
	//creates the buttons used by the sidebar, and then loads images corresponding to their colors
	HWND b_hwnd;
	HANDLE b_icon;

	b_hwnd = CreateWindow(L"button", L"BLACK", WS_VISIBLE | WS_CHILD | BS_ICON, 5, 5, 35, 25,        
						 hwnd, (HMENU) IDB_BLACK, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BLACK), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"WHITE", WS_VISIBLE | WS_CHILD | BS_ICON, 50, 5, 35, 25,        
						 hwnd, (HMENU) IDB_WHITE, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WHITE), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"BLUE", WS_VISIBLE | WS_CHILD | BS_ICON, 50, 40, 35, 25,        
						 hwnd, (HMENU) IDB_BLUE, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BLUE), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"AQUA", WS_VISIBLE | WS_CHILD | BS_ICON, 5, 40, 35, 25,        
						 hwnd, (HMENU) IDB_AQUA, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_AQUA), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"PURPLE", WS_VISIBLE | WS_CHILD | BS_ICON, 5, 75, 35, 25, 
						  hwnd, (HMENU) IDB_PURPLE, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PURPLE), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"GREEN", WS_VISIBLE | WS_CHILD | BS_ICON, 50, 75, 35, 25,        
						  hwnd, (HMENU) IDB_GREEN, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_GREEN), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"YELLOW", WS_VISIBLE | WS_CHILD | BS_ICON, 5, 110, 35, 25,        
						  hwnd, (HMENU) IDB_YELLOW, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_YELLOW), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"PEACH", WS_VISIBLE | WS_CHILD | BS_ICON, 50, 110, 35, 25,        
						 hwnd, (HMENU) IDB_PEACH, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PEACH), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"ORANGE", WS_VISIBLE | WS_CHILD | BS_ICON, 5, 145, 35, 25,        
						 hwnd, (HMENU) IDB_ORANGE, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ORANGE), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"RED", WS_VISIBLE | WS_CHILD | BS_ICON, 50, 145, 35, 25,        
						 hwnd, (HMENU) IDB_RED, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_RED), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"BROWN", WS_VISIBLE | WS_CHILD | BS_ICON, 5, 180, 35, 25,        
						 hwnd, (HMENU) IDB_BROWN, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BROWN), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);


	b_hwnd = CreateWindow(L"button", L"GREY", WS_VISIBLE | WS_CHILD | BS_ICON, 50, 180, 35, 25,        
						 hwnd, (HMENU) IDB_GREY, NULL, NULL);
	b_icon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_GREY), IMAGE_ICON,
							100, 100, NULL);
	SendMessage(b_hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)b_icon);
}