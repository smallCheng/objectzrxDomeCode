#pragma once
class CAppDirectoryUtil
{
public:
	CAppDirectoryUtil();
	~CAppDirectoryUtil();

	//用于获取正在运行的ARX文件所在的文件夹
	static CString GetCurrentDirctory(HMODULE hInstance= _hdllInstance);
	//用于获取正在运行的ARX文件所在目录的上一层文件夹
	static CString GetParentDirectory(HMODULE hInstance= _hdllInstance);
};