#pragma once
class CBlockUtil
{
public:
	CBlockUtil();
	~CBlockUtil();

	//插入块参照（参数：块定义ID、插入点、缩放比例、旋转角度）
	static AcDbObjectId InsertBlockRef(AcDbObjectId blkDefId, const AcGePoint3d &insertPoint, double scale, double rotation);
	//修改插入块参照的插入比例
	static void SetScaleFactor(AcDbBlockReference *pBlkRef, double scale);
	//获取指定名称的块定义
	static AcDbObjectId GetBlkDefId(const TCHAR* blkDefName);
	//插入一个到带有属性定义的图块，所有的属性值使用定义属性定义的默认值
	static AcDbObjectId InsertBlockRefWithAttribute(const AcDbObjectId &blkDefId, const AcGePoint3d &insertPoint, double scale, double rotation);
	//根据输入的参数中的属性定义，构建与属性定义匹配的属性对象，将其添加到输入的块参照
	static void AppendAttributeToBlkRef(AcDbBlockReference *pBlkRef, AcDbAttributeDefinition *pAttDef);
	//修改块参照中特定名称的属性对象的字符串内容
	static bool SetBlockRefAttribute(AcDbBlockReference *pBlikRef, const TCHAR *tag, const TCHAR *val);
	//从块参照中提取属性值
	static bool GetAttributeValue(AcDbBlockReference *pBlkRef, const TCHAR *tag, CString &value);
	//获取块参照和其他实体相交的真实相交
	static void IntersectWith(AcDbBlockReference *pBlkRef, AcDbEntity *pEnt, AcGePoint3dArray &intPonts, bool bIntersectWithText = true, bool bProjectToXOY = false, bool bExtendEntArg = false);
};

