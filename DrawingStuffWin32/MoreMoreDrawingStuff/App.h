#pragma once

#define MAX_LOADSTRING 100

#include "Resource.h"
#include <d2d1.h>
#include <vector>
#include <math.h>
#include <string>
#include <map>


template<class Interface>
inline void
SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}


class App
{
public:
	App(void);
	~App(void);
	bool Init(HINSTANCE instance, int cmd);
	int RunMessageLoop();
	ATOM RegisterClass();
	BOOL InitInstance(int);
	//Main window procedure
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	//Color bar window procedure
	static LRESULT CALLBACK C_WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
	HINSTANCE getInstance() {return hInstance;}



private:
	HINSTANCE hInstance;					//current instance
	TCHAR szTitle[MAX_LOADSTRING];			//title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];	//main window class name

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(HWND hwnd);
	void DiscardDeviceResources();
	HRESULT OnRender(HWND hwnd);
	void OnMouseDown(WPARAM wParam, LPARAM lParam, HWND hwnd);
	void ClearScreen(HWND hwnd);
	void onLeftButtonDown(WPARAM wParam, LPARAM lParam, HWND hwnd);
	void paintRectangle(float x, float y);
	void DrawOnMovement(std::pair<float, float> oldCoords, float targetX, float targetY);
	void LoadButtons(HWND hwnd);
	void SetColor(D2D1::ColorF newColor) {color = newColor;}

	ID2D1Factory* m_pD2DFactory;
    ID2D1EllipseGeometry* m_pEllipseGeometry;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBrush;
	std::pair<float, float> tempPoints;
	D2D1::ColorF color;
	
};

