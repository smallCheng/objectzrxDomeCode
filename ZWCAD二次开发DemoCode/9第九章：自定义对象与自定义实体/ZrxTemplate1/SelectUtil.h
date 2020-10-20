#pragma once
class CSelectUtil
{
public:
	CSelectUtil();
	~CSelectUtil();

	//获取当前图形中位于给定矩形范围内的所有实体(以实体的包围框进行判断
	static void GetEntityInRectangle(const AcGePoint2d &firstPoint, const AcGePoint2d &secondPoint, AcDbObjectIdArray &entIds);
	//提示用户选择多个实体（过滤）
	static bool PromptSelectEnts(const TCHAR *prompt, struct resbuf * rb, AcDbObjectIdArray &entIds);
	//提示用户选择多个实体（限定多种实体类型）
	static bool PromptSelectEnts(const TCHAR *prompt, const std::vector<AcRxClass*> &classDescs, AcDbObjectIdArray &entIds);
	//提示用户选择多个实体（限定一种实体类型）
	static bool PromptSelectEnts(const TCHAR *prompt, AcRxClass* &classDesc, AcDbObjectIdArray &entIds);
	//提示用户选择一个实体（限定多种实体类型）
	static bool PromptSelectEntity(const TCHAR *prompt, const std::vector<AcRxClass*> &classDescs, AcDbEntity * &pEnt,AcGePoint3d &pickPoint,bool bOpenFoeWrite=true);
	//提示用户选择一个实体（限定一种实体类型）
	static bool PromptSelectEntity(const TCHAR *prompt, AcRxClass* &classDesc, AcDbEntity * &pEnt, AcGePoint3d &pickPoint, bool bOpenFoeWrite = true);


};

