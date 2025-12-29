#include <windows.h>
#include <stdio.h>

// Buton ve Metin kutusu kimlikleri
#define ID_BUTTON 1
#define ID_EDIT 2

HWND hEdit;

// USB dosyasını oluşturan fonksiyon
void CreateAutorun(char driveLetter) {
    char path[20];
    sprintf(path, "%c:\\autorun.inf", driveLetter);
    
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "[Autorun]\nIcon=simge.ico\nLabel=Ozel USB\n");
        fclose(f);
        MessageBox(NULL, "Autorun.inf basariyla olusturuldu!", "Basarili", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(NULL, "Hata: Dosya olusturulamadi. Yonetici izinlerini kontrol edin.", "Hata", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            CreateWindow("STATIC", "USB Harfi (E, F, G):", WS_VISIBLE | WS_CHILD, 20, 20, 150, 20, hwnd, NULL, NULL, NULL);
            hEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 170, 20, 30, 20, hwnd, (HMENU)ID_EDIT, NULL, NULL);
            CreateWindow("BUTTON", "Ikonu Uygula", WS_VISIBLE | WS_CHILD, 50, 60, 120, 30, hwnd, (HMENU)ID_BUTTON, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON) {
                char buffer[2];
                GetWindowText(hEdit, buffer, 2);
                if (buffer[0] != '\0') {
                    CreateAutorun(buffer[0]);
                } else {
                    MessageBox(hwnd, "Lutfen bir harf girin!", "Uyari", MB_OK | MB_ICONWARNING);
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "USB_GUI";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "USB Icon Yapici", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 150, NULL, NULL, hInstance, NULL);
    
    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}