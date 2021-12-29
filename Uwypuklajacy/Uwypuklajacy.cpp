// Języki Asemblerowe
// Filtrowanie obrazu - filtr uwypuklający
// Każda ze składowych obrazu RGB przekształcana jest przez maskę, tak aby wykryć krawędzie w obrazie.Efekt powinien być porównywalny do płaskorzeźby
// 11.01.2022 r.Rok akademicki : 3, semestr : 5, grupa : V
// Autor: Michał Foks
// wersja: 1.0
// 
// Uwypuklajacy.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "Uwypuklajacy.h"
#include "atlstr.h"
#include <windowsx.h>
#include <commdlg.h>
#include <string>
#include <thread>
#include <commctrl.h>
#include "Image.h"
#include <chrono>

#define MAX_LOADSTRING 100

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

//!!!
typedef HRESULT(__cdecl* LPFNDLLFUNC)(unsigned char*, unsigned char*, UINT, UINT, UINT, UINT); // DLL function handler
//przyciski
HWND buttonAddPicture;
HWND chboxASM;
HWND chboxCPP;
HWND libFrame;
HWND thrFrame;
HWND threadChoice;
HWND hText;
HWND buttonFiltr;
HWND progressBar;

#define ID_BUTTONADDPICTURE 501
#define ID_CHBOXASM 502
#define ID_CHBOXCPP 503
#define ID_BUTTONFILTER 504
#define ID_THREADCHOICE 505
#define ID_PROGRESSBAR 506

