#include "pch.h"  // Replaces the original #include "stdafx.h"
#include "HDCPGenerator.h"
#include "HDCPGeneratorDlg.h"

BEGIN_MESSAGE_MAP(CHDCPGeneratorApp, CWinAppEx)
END_MESSAGE_MAP()

CHDCPGeneratorApp theApp;

BOOL CHDCPGeneratorApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    AfxEnableControlContainer();

    // Create the dialog
    CHDCPGeneratorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}