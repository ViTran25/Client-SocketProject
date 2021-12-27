
// ClientDlg.h : header file
//

#pragma once
#define PORT 25000
#define WM_SOCKET WM_USER+2

// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	char* ConvertToChar(const CString& s);
	void Split(CString src, CString des[2]);
	void mSend(CString Command);
	int mRecv(CString& Command);
	void getDateTime(CString& dateTime);

	SOCKET sClient;
	sockaddr_in servAdd;
	CString Command;
	int buffLength;
	CString strResult[2];
	CString userOnline;
	int level;
	int isLogon;
	CString dateTime;

	CString m_msgString;
	CString IP;
	CString m_userName;
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	CString m_passWord;
	afx_msg void OnBnClickedSignIn();
	COleDateTime m_dateTime;
	afx_msg void OnBnClickedWorld1();
	CString m_countryName;
	CString m_provinceName;
	afx_msg void OnBnClickedWorld2();
	afx_msg void OnBnClickedVn1();
	afx_msg void OnBnClickedVn2();
};
