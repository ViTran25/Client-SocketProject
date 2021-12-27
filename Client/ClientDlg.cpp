
// ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_msgString(_T(""))
	, IP(_T(""))
	, m_userName(_T(""))
	, m_passWord(_T(""))
	, m_dateTime(COleDateTime::GetCurrentTime())
	, m_countryName(_T(""))
	, m_provinceName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MSGBOX, m_msgString);
	DDX_Text(pDX, IDC_IP, IP);
	DDX_Text(pDX, IDC_USER, m_userName);
	DDX_Text(pDX, IDC_PASS, m_passWord);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_dateTime);
	DDX_Text(pDX, IDC_COUNTRY, m_countryName);
	DDX_Text(pDX, IDC_PROVINCE_VN, m_provinceName);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, &CClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGOUT, &CClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_SIGN_IN, &CClientDlg::OnBnClickedSignIn)
	ON_BN_CLICKED(IDC_WORLD1, &CClientDlg::OnBnClickedWorld1)
	ON_BN_CLICKED(IDC_WORLD2, &CClientDlg::OnBnClickedWorld2)
	ON_BN_CLICKED(IDC_VN1, &CClientDlg::OnBnClickedVn1)
	ON_BN_CLICKED(IDC_VN2, &CClientDlg::OnBnClickedVn2)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here

	IP = "127.0.0.1";
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_WORLD1)->EnableWindow(FALSE);
	GetDlgItem(IDC_WORLD2)->EnableWindow(FALSE);
	GetDlgItem(IDC_VN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_VN2)->EnableWindow(FALSE);
	UpdateData(FALSE);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::Split(CString src, CString des[2])
{
	int p1, p2;
	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);
	p2 = src.Find(_T("\r\n"), p1 + 1);
	des[1] = src.Mid(p1 + 2, p2 - (p1 + 2));
}

char* CClientDlg::ConvertToChar(const CString& s) {
	int nSize = s.GetLength();
	char* pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CClientDlg::mSend(CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CClientDlg::mRecv(CString& Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sClient, (char*)&buffLength, sizeof(int), 0);
	recv(sClient, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;
	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

void CClientDlg::getDateTime(CString& dateTime) 
{
	int m_day = m_dateTime.GetDay();
	int m_month = m_dateTime.GetMonth();
	int m_year = m_dateTime.GetYear();
	CString day, month, year;
	day.Format(L"%d", m_day);
	month.Format(L"%d", m_month);
	year.Format(L"%d", m_year);
	dateTime = _T("(") + day + _T("-") + month + _T("-") + year + _T(")");
}

void numberFormat(CString& cases)
{
	int index = cases.GetLength();
	if (index > 3) {
		for (index -= 3; index > 0; index -= 3)
			cases.Insert(index, ',');
	}
}

LRESULT CClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam)) {
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam)) {
	case FD_READ:
		{
			CString temp;
			if (mRecv(temp) < 0)
				break;
			Split(temp, strResult);
			int flag1 = _ttoi(strResult[0]);
			int flag2 = _ttoi(strResult[1]);
			switch (flag1) {
			case 1:
				{
					if (flag2 == 1) {
						m_msgString += _T("Dang nhap thanh cong\r\n");
						GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
						GetDlgItem(IDC_WORLD1)->EnableWindow(TRUE);
						GetDlgItem(IDC_WORLD2)->EnableWindow(TRUE);
						GetDlgItem(IDC_VN1)->EnableWindow(TRUE);
						GetDlgItem(IDC_VN2)->EnableWindow(TRUE);
						GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
						GetDlgItem(IDC_SIGN_IN)->EnableWindow(FALSE);
					}
					else if (flag2 == 2)
						m_msgString += _T("Sai user name\r\n");
					else if (flag2 == 3)
						m_msgString += _T("Sai password\r\n");
					else
						m_msgString += _T("Dang nhap that bai\r\n");

					UpdateData(FALSE);
					break;
				}
			case 2:
				{
					if (flag2 == 1) {
						m_msgString += _T("Dang ky thanh cong\r\n");
						GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
						GetDlgItem(IDC_WORLD1)->EnableWindow(TRUE);
						GetDlgItem(IDC_WORLD2)->EnableWindow(TRUE);
						GetDlgItem(IDC_VN1)->EnableWindow(TRUE);
						GetDlgItem(IDC_VN2)->EnableWindow(TRUE);
						GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
						GetDlgItem(IDC_SIGN_IN)->EnableWindow(FALSE);
					}
					else if (flag2 == 2)
						m_msgString += _T("User name da ton tai\r\n");
					else
						m_msgString += _T("Dang ky that bai\r\n");

					UpdateData(FALSE);
					break;
				}
			case 4:
				{
					if (flag2 == 0)
						m_msgString += _T("Không có dữ liệu\r\n");
					else if (flag2 == 1)
						m_msgString += _T("Data lỗi\r\n");
					else {
						numberFormat(strResult[1]);
						m_msgString += dateTime;
						m_msgString += _T(" Thế giới có ");
						m_msgString += strResult[1];
						m_msgString += _T(" ca nhiễm covid 19\r\n");
					}

					UpdateData(FALSE);
					break;
				}
			case 5:
				{
					if (flag2 == 0)
						m_msgString += _T("Không có dữ liệu\r\n");
					else if (flag2 == 1)
						m_msgString += _T("Data lỗi\r\n");
					else {
						numberFormat(strResult[1]);
						m_msgString += dateTime;
						m_msgString += _T(" ") + m_countryName + _T(" có ");
						m_msgString += strResult[1];
						m_msgString += _T(" ca nhiễm covid 19\r\n");
					}

					UpdateData(FALSE);
					break;
				}
			case 6:
				{
					if (flag2 == 0)
						m_msgString += _T("Không có dữ liệu\r\n");
					else if (flag2 == 1)
						m_msgString += _T("Data lỗi\r\n");
					else {
						numberFormat(strResult[1]);
						m_msgString += dateTime;
						m_msgString += _T(" VN- Cả nước có ");
						m_msgString += strResult[1];
						m_msgString += _T(" ca nhiễm covid 19\r\n");
					}

					UpdateData(FALSE);
					break;
				}
			case 7:
				{
					if (flag2 == 0)
						m_msgString += _T("Không có dữ liệu\r\n");
					else if (flag2 == 1)
						m_msgString += _T("Data lỗi\r\n");
					else {
						numberFormat(strResult[1]);
						m_msgString += dateTime;
						m_msgString += _T(" VN- ") + m_provinceName + _T(" có ");
						m_msgString += strResult[1];
						m_msgString += _T(" ca nhiễm covid 19\r\n");
					}

					UpdateData(FALSE);
					break;
				}
			}
			break;
		}
	case FD_CLOSE:
		{
			closesocket(sClient);
			m_msgString += _T("Server da dong ket noi\r\n");
			GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
			GetDlgItem(IDC_WORLD1)->EnableWindow(FALSE);
			GetDlgItem(IDC_WORLD2)->EnableWindow(FALSE);
			GetDlgItem(IDC_VN1)->EnableWindow(FALSE);
			GetDlgItem(IDC_VN2)->EnableWindow(FALSE);
			GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SIGN_IN)->EnableWindow(TRUE);
			UpdateData(FALSE);
			break;
		}
	}
	return 0;
}

void CClientDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if (m_userName == "") {
		MessageBox(_T("Vui long nhap ten user"));
		return;
	}

	if (m_passWord == "") {
		MessageBox(_T("Vui long nhap password"));
		return;
	}

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET) {
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}

	servAdd.sin_family = AF_INET;
 	servAdd.sin_port = htons(PORT);
	char* cIP = ConvertToChar(IP);
	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(IP);
	if (servAdd.sin_addr.s_addr == INADDR_NONE) {
		host = (gethostbyname(cpy_IP));
		if (host == NULL) {
			MessageBox(_T("Khong the ket noi den server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0], host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		int err_code = WSAGetLastError();
		MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
		return;
	}

	Command = _T("1\r\n");
	Command += m_userName;
	Command += _T("\r\n");
	Command += m_passWord;
	Command += _T("\r\n");
	mSend(Command);
	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}

void CClientDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	INT_PTR i = MessageBox(_T("Ban muon logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	Command = _T("3\r\n");
	mSend(Command);
}

void CClientDlg::OnBnClickedSignIn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_userName == "") {
		MessageBox(_T("Vui long nhap ten user"));
		return;
	}

	if (m_passWord == "") {
		MessageBox(_T("Vui long password"));
		return;
	}

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET) {
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);
	char* cIP = ConvertToChar(IP);
	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(IP);
	if (servAdd.sin_addr.s_addr == INADDR_NONE) {
		host = (gethostbyname(cpy_IP));
		if (host == NULL) {
			MessageBox(_T("Khong the ket noi den server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0], host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
		return;
	}

	Command = _T("2\r\n");
	Command += m_userName;
	Command += _T("\r\n");
	Command += m_passWord;
	Command += _T("\r\n");
	mSend(Command);
	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}

void CClientDlg::OnBnClickedWorld1()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	getDateTime(dateTime);
	Command = _T("4\r\n");
	Command += dateTime;
	Command += "\r\n";
	mSend(Command);
}

void getCountryName(CString& country) {
	//Do if else gioi han nest 128 ma lai co toi 225 quoc gia nen can 2 vong if
	CString country_1(country);
	CString country_2(country);
	if (country_1 == _T("afghanistan") || country_1 == _T("af"))
		country_1 = "Afghanistan";
	else if (country_1 == _T("albania") || country_1 == _T("al"))
		country_1 = "Albania";
	else if (country_1 == _T("algeria") || country_1 == _T("dz"))
		country_1 = "Algeria";
	else if (country_1 == _T("andorra") || country_1 == _T("ad"))
		country_1 = "Andorra";
	else if (country_1 == _T("angola") || country_1 == _T("ao"))
		country_1 = "Angola";
	else if (country_1 == _T("anguilla") || country_1 == _T("al"))
		country_1 = "Anguilla";
	else if (country_1 == _T("antiguaandbarbuda") || country_1 == _T("ag"))
		country_1 = "Antigua and Barbuda";
	else if (country_1 == _T("argentina") || country_1 == _T("ar"))
		country_1 = "Argentina";
	else if (country_1 == _T("armenia") || country_1 == _T("am"))
		country_1 = "Armenia";
	else if (country_1 == _T("aruba") || country_1 == _T("aw"))
		country_1 = "Aruba";
	else if (country_1 == _T("australia") || country_1 == _T("au"))
		country_1 = "Australia";
	else if (country_1 == _T("austria") || country_1 == _T("at"))
		country_1 = "Austria";
	else if (country_1 == _T("azerbaijan") || country_1 == _T("az"))
		country_1 = "Azerbaijan";
	else if (country_1 == _T("bahamas") || country_1 == _T("bs"))
		country_1 = "Bahamas";
	else if (country_1 == _T("bahrain") || country_1 == _T("bh"))
		country_1 = "Bahrain";
	else if (country_1 == _T("bangladesh") || country_1 == _T("bd"))
		country_1 = "Bangladesh";
	else if (country_1 == _T("barbados") || country_1 == _T("bb"))
		country_1 = "Barbados";
	else if (country_1 == _T("belarus") || country_1 == _T("by"))
		country_1 = "Belarus";
	else if (country_1 == _T("belgium") || country_1 == _T("be"))
		country_1 = "Belgium";
	else if (country_1 == _T("belize") || country_1 == _T("bz"))
		country_1 = "Belize";
	else if (country_1 == _T("benin") || country_1 == _T("bj"))
		country_1 = "Benin";
	else if (country_1 == _T("bermuda") || country_1 == _T("bm"))
		country_1 = "Bermuda";
	else if (country_1 == _T("bhutan") || country_1 == _T("bt"))
		country_1 = "Bhutan";
	else if (country_1 == _T("bolivia") || country_1 == _T("bo"))
		country_1 = "Bolivia";
	else if (country_1 == _T("bosniaandherzegovina") || country_1 == _T("ba"))
		country_1 = "Bosnia and Herzegovina";
	else if (country_1 == _T("botswana") || country_1 == _T("bw"))
		country_1 = "Botswana";
	else if (country_1 == _T("brazil") || country_1 == _T("br"))
		country_1 = "Brazil";
	else if (country_1 == _T("britishvirginislands") || country_1 == _T("vg"))
		country_1 = "British Virgin Islands";
	else if (country_1 == _T("brunei") || country_1 == _T("bn"))
		country_1 = "Brunei";
	else if (country_1 == _T("bulgaria") || country_1 == _T("bg"))
		country_1 = "Bulgaria";
	else if (country_1 == _T("burkinafaso") || country_1 == _T("bf"))
		country_1 = "Burkina Faso";
	else if (country_1 == _T("burundi") || country_1 == _T("bi"))
		country_1 = "Burundi";
	else if (country_1 == _T("cambodia") || country_1 == _T("kh"))
		country_1 = "Cambodia";
	else if (country_1 == _T("cameroon") || country_1 == _T("cm"))
		country_1 = "Cameroon";
	else if (country_1 == _T("canada") || country_1 == _T("ca"))
		country_1 = "Canada";
	else if (country_1 == _T("caboverde") || country_1 == _T("cv"))
		country_1 = "Cabo Verde";
	else if (country_1 == _T("caymanislands") || country_1 == _T("ky"))
		country_1 = "Cayman Islands";
	else if (country_1 == _T("chad") || country_1 == _T("td"))
		country_1 = "Chad";
	else if (country_1 == _T("chile") || country_1 == _T("cl"))
		country_1 = "Chile";
	else if (country_1 == _T("china") || country_1 == _T("cn"))
		country_1 = "China";
	else if (country_1 == _T("colombia") || country_1 == _T("co"))
		country_1 = "Colombia";
	else if (country_1 == _T("comoros") || country_1 == _T("km"))
		country_1 = "Comoros";
	else if (country_1 == _T("congo") || country_1 == _T("cg"))
		country_1 = "Congo";
	else if (country_1 == _T("costarica") || country_1 == _T("cr"))
		country_1 = "Costa Rica";
	else if (country_1 == _T("croatia") || country_1 == _T("hr"))
		country_1 = "Croatia";
	else if (country_1 == _T("cuba") || country_1 == _T("cu"))
		country_1 = "Cuba";
	else if (country_1 == _T("cyprus") || country_1 == _T("cy"))
		country_1 = "Cyprus";
	else if (country_1 == _T("czechrepublic") || country_1 == _T("cz"))
		country_1 = "Czechia";
	else if (country_1 == _T("denmark") || country_1 == _T("dk"))
		country_1 = "Denmark";
	else if (country_1 == _T("djibouti") || country_1 == _T("dj"))
		country_1 = "Djibouti";
	else if (country_1 == _T("dominica") || country_1 == _T("dm"))
		country_1 = "Dominica";
	else if (country_1 == _T("dominicanrepublic") || country_1 == _T("do"))
		country_1 = "Dominican Republic";
	else if (country_1 == _T("ecuador") || country_1 == _T("ec"))
		country_1 = "Ecuador";
	else if (country_1 == _T("egypt") || country_1 == _T("eg"))
		country_1 = "Egypt";
	else if (country_1 == _T("elsalvador") || country_1 == _T("sv"))
		country_1 = "El Salvador";
	else if (country_1 == _T("equatorialguinea") || country_1 == _T("gq"))
		country_1 = "Equatorial Guinea";
	else if (country_1 == _T("eritrea") || country_1 == _T("er"))
		country_1 = "Eritrea";
	else if (country_1 == _T("estonia") || country_1 == _T("ee"))
		country_1 = "Estonia";
	else if (country_1 == _T("estonia") || country_1 == _T("ee"))
		country_1 = "Estonia";
	else if (country_1 == _T("ethiopia") || country_1 == _T("et"))
		country_1 = "Ethiopia";
	else if (country_1 == _T("falklandislands") || country_1 == _T("fk"))
		country_1 = "Falkland Islands";
	else if (country_1 == _T("faeroeislands") || country_1 == _T("fo"))
		country_1 = "Faeroe Islands";
	else if (country_1 == _T("fiji") || country_1 == _T("fj"))
		country_1 = "Fiji";
	else if (country_1 == _T("finland") || country_1 == _T("fi"))
		country_1 = "Finland";
	else if (country_1 == _T("france") || country_1 == _T("fr"))
		country_1 = "France";
	else if (country_1 == _T("frenchguiana") || country_1 == _T("gf"))
		country_1 = "French Guiana";
	else if (country_1 == _T("frenchpolynesia") || country_1 == _T("pf"))
		country_1 = "French Polynesia";
	else if (country_1 == _T("gabon") || country_1 == _T("ga"))
		country_1 = "Gabon";
	else if (country_1 == _T("gambia") || country_1 == _T("gm"))
		country_1 = "Gambia";
	else if (country_1 == _T("georgia") || country_1 == _T("ge"))
		country_1 = "Georgia";
	else if (country_1 == _T("germany") || country_1 == _T("de"))
		country_1 = "Germany";
	else if (country_1 == _T("ghana") || country_1 == _T("gh"))
		country_1 = "Ghana";
	else if (country_1 == _T("gibraltar") || country_1 == _T("gi"))
		country_1 = "Gibraltar";
	else if (country_1 == _T("greece") || country_1 == _T("gr"))
		country_1 = "Greece";
	else if (country_1 == _T("greenland") || country_1 == _T("gl"))
		country_1 = "Greenland";
	else if (country_1 == _T("grenada") || country_1 == _T("gd"))
		country_1 = "Grenada";
	else if (country_1 == _T("guadeloupe") || country_1 == _T("gp"))
		country_1 = "Guadeloupe";
	else if (country_1 == _T("Guatemala") || country_1 == _T("gt"))
		country_1 = "Guatemala";
	else if (country_1 == _T("Guinea") || country_1 == _T("gn"))
		country_1 = "Guinea";
	else if (country_1 == _T("guinea-bissau") || country_1 == _T("gw"))
		country_1 = "Guinea-Bissau";
	else if (country_1 == _T("guyana") || country_1 == _T("gy"))
		country_1 = "Guyana";
	else if (country_1 == _T("haiti") || country_1 == _T("ht"))
		country_1 = "Haiti";
	else if (country_1 == _T("vaticancity") || country_1 == _T("va"))
		country_1 = "Vatican City";
	else if (country_1 == _T("honduras") || country_1 == _T("hn"))
		country_1 = "Honduras";
	else if (country_1 == _T("hongkong") || country_1 == _T("hk"))
		country_1 = "Hong Kong";
	else if (country_1 == _T("hungary") || country_1 == _T("hu"))
		country_1 = "Hungary";
	else if (country_1 == _T("iceland") || country_1 == _T("is"))
		country_1 = "Iceland";
	else if (country_1 == _T("india") || country_1 == _T("in"))
		country_1 = "India";
	else if (country_1 == _T("indonesia") || country_1 == _T("id"))
		country_1 = "Indonesia";
	else if (country_1 == _T("iran") || country_1 == _T("ir"))
		country_1 = "Iran";
	else if (country_1 == _T("iraq") || country_1 == _T("iq"))
		country_1 = "Iraq";
	else if (country_1 == _T("ireland") || country_1 == _T("ie"))
		country_1 = "Ireland";
	else if (country_1 == _T("israel") || country_1 == _T("il"))
		country_1 = "Israel";
	else if (country_1 == _T("italy") || country_1 == _T("it"))
		country_1 = "Italy";
	else if (country_1 == _T("jamaica") || country_1 == _T("jm"))
		country_1 = "Jamaica";
	else if (country_1 == _T("japan") || country_1 == _T("jp"))
		country_1 = "Japan";
	else if (country_1 == _T("jordan") || country_1 == _T("jo"))
		country_1 = "Jordan";
	else if (country_1 == _T("kazakhstan") || country_1 == _T("kz"))
		country_1 = "Kazakhstan";
	else if (country_1 == _T("kenya") || country_1 == _T("ke"))
		country_1 = "Kenya";
	else if (country_1 == _T("korea") || country_1 == _T("kr"))
		country_1 = "S. Korea";
	else if (country_1 == _T("kuwait") || country_1 == _T("kw"))
		country_1 = "Kuwait";
	else if (country_1 == _T("kyrgyzstan") || country_1 == _T("kg"))
		country_1 = "Kyrgyzstan";
	else if (country_1 == _T("Laos") || country_1 == _T("la"))
		country_1 = "Laos";
	else if (country_1 == _T("latvia") || country_1 == _T("lv"))
		country_1 = "Latvia";
	else if (country_1 == _T("lebanon") || country_1 == _T("lb"))
		country_1 = "Lebanon";
	else if (country_1 == _T("lesotho") || country_1 == _T("ls"))
		country_1 = "Lesotho";
	else if (country_1 == _T("liberia") || country_1 == _T("lr"))
		country_1 = "Liberia";
	else if (country_1 == _T("libya") || country_1 == _T("ly"))
		country_1 = "Libya";
	else if (country_1 == _T("liechtenstein") || country_1 == _T("li"))
		country_1 = "Liechtenstein";
	else if (country_1 == _T("lithuania") || country_1 == _T("lt"))
		country_1 = "Lithuania";
	else if (country_1 == _T("luxembourg") || country_1 == _T("lu"))
		country_1 = "Luxembourg";
	else if (country_1 == _T("macao") || country_1 == _T("mo"))
		country_1 = "Macao";
	else if (country_1 == _T("northmacedonia") || country_1 == _T("mk"))
		country_1 = "North Macedonia";
	else if (country_1 == _T("madagascar") || country_1 == _T("mg"))
		country_1 = "Madagascar";
	else if (country_1 == _T("malawi") || country_1 == _T("mw"))
		country_1 = "Malawi";
	else if (country_1 == _T("malaysia") || country_1 == _T("my"))
		country_1 = "Malaysia";
	else if (country_1 == _T("maldives") || country_1 == _T("mv"))
		country_1 = "Maldives";
	else if (country_1 == _T("mali") || country_1 == _T("ml"))
		country_1 = "Mali";
	else if (country_1 == _T("malta") || country_1 == _T("mt"))
		country_1 = "Malta";
	else if (country_1 == _T("marshallislands") || country_1 == _T("mh"))
		country_1 = "Marshall Islands";
	else if (country_1 == _T("martinique") || country_1 == _T("mq"))
		country_1 = "Martinique";
	else if (country_1 == _T("mauritania") || country_1 == _T("mr"))
		country_1 = "Mauritania";
	else if (country_1 == _T("mauritius") || country_1 == _T("mu"))
		country_1 = "Mauritius";
	else 
		country_1 = "none";

	if (country_2 == _T("mayotte") || country_2 == _T("yt"))
		country_2 = "Mayotte";
	else if (country_2 == _T("mexico") || country_2 == _T("mx"))
		country_2 = "Mexico";
	else if (country_2 == _T("micronesia") || country_2 == _T("fm"))
		country_2 = "Micronesia";
	else if (country_2 == _T("moldova") || country_2 == _T("md"))
		country_2 = "Moldova";
	else if (country_2 == _T("monaco") || country_2 == _T("mc"))
		country_2 = "Monaco";
	else if (country_2 == _T("mongolia") || country_2 == _T("mn"))
		country_2 = "Mongolia";
	else if (country_2 == _T("montserrat") || country_2 == _T("ms"))
		country_2 = "Montserrat";
	else if (country_2 == _T("mali") || country_2 == _T("ml"))
		country_2 = "Mali";
	else if (country_2 == _T("morocco") || country_2 == _T("ma"))
		country_2 = "Morocco";
	else if (country_2 == _T("mozambique") || country_2 == _T("mz"))
		country_2 = "Mozambique";
	else if (country_2 == _T("myanmar") || country_2 == _T("mm"))
		country_2 = "Myanmar";
	else if (country_2 == _T("namibia") || country_2 == _T("na"))
		country_2 = "Namibia";
	else if (country_2 == _T("nepal") || country_2 == _T("np"))
		country_2 = "Nepal";
	else if (country_2 == _T("netherlands") || country_2 == _T("nl"))
		country_2 = "Netherlands";
	else if (country_2 == _T("newcaledonia") || country_2 == _T("nc"))
		country_2 = "New Caledonia";
	else if (country_2 == _T("newzealand") || country_2 == _T("nz"))
		country_2 = "New Zealand";
	else if (country_2 == _T("nicaragua") || country_2 == _T("ni"))
		country_2 = "Nicaragua";
	else if (country_2 == _T("niger") || country_2 == _T("ne"))
		country_2 = "Niger";
	else if (country_2 == _T("nigeria") || country_2 == _T("ng"))
		country_2 = "Nigeria";
	else if (country_2 == _T("norway") || country_2 == _T("no"))
		country_2 = "Norway";
	else if (country_2 == _T("oman") || country_2 == _T("om"))
		country_2 = "Oman";
	else if (country_2 == _T("pakistan") || country_2 == _T("pk"))
		country_2 = "Pakistan";
	else if (country_2 == _T("palau") || country_2 == _T("pw"))
		country_2 = "Palau";
	else if (country_2 == _T("palestine") || country_2 == _T("ps"))
		country_2 = "Palestine";
	else if (country_2 == _T("panama") || country_2 == _T("pa"))
		country_2 = "Panama";
	else if (country_2 == _T("papuanewguinea") || country_2 == _T("pg"))
		country_2 = "Papua New Guinea";
	else if (country_2 == _T("paraguay") || country_2 == _T("py"))
		country_2 = "Paraguay";
	else if (country_2 == _T("peru") || country_2 == _T("pe"))
		country_2 = "Peru";
	else if (country_2 == _T("philippines") || country_2 == _T("ph"))
		country_2 = "Philippines";
	else if (country_2 == _T("poland") || country_2 == _T("pl"))
		country_2 = "Poland";
	else if (country_2 == _T("portugal") || country_2 == _T("pt"))
		country_2 = "Portugal";
	else if (country_2 == _T("qatar") || country_2 == _T("qa"))
		country_2 = "Qatar";
	else if (country_2 == _T("romania") || country_2 == _T("ro"))
		country_2 = "Romania";
	else if (country_2 == _T("russia") || country_2 == _T("ru"))
		country_2 = "Russia";
	else if (country_2 == _T("rwanda") || country_2 == _T("rw"))
		country_2 = "Rwanda";
	else if (country_2 == _T("sainthelena") || country_2 == _T("sh"))
		country_2 = "Saint Helena";
	else if (country_2 == _T("saintkittsandnevis") || country_2 == _T("kn"))
		country_2 = "Saint Kitts and Nevis";
	else if (country_2 == _T("saintlucia") || country_2 == _T("lc"))
		country_2 = "Saint Lucia";
	else if (country_2 == _T("saintpierremiquelon") || country_2 == _T("pm"))
		country_2 = "Saint Pierre Miquelon";
	else if (country_2 == _T("saintmartin") || country_2 == _T("mf"))
		country_2 = "Saint Martin";
	else if (country_2 == _T("samoa") || country_2 == _T("ws"))
		country_2 = "Samoa";
	else if (country_2 == _T("sanmarino") || country_2 == _T("sm"))
		country_2 = "San Marino";
	else if (country_2 == _T("saotomeandprincipe") || country_2 == _T("st"))
		country_2 = "Sao Tome and Principe";
	else if (country_2 == _T("saudiarabia") || country_2 == _T("sa"))
		country_2 = "Saudi Arabia";
	else if (country_2 == _T("senegal") || country_2 == _T("sn"))
		country_2 = "Senegal";
	else if (country_2 == _T("serbia") || country_2 == _T("cs"))
		country_2 = "Serbia";
	else if (country_2 == _T("seychelles") || country_2 == _T("sc"))
		country_2 = "Seychelles";
	else if (country_2 == _T("sierraleone") || country_2 == _T("sl"))
		country_2 = "Sierra Leone";
	else if (country_2 == _T("singapore") || country_2 == _T("sg"))
		country_2 = "Singapore";
	else if (country_2 == _T("slovakia") || country_2 == _T("sk"))
		country_2 = "Slovakia";
	else if (country_2 == _T("slovenia") || country_2 == _T("si"))
		country_2 = "Slovenia";
	else if (country_2 == _T("solomonislands") || country_2 == _T("sb"))
		country_2 = "Solomon Islands";
	else if (country_2 == _T("somalia") || country_2 == _T("so"))
		country_2 = "Somalia";
	else if (country_2 == _T("southafrica") || country_2 == _T("za"))
		country_2 = "South Africa";
	else if (country_2 == _T("southsudan") || country_2 == _T("ss"))
		country_2 = "South Sudan";
	else if (country_2 == _T("spain") || country_2 == _T("es"))
		country_2 = "Spain";
	else if (country_2 == _T("srilanka") || country_2 == _T("lk"))
		country_2 = "Sri Lanka";
	else if (country_2 == _T("sudan") || country_2 == _T("sd"))
		country_2 = "Sudan";
	else if (country_2 == _T("suriname") || country_2 == _T("sr"))
		country_2 = "Suriname";
	else if (country_2 == _T("eswatini") || country_2 == _T("sz"))
		country_2 = "Eswatini";
	else if (country_2 == _T("sweden") || country_2 == _T("se"))
		country_2 = "Sweden";
	else if (country_2 == _T("syria") || country_2 == _T("sy"))
		country_2 = "Syria";
	else if (country_2 == _T("taiwan") || country_2 == _T("tw"))
		country_2 = "Taiwan";
	else if (country_2 == _T("tajikistan") || country_2 == _T("tj"))
		country_2 = "Tajikistan";
	else if (country_2 == _T("tanzania") || country_2 == _T("tz"))
		country_2 = "Tanzania";
	else if (country_2 == _T("thailand") || country_2 == _T("th"))
		country_2 = "Thailand";
	else if (country_2 == _T("timor-leste") || country_2 == _T("tl"))
		country_2 = "Timor-Leste";
	else if (country_2 == _T("togo") || country_2 == _T("tg"))
		country_2 = "Togo";
	else if (country_2 == _T("tonga") || country_2 == _T("to"))
		country_2 = "Tonga";
	else if (country_2 == _T("trinidadandtobago") || country_2 == _T("tt"))
		country_2 = "Trinidad and Tobago";
	else if (country_2 == _T("tunisia") || country_2 == _T("tn"))
		country_2 = "Tunisia";
	else if (country_2 == _T("turkey") || country_2 == _T("tr"))
		country_2 = "Turkey";
	else if (country_2 == _T("turksandcaicos") || country_2 == _T("tc"))
		country_2 = "Turks and Caicos";
	else if (country_2 == _T("uganda") || country_2 == _T("ug"))
		country_2 = "Uganda";
	else if (country_2 == _T("ukraine") || country_2 == _T("ua"))
		country_2 = "Ukraine";
	else if (country_2 == _T("uae") || country_2 == _T("unitedarabemirates") ||
		country_2 == _T("u.a.e") || country_2 == _T("ae"))
		country_2 = "UAE";
	else if (country_2 == _T("unitedkingdom") || country_2 == _T("uk") ||
		country_2 == _T("england") || country_2 == _T("british") || country_2 == _T("gb"))
		country_2 = "UK";
	else if (country_2 == _T("unitedstates") || country_2 == _T("usa") ||
		country_2 == _T("hoaky") || country_2 == _T("my") || country_2 == _T("us"))
		country_2 = "USA";
	else if (country_2 == _T("uruguay") || country_2 == _T("uy"))
		country_2 = "Uruguay";
	else if (country_2 == _T("uzbekistan") || country_2 == _T("uz"))
		country_2 = "Uzbekistan";
	else if (country_2 == _T("vanuatu") || country_2 == _T("vu"))
		country_2 = "Vanuatu";
	else if (country_2 == _T("venezuela") || country_2 == _T("ve"))
		country_2 = "Venezuela";
	else if (country_2 == _T("vietnam") || country_2 == _T("vn"))
		country_2 = "Vietnam";
	else if (country_2 == _T("wallisandfutuna") || country_2 == _T("wf"))
		country_2 = "Wallis and Futuna";
	else if (country_2 == _T("westernsahara") || country_2 == _T("eh"))
		country_2 = "Western Sahara";
	else if (country_2 == _T("yemen") || country_2 == _T("ye"))
		country_2 = "Yemen";
	else if (country_2 == _T("zambia") || country_2 == _T("zm"))
		country_2 = "Zambia";
	else if (country_2 == _T("zimbabwe") || country_2 == _T("zw"))
		country_2 = "Zimbabwe";
	else
		country_2 = "none";
	//Kiem tra country co match quoc gia nao hay khong
	if (country_1 != _T("none"))
		country = country_1;
	else if (country_2 != _T("none"))
		country = country_2;
	else
		country = "none";
}

void getPronvinceName(CString& province , CString& send_province)
{
	if (province == _T("hànội") || province == _T("hÀnỘi") || province == _T("hn") ||
		province == _T("hanoi") || province == _T("tp.hanoi") ||
		province == _T("tp.hànội") || province == _T("tp.hÀnỘi")) {
		send_province = _T("HN");
		province = _T("TP. Hà Nội");
	}
	else if (province == _T("hànam") || province == _T("hÀnam") || 
		province == _T("ha") || province == _T("hanam")) {
		send_province = "HA";
		province = "Hà Nam";
	}
	else if (province == _T("tp.hồchíminh") || province == _T("tp.hỒchÍminh") ||
		province == _T("hồchíminh") || province == _T("hcm") || province == _T("hỒchÍminh") ||
		province == _T("tp.hochiminh") || province == _T("hochiminh") ||
		province == _T("tp.hcm") || province == _T("hm")) {
		send_province = "HM";
		province = "TP. Ho Chi Minh";
	}
	else if (province == _T("thanhhóa") || province == _T("thanhhÓa") ||
		province == _T("thanhhoa") || province == _T("th")) {
		send_province = "TH";
		province = "Thanh Hóa";
	}
	else if (province == _T("nghệan") || province == _T("nghỆan") ||
		province == _T("na") || province == _T("nghean")) {
		send_province = "NA";
		province = "Nghe An";
	}
	else if (province == _T("Đồngnai") || province == _T("ĐỒngnai") || province == _T("dongnai") ||
		province == _T("đồngnai") || province == _T("dn")) {
		send_province = "DN";
		province = "Dong Nai";
	}
	else if (province == _T("bìnhĐịnh") || province == _T("bÌnhĐỊnh") || province == _T("binhdinh") ||
		province == _T("bìnhđịnh") || province == _T("bđ") || province == _T("bĐ")) {
		send_province = "BĐ";
		province = "Binh Dinh";
	}
	else if (province == _T("angiang") || province == _T("ag")) {
		send_province = "AG";
		province = "An giang";
	}
	else if (province == _T("hảidương") || province == _T("hẢidƯƠng") || province == _T("hd") ||
		province == _T("haiduong")) {
		send_province = "HD";
		province = "Hai Duong";
	}
	else if (province == _T("Đắklắk") || province == _T("đắklắk") || province == _T("ĐẮklẮk") ||
		province == _T("daklak") || province == _T("dl")) {
		send_province = "DL";
		province = "Dak Lak";
	}
	else if (province == _T("tháibình") || province == _T("thÁibÌnh") ||
		province == _T("thaibinh") || province == _T("tb")) {
		send_province = "TB";
		province = "Thái Bình";
	}
	else if (province == _T("hảiphòng") || province == _T("hẢiphÒng") ||
		province == _T("haiphong") || province == _T("hp")) {
		send_province = "HP";
		province = "Hai Phong";
	}
	else if (province == _T("bắcgiang") || province == _T("bẮcgiang") ||
		province == _T("bacgiang") || province == _T("bg")) {
		send_province = "BG";
		province = "Bac Giang";
	}
	else if (province == _T("namĐịnh") || province == _T("namĐỊnh") || 
		province == _T("namđịnh") || province == _T("namdinh") || province == _T("nd")) {
		send_province = "ND";
		province = "Nam Dinh";
	}
	else if (province == _T("tiềngiang") || province == _T("tiỀngiang") ||
		province == _T("tiengiang") || province == _T("tg")) {
		send_province = "TG";
		province = "Tien Giang";
	}
	else if (province == _T("kiêngiang") || province == _T("kiÊngiang") ||
		province == _T("kiengiang") || province == _T("kg")) {
		send_province = "KG";
		province = "Kiên Giang";
	}
	else if (province == _T("longan") || province == _T("la")) {
		send_province = "LA";
		province = "Long An";
	}
	else if (province == _T("Đồngtháp") || province == _T("đồngtháp") ||
		province == _T("ĐỒngthÁp") || province == _T("dt") || province == _T("dongthap")) {
		send_province = "DT";
		province = "Dong Thap";
	}
	else if (province == _T("gialai") || province == _T("gl")) {
		send_province = "GL";
		province = "Gia Lai";
	}
	else if (province == _T("quảngngãi") || province == _T("quẢngngÃi") ||
		province == _T("quangngai") || province == _T("qn")) {
		send_province = "QN";
		province = "Quang Ngai";
	}
	else if (province == _T("phúthọ") || province == _T("phÚthỌ") ||
		province == _T("phutho") || province == _T("pt")) {
		send_province = "PT";
		province = "Phu Tho";
	}
	else if (province == _T("bắcninh") || province == _T("bẮcninh") ||
		province == _T("bacninh") || province == _T("bn")) {
		send_province = "BN";
		province = "Bac Ninh";
	}
	else if (province == _T("lâmĐồng") || province == _T("lÂmĐỒng") || province == _T("lâmđồng") ||
		province == _T("lamdong") || province == _T("ld")) {
		send_province = "LD";
		province = "Lam Dong";
	}
	else if (province == _T("hàtĩnh") || province == _T("hÀtĨnh") ||
		province == _T("hatinh") || province == _T("ht")) {
		send_province = "HT";
		province = "Ha Tinh";
	}
	else if (province == _T("bìnhthuận") || province == _T("bÌnhthuẬn") ||
		province == _T("binhthuan") || province == _T("bh")) {
		send_province = "BH";
		province = "Binh Thuan";
	}
	else if (province == _T("tâyninh") || province == _T("tÂyninh") ||
		province == _T("tayninh") || province == _T("tn")) {
		send_province = "TN";
		province = "Tây Ninh";
	}
	else if (province == _T("hưngyên") || province == _T("hƯngyÊn") ||
		province == _T("hungyen") || province == _T("hy")) {
		send_province = "HY";
		province = "Hung Yen";
	}
	else if (province == _T("sơnla") || province == _T("sƠnla") ||
		province == _T("sonla") || province == _T("sl")) {
		send_province = "SL";
		province = "Son La";
	}
	else if (province == _T("cầnthơ") || province == _T("cẦnthƠ") ||
		province == _T("cantho") || province == _T("ct")) {
		send_province = "CT";
		province = "Can Tho";
	}
	else if (province == _T("khánhhòa") || province == _T("khÁnhhÒa") ||
		province == _T("khanhhoa") || province == _T("kh")) {
		send_province = "KH";
		province = "Khánh Hòa";
	}
	else if (province == _T("sóctrăng") || province == _T("sÓctrĂng") ||
		province == _T("soctrang") || province == _T("st")) {
		send_province = "ST";
		province = "Soc Trang";
	}
	else if (province == _T("càmau") || province == _T("cÀmau") ||
		province == _T("camau") || province == _T("cm")) {
		send_province = "CM";
		province = "Cà Mau";
	}
	else if (province == _T("vĩnhphúc") || province == _T("vĨnhphÚc") ||
		province == _T("vinhphuc") || province == _T("vp")) {
		send_province = "VP";
		province = "Vinh Phuc";
	}
	else if (province == _T("bàrịa-vũngtàu") || province == _T("bÀrỊa-vŨngtÀu") ||
		province == _T("bàrịavũngtàu") || province == _T("bÀrỊavŨngtÀu") ||
		province == _T("bariavungtau") || province == _T("baria-vungtau") ||
		province == _T("br")) {
		send_province = "BR";
		province = "Ba Ria-Vung Tau";
	}
	else if (province == _T("Đắknông") || province == _T("ĐẮknÔng") ||
		province == _T("đắknông") || province == _T("daknong") ||
		province == _T("đn") || province == _T("Đn")) {
		send_province = "ĐN";
		province = "Dak Nong";
	}
	else if (province == _T("thừathiênhuế") || province == _T("thỪathiÊnhuẾ") ||
		province == _T("thuathienhue") || province == _T("tt")) {
		send_province = "TT";
		province = "Thua Thien Hue";
	}
	else if (province == _T("vĩnhlong") || province == _T("vĨnhlong") ||
		province == _T("vinhlong") || province == _T("vl")) {
		send_province = "VL";
		province = "Vinh Long";
	}
	else if (province == _T("tràvinh") || province == _T("trÀvinh") ||
		province == _T("travinh") || province == _T("tv")) {
		send_province = "TV";
		province = "Trà Vinh";
	}
	else if (province == _T("bìnhphước") || province == _T("bÌnhphƯỚc") ||
		province == _T("binhphuoc") || province == _T("bp")) {
		send_province = "BP";
		province = "Binh Phuoc";
	}
	else if (province == _T("ninhbình") || province == _T("ninhbÌnh") ||
		province == _T("ninhbinh") || province == _T("nb")) {
		send_province = "NB";
		province = "Ninh Bình";
	}
	else if (province == _T("phúyên") || province == _T("phÚyÊn") ||
		province == _T("phuyen") || province == _T("py")) {
		send_province = "PY";
		province = "Phú Yên";
	}
	else if (province == _T("bạcliêu") || province == _T("bẠcliÊu") ||
		province == _T("baclieu") || province == _T("bl")) {
		send_province = "BL";
		province = "Bac Lieu";
	}
	else if (province == _T("quảngbình") || province == _T("quẢngbÌnh") ||
		province == _T("quangbinh") || province == _T("qb")) {
		send_province = "QB";
		province = "Quang Binh";
	}
	else if (province == _T("hậugiang") || province == _T("hẬugiang") ||
		province == _T("haugiang") || province == _T("hg")) {
		send_province = "HG";
		province = "Hau Giang";
	}
	else if (province == _T("hòabình") || province == _T("hÒabÌnh") ||
		province == _T("hoabinh") || province == _T("hb")) {
		send_province = "HB";
		province = "Hòa Bình";
	}
	else if (province == _T("yênbái") || province == _T("yÊnbÁi") ||
		province == _T("yenbai") || province == _T("yb")) {
		send_province = "YB";
		province = "Yên Bái";
	}
	else if (province == _T("tuyênquang") || province == _T("tuyÊnquang") ||
		province == _T("tuyenquang") || province == _T("tq")) {
		send_province = "TQ";
		province = "Tuyên Quang";
	}
	else if (province == _T("lạngsơn") || province == _T("lẠngsƠn") ||
		province == _T("langson") || province == _T("ls")) {
		send_province = "LS";
		province = "Lang Son";
	}
	else if (province == _T("laichâu") || province == _T("laichÂu") ||
		province == _T("laichau") || province == _T("lc")) {
		send_province = "LC";
		province = "Lai Châu";
	}
	else if (province == _T("quảngtrị") || province == _T("quẢngtrỊ") ||
		province == _T("quangtri") || province == _T("qt")) {
		send_province = "QT";
		province = "Quang Tri";
	}
	else if (province == _T("Điệnbiên") || province == _T("ĐiỆnbiÊn") ||
		province == _T("điệnbiên") || province == _T("dienbien") ||
		province == _T("đb") || province == _T("Đb")) {
		send_province = "ĐB";
		province = "Dien Bien";
	}
	else if (province == _T("ninhthuận") || province == _T("ninhthuẬn") ||
		province == _T("ninhthuan") || province == _T("nt")) {
		send_province = "NT";
		province = "Ninh Thuan";
	}
	else if (province == _T("kontum") || province == _T("kt")) {
		send_province = "KT";
		province = "Kon Tum";
	}
	else if (province == _T("caobằng") || province == _T("caobẰng") ||
		province == _T("caobang") || province == _T("cb")) {
		send_province = "CB";
		province = "Cao Bang";
	}
	else if (province == _T("bắckạn") || province == _T("bẮckẠn") ||
		province == _T("backan") || province == _T("bk")) {
		send_province = "BK";
		province = "Bac Kan";
	}
	else if (province == _T("bìnhdương") || province == _T("bÌnhdƯƠng") ||
		province == _T("binhduong") || province == _T("bd")) {
		send_province = "BD";
		province = "Binh Duong";
	}
	else if (province == _T("quảngninh") || province == _T("quẢngninh") ||
		province == _T("quangninh") || province == _T("qh")) {
		send_province = "QH";
		province = "Quang Ninh";
	}
	else if (province == _T("quảngngãi") || province == _T("quẢngngÃi") ||
		province == _T("quangngai") || province == _T("qa")) {
		send_province = "QA";
		province = "Quang Ngai";
	}
	else if (province == _T("bếntre") || province == _T("bẾntre") ||
		province == _T("bentre") || province == _T("bt")) {
		send_province = "BT";
		province = "Ben Tre";
	}
	else if (province == _T("tháinguyên") || province == _T("thÁinguyÊn") ||
		province == _T("thainguyen") || province == _T("ti")) {
		send_province = "TI";
		province = "Thái Nguyên";
	}
	else if (province == _T("hàgiang") || province == _T("hÀgiang") ||
		province == _T("hagiang") || province == _T("hi")) {
		send_province = "HI";
		province = "Hà Giang";
	}
	else if (province == _T("Đànẵng") || province == _T("ĐÀnẴng") ||
		province == _T("đànẵng") || province == _T("danang") ||
		province == _T("Đg") || province == _T("đg")) {
		send_province = "ĐG";
		province = "Da Nang";
	}
	else if (province == _T("làocai") || province == _T("lÀocai") ||
		province == _T("laocai") || province == _T("li")) {
		send_province = "LI";
		province = "Lào Cai";
	}
	else {
		province = "none";
		send_province = "none";
	}
}

void CClientDlg::OnBnClickedWorld2()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	getDateTime(dateTime);
	Command = _T("5\r\n");
	Command += dateTime;
	Command += "\r\n";
	m_countryName.MakeLower();
	m_countryName.Remove(' ');
	getCountryName(m_countryName);
	if (m_countryName == _T("none")) {
		MessageBox(_T("Can't find country"));
		return;
	}
	else {
		Command += m_countryName;
		Command += "\r\n";
		mSend(Command);
	}
}

void CClientDlg::OnBnClickedVn1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	getDateTime(dateTime);
	Command = _T("6\r\n");
	Command += dateTime;
	Command += "\r\n";
	mSend(Command);
}


void CClientDlg::OnBnClickedVn2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	getDateTime(dateTime);
	Command = _T("7\r\n");
	Command += dateTime;
	Command += "\r\n";
	CString send_province;
	m_provinceName.MakeLower();
	m_provinceName.Remove(' ');
	getPronvinceName(m_provinceName, send_province);
	if (send_province == _T("none")) {
		MessageBox(_T("Can't find province"));
		return;
	}
	else {
		Command += send_province;
		Command += "\r\n";
		mSend(Command);
	}
}
