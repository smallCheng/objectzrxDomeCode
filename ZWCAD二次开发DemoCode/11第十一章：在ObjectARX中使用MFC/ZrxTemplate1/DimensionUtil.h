#pragma once
class CDimensionUtil
{
public:
	CDimensionUtil();
	~CDimensionUtil();


	//创建对齐标注（参数：第一条尺寸边界线的起点、第二条尺寸边界线的起点、通过尺寸线的一点、标注文字、文字样式）
	static AcDbObjectId AddDimAligned(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &ptLine, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建对齐标注（参数：第一条尺寸边界线的起点、第二条尺寸边界线的起点、通过尺寸线的一点、文字偏移量、标注文字、文字样式）
	static AcDbObjectId AddDimAligned(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &ptLine, const AcGeVector3d &vecOffset = AcGeVector3d::kIdentity, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建转角标注（参数：标注的旋转角度、第一条尺寸边界线的起点、第二条尺寸边界线的起点、通过尺寸线的一点、标注文字、文字样式）
	static AcDbObjectId AddDimRotated(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &ptLine, double rotation, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建半径标注（参数：曲线的中心点、引线附着坐标、引线长度、标注文字、文字样式）
	static AcDbObjectId AddDimRadial(const AcGePoint3d &ptCenter, const AcGePoint3d &ptChord, double leaderLength, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建半径标注（参数：圆心、半径、尺寸线旋转角度、引线长度）
	static AcDbObjectId AddDimRadial(const AcGePoint3d &ptCenter, double radius, double angle, double leaderLength = 5);
	//创建直径标注（参数：直径的两个端点，引线长度、标注文字、文字样式）
	static AcDbObjectId AddDimDiameteric(const AcGePoint3d &ptChord1, const AcGePoint3d &ptChord2, double leaderLength, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建直径标注（参数：圆心、半径、尺寸标注线的旋转角度、引线长度）
	static AcDbObjectId AddDimDiameteric(const AcGePoint3d &ptCenter, double radius, double angle, double leaderLength = 5);
	//创建角度标注（参数：根据两条直线的关系创建）
	static AcDbObjectId AddDim2LineAngular(const AcGePoint3d &ptStart1, const AcGePoint3d &ptEnd1, const AcGePoint3d &ptStart2, const AcGePoint3d &ptEnd2, const AcGePoint3d &ptArc, const TCHAR* dimText, AcDbObjectId dimStyle);
	//创建角度标注（参数：顶点、起始点、终止点、标注尺寸线通过点）
	static AcDbObjectId AddDim3PtAngular(const AcGePoint3d &ptCenter, const AcGePoint3d &ptEnd1, const AcGePoint3d &ptEnd2, const AcGePoint3d &ptArc, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建坐标标注（参数：是否是X轴标注、标注箭头的起始位置、标注箭头的终止位置、标注文字、文字样式）
	static AcDbObjectId AddDimOrdinate(Adesk::Boolean xAxis, const AcGePoint3d &ptStart, const AcGePoint3d &ptEnd, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建坐标标注（同时创建一个点的X、Y坐标标注）
	static AcDbObjectIdArray AddDimOrdinate(const AcGePoint3d &ptDef, const AcGePoint3d &ptTextX, const AcGePoint3d &ptTextY);
	//创建坐标标注（参数：标注插入点、X偏移量、Y偏移量）
	static AcDbObjectIdArray AddDimOrdinate(const AcGePoint3d &ptDef, const AcGeVector3d &vecOffsetX, const AcGeVector3d &vecOffsetY);
};

