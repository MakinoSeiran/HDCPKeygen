#pragma once
#include "HDCPCore.h"
#include "afxwin.h"

class CHDCPGeneratorDlg : public CDialogEx
{
public:
    CHDCPGeneratorDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_HDCPGENERATOR_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;

    // Message handlers
    afx_msg void OnGenerate();
    afx_msg void OnCopyKSV();
    afx_msg void OnCopyKey();
    afx_msg void OnTest();

    DECLARE_MESSAGE_MAP()

private:
    HDCPCore m_core;                 // Core algorithm object
    uint64_t m_currentKSV;            // Currently generated KSV
    std::vector<uint64_t> m_currentKey; // Currently generated Key

    CButton m_radioSource;
    CButton m_radioSink;
    CEdit m_editKSV;
    CEdit m_editKey;

    void CopyToClipboard(const CString& text);

public:
    // DDX variables
    CString m_strKSV;
    CString m_strKey;
};