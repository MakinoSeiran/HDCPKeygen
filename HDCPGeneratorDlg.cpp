#include "pch.h"
#include "HDCPGenerator.h"
#include "HDCPGeneratorDlg.h"
#include "HDCPCore.h"      
#include "afxdialogex.h"
#include <sstream>
#include <iomanip>
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHDCPGeneratorDlg::CHDCPGeneratorDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_HDCPGENERATOR_DIALOG, pParent)
    , m_currentKSV(0)
    , m_strKSV(_T(""))
    , m_strKey(_T(""))
{
}

void CHDCPGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RADIO_SOURCE, m_radioSource);
    DDX_Control(pDX, IDC_RADIO_SINK, m_radioSink);
    DDX_Control(pDX, IDC_EDIT_KSV, m_editKSV);
    DDX_Control(pDX, IDC_EDIT_KEY, m_editKey);
    DDX_Text(pDX, IDC_EDIT_KSV, m_strKSV);
    DDX_Text(pDX, IDC_EDIT_KEY, m_strKey);
}

BEGIN_MESSAGE_MAP(CHDCPGeneratorDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CHDCPGeneratorDlg::OnGenerate)
    ON_BN_CLICKED(IDC_BUTTON_COPY_KSV, &CHDCPGeneratorDlg::OnCopyKSV)
    ON_BN_CLICKED(IDC_BUTTON_COPY_KEY, &CHDCPGeneratorDlg::OnCopyKey)
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CHDCPGeneratorDlg::OnTest)
END_MESSAGE_MAP()

BOOL CHDCPGeneratorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set default radio button to Source
    m_radioSource.SetCheck(TRUE);
    m_radioSink.SetCheck(FALSE);

    // Load master key file (assuming the file is in the same directory as the exe)
    std::wstring path = L".\\master-key.txt";
    if (!m_core.LoadMasterKey(path))
    {
        CString msg;
        msg.Format(L"Failed to load master key file: %s\nPlease place master-key.txt in the program directory.",
            m_core.GetLastError().c_str());
        AfxMessageBox(msg);
    }

    // Play background music (if implemented)
    // PlayBackgroundMusic();

    // Generate an initial set of keys
    OnGenerate();

    return TRUE;
}

void CHDCPGeneratorDlg::OnGenerate()
{
    // Determine the currently selected mode (Source or Sink)
    BOOL bIsSink = (m_radioSink.GetCheck() == BST_CHECKED);

    // Generate KSV
    m_currentKSV = m_core.GenerateKSV();

    // Generate Key
    if (bIsSink)
        m_currentKey = m_core.GenerateSinkKey(m_currentKSV);
    else
        m_currentKey = m_core.GenerateSourceKey(m_currentKSV);

    // Format output
    std::string ksvStr = HDCPCore::FormatKSV(m_currentKSV);
    std::string keyStr = HDCPCore::FormatKey(m_currentKey, bIsSink ? true : false);

    // Convert to CString and assign to member variables
    m_strKSV = ksvStr.c_str();
    m_strKey = keyStr.c_str();

    // Update UI display
    UpdateData(FALSE);
}

void CHDCPGeneratorDlg::OnCopyKSV()
{
    std::string ksvStr = HDCPCore::FormatKSV(m_currentKSV);
    CString text(ksvStr.c_str());
    CopyToClipboard(text);
}

void CHDCPGeneratorDlg::OnCopyKey()
{
    std::ostringstream oss;
    for (int i = 0; i < 40; i += 5)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (i + j < 40)
                oss << std::hex << std::setw(14) << std::setfill('0') << m_currentKey[i + j] << " ";
        }
        oss << "\n";
    }
    std::string keyStr = oss.str();
    CString text(keyStr.c_str());
    CopyToClipboard(text);
}

void CHDCPGeneratorDlg::OnTest()
{
    std::string result;
    bool ok = m_core.SelfTest(result);
    CString msg(result.c_str());
    AfxMessageBox(msg);
}

void CHDCPGeneratorDlg::CopyToClipboard(const CString& text)
{
    if (!OpenClipboard())
        return;

    EmptyClipboard();

    // 获取字符串长度（字符数）
    int len = text.GetLength();
    // 计算需要的字节数，包括终止符
    size_t bytes = (len + 1) * sizeof(TCHAR);

    // 分配全局内存
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (hGlob)
    {
        // 锁定内存并复制字符串
        LPTSTR pStr = (LPTSTR)GlobalLock(hGlob);
        if (pStr)
        {
            _tcscpy_s(pStr, len + 1, text); // 安全复制
            GlobalUnlock(hGlob);

            // 根据项目字符集设置剪贴板格式
#ifdef _UNICODE
            SetClipboardData(CF_UNICODETEXT, hGlob);
#else
            SetClipboardData(CF_TEXT, hGlob);
#endif
        }
        else
        {
            GlobalFree(hGlob);
        }
    }

    CloseClipboard();
}