//sciezka do pliku
wchar_t fname[100];
bool noFile = true;
//obraz
Image image;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
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

    // TODO: W tym miejscu umieść kod

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UWYPUKLAJACY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UWYPUKLAJACY));

    MSG msg;

    // Główna pętla komunikatów:
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
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UWYPUKLAJACY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_UWYPUKLAJACY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej
   
   //progress bar
   /*INITCOMMONCONTROLSEX icc;
   icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
   icc.dwICC = ICC_BAR_CLASSES; 
   InitCommonControlsEx(&icc);
    */
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 1000, 300, nullptr, nullptr, hInstance, nullptr);

   buttonAddPicture = CreateWindowEx(0, L"BUTTON", L"Wybierz obraz", WS_CHILD | WS_VISIBLE,
       50, 30, 150, 30, hWnd, (HMENU) ID_BUTTONADDPICTURE, hInstance, NULL);

   libFrame = CreateWindowEx(0, L"BUTTON", L"Wybierz bibliotekę z której funkcji chcesz użyć", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
       300, 10, 600, 60, hWnd, NULL, hInstance, NULL);

   chboxASM = CreateWindowEx(0, L"BUTTON", L"Biblioteka assemblerowa", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       400, 30, 200, 30, hWnd, (HMENU)ID_CHBOXASM, hInstance, NULL);
   CheckDlgButton(hWnd, ID_CHBOXASM, BST_CHECKED); //ustawienie jednego przycisku domyslnie

   chboxCPP = CreateWindowEx(0, L"BUTTON", L"Biblioteka C++", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       700, 30, 120, 30, hWnd, (HMENU)ID_CHBOXCPP, hInstance, NULL);
   
   hText = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 70, 140, 20,
       hWnd, NULL, hInstance, NULL);
   SetWindowText(hText, L"Podaj liczbę wątków: ");

   threadChoice = CreateWindowEx(WS_EX_CLIENTEDGE, L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
       CBS_DROPDOWNLIST, 440, 70, 150, 200, hWnd, (HMENU)ID_THREADCHOICE, hInstance, NULL);
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"1");
   SendMessage(threadChoice, CB_SETCURSEL, 0, 0);
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"2");
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"4");
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"8");
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"16");
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"32");
   SendMessage(threadChoice, CB_ADDSTRING, 0, (LPARAM)L"64");
   //CheckDlgButton(hWnd, ID_THREADCHOICE, (LPARAM)L"2"));

   buttonFiltr = CreateWindowEx(0, L"BUTTON", L"Przepuść przez filtr uwypuklający", WS_CHILD | WS_VISIBLE,
       600, 70, 300, 30, hWnd, (HMENU)ID_BUTTONFILTER, hInstance, NULL);

   progressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
       600, 110, 300, 30, hWnd, (HMENU)ID_PROGRESSBAR, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wParam)
            {
            case ID_BUTTONADDPICTURE:
            {
                SendMessage(progressBar, PBM_SETPOS, (WPARAM)0, 0); //wyzeruj progressbar
                LPSTR filebuff = new char[256];
                OPENFILENAME open = { 0 };

                open.lStructSize = sizeof(OPENFILENAME);
                open.hwndOwner = hWnd; //Handle to the parent window
                open.lpstrFilter = L"Image Files(.jpg|.png|.bmp|.jpeg)\0*.jpg;*.png;*.bmp;*.jpeg\0\0";
                open.lpstrFile = fname;
                open.lpstrFile[0] = '\0';
                open.nMaxFile = MAX_PATH;
                open.lpstrTitle = L"Wybierz obraz\0";
                open.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
                
                if (GetOpenFileName(&open))
                {
                    //MessageBox(hWnd, open.lpstrFile, L"Info", MB_ICONINFORMATION);
                    noFile = false;
                    image.SetImage(fname);
                    image.Read();
                }
                else
                {
                    MessageBox(hWnd, open.lpstrFile, L"Nie wybrałeś pliku.", MB_ICONINFORMATION);
                }

                break;
            }
            case ID_BUTTONFILTER:

                if (noFile)
                {
                    MessageBox(hWnd, 0, L"Wybierz plik!", MB_ICONINFORMATION);
                    break;
                }

                SendMessage(progressBar, PBM_SETPOS, (WPARAM)0, 0);

                int id = ComboBox_GetCurSel(threadChoice); //indeks wybranego elementu z listy
                int numberOfThreads = 0;
                switch(id)
                {
                case 0:
                    numberOfThreads = 1;
                    break;
                case 1:
                    numberOfThreads = 2;
                    break;
                case 2:
                    numberOfThreads = 4;
                    break;
                case 3:
                    numberOfThreads = 8;
                    break;
                case 4:
                    numberOfThreads = 16;
                    break;
                case 5:
                    numberOfThreads = 32;
                    break;
                case 6:
                    numberOfThreads = 64;
                    break;
                }
                
                SendMessage(progressBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, numberOfThreads));
                int numberOfRows = image.GetHeight();
                int rowsForThread = numberOfRows / numberOfThreads;
                int rest = numberOfRows % numberOfThreads;
                double update = 100 / numberOfThreads;
                std::vector<std::thread> threads(numberOfThreads);
                int actualRow = 0;
                if (IsDlgButtonChecked(hWnd, ID_CHBOXCPP) == BST_CHECKED) //dll cpp
                {
                    HINSTANCE hDLL = LoadLibrary(L"Filtr"); // Load FiltrAsm.dll library dynamically
                    LPFNDLLFUNC lpfnDllFunc1; // Function pointer
                    if (NULL != hDLL)
                        lpfnDllFunc1 = (LPFNDLLFUNC)GetProcAddress(hDLL, "embossingFilter");
                    else
                    {
                        MessageBox(hWnd, 0, L"Problem z dll lub funkcją - CPP", MB_ICONINFORMATION);
                        break;
                    }
                    //measure time
                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numberOfThreads; i++)
                    {
                        if (rest != 0)
                        {
                            threads[i] = std::thread(lpfnDllFunc1, image.GetColorsPtr(), image.GetColorsFilterPtr(),
                                actualRow, actualRow + rowsForThread + 1, image.GetWidth(), image.GetHeight());
                            rest--;
                            actualRow += rowsForThread + 1;
                        }
                        else
                        {
                            threads[i] = std::thread(lpfnDllFunc1, image.GetColorsPtr(), image.GetColorsFilterPtr(),
                                actualRow, actualRow + rowsForThread, image.GetWidth(), image.GetHeight());
                            actualRow += rowsForThread;
                        }
                    }
                    for (int i = 0; i < numberOfThreads; i++)
                    {
                        SendMessage(progressBar, PBM_DELTAPOS, (WPARAM)1, 0);
                        threads[i].join();
                    }
                    //end measure time
                    auto stop = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                    int int_duration = duration.count();
                    std::string string_duration = std::to_string(int_duration);
                    MessageBoxA(hWnd, string_duration.c_str(), "Czas trwania [ms]: ", MB_OK);
                }
                if (IsDlgButtonChecked(hWnd, ID_CHBOXASM) == BST_CHECKED) //asm
                {
                    HINSTANCE hDLL2 = LoadLibrary(L"FiltrAsm"); // Load FiltrAsm.dll library dynamically
                    LPFNDLLFUNC lpfnDllFunc2; // Function pointer
                    if (NULL != hDLL2) 
                        lpfnDllFunc2 = (LPFNDLLFUNC)GetProcAddress(hDLL2, "embossingFilter");
                    else
                    {
                        MessageBox(hWnd, 0, L"Problem z dll lub funkcją - ASM", MB_ICONINFORMATION);
                        break;
                    }

                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < numberOfThreads; i++)
                    {
                        if (rest != 0)
                        {
                            threads[i] = std::thread(lpfnDllFunc2, image.GetColorsPtr(), image.GetColorsFilterPtr(),
                                actualRow, actualRow + rowsForThread + 1, image.GetWidth(), image.GetHeight());
                            rest--;
                            actualRow += rowsForThread + 1;
                        }
                        else
                        {
                            threads[i] = std::thread(lpfnDllFunc2, image.GetColorsPtr(), image.GetColorsFilterPtr(),
                                actualRow, actualRow + rowsForThread, image.GetWidth(), image.GetHeight());
                            actualRow += rowsForThread;
                        }
                    }
                    for (int i = 0; i < numberOfThreads; i++)
                    {
                        SendMessage(progressBar, PBM_DELTAPOS, (WPARAM)1, 0);
                        threads[i].join();
                    }
                    //end measure time
                    auto stop = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                    int int_duration = duration.count();
                    std::string string_duration = std::to_string(int_duration);
                    MessageBoxA(hWnd, string_duration.c_str(), "Czas trwania [ms]: ", MB_OK);
                }
                //save
                image.Save();

                break;
            }


            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
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

// Procedura obsługi komunikatów dla okna informacji o programie.
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
