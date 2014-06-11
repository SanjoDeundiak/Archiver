#pragma once

#include <windows.h>
#include <shlobj.h>     // For IShellExtInit and IContextMenu
#include <vector>

class FileContextMenuExt : public IShellExtInit, public IContextMenu
{
public:
    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellExtInit
    IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

    // IContextMenu
    IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
    IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	
    FileContextMenuExt(void);

protected:
    ~FileContextMenuExt(void);

private:
    // Reference count of component.
    long m_cRef;

    // The name of the selected file.
    wchar_t m_szSelectedFile[MAX_PATH];
	wchar_t m_execPath[MAX_PATH];
	std::vector<std::wstring> m_szSelectedFiles;

    // The method that handles the "display" verb.
    void OnVerbDisplayFileName(HWND hWnd);

    PWSTR m_pszMenuText;
    HANDLE m_hMenuBmp;
    PCSTR m_pszVerb;
    PCWSTR m_pwszVerb;
    PCSTR m_pszVerbCanonicalName;
    PCWSTR m_pwszVerbCanonicalName;
    PCSTR m_pszVerbHelpText;
    PCWSTR m_pwszVerbHelpText;
};