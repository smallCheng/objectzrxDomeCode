#pragma once
#include <gedxfio.h>
class ZrxCreateEntitys
{
public:
	ZrxCreateEntitys();
	~ZrxCreateEntitys();
public:
	//添加实体到模型空间
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt);


	//创建直线（参数：起点、终点）
	static AcDbObjectId CreateLine(AcGePoint3d ptStart, AcGePoint3d ptEnd);


	//创建圆(参数：圆心、半径)
	static AcDbObjectId CreateCircle(const AcGePoint3d &centerPoint,double radius );
	//创建圆(参数：直径两端点)
	static AcDbObjectId CreateCircle(const AcGePoint3d &pt1, const AcGePoint3d &pt2);
	//创建圆（参数：圆上三点）
	static AcDbObjectId CreateCircle(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &pt3);
	//创建圆（参数：圆上三点）
	static AcDbObjectId CreateCircle(const AcGePoint2d &pt1, const AcGePoint2d &pt2, const AcGePoint2d &pt3);


	//创建圆弧（参数：圆心、平面法向量、半径、起始角度、终止角度）
	static AcDbObjectId CreateArc(const AcGePoint3d &ptCenter, const AcGeVector3d &vec, double radius, double startAngle, double endAngle);
	//在XOY平面创建圆弧（参数：圆心、半径、起始角度、终止角度）
	static AcDbObjectId CreateArc(const AcGePoint2d &ptCenter, double radius, double startAngle, double endAngle);
	//创建圆弧  （参数1：起点、终点、圆心 or 参数2：起点、终点、弧上一点）
	static AcDbObjectId CreateArc(const AcGePoint2d &ptStart, const AcGePoint2d &ptCenter_or_ptOnArc, const AcGePoint2d &ptEnd, BOOL flay = TRUE);
	//创建圆弧（参数：起点、圆心、圆弧角度）
	static AcDbObjectId CreateArc(const AcGePoint2d &ptStart, const AcGePoint2d &ptCenter, double angle);


	//创建多段线（轻量多段线）（参数：点集、宽度）
	static AcDbObjectId CreatePolyLine(const AcGePoint2dArray &points, double width = 0);
	//创建多段线（一条直线）(参数：开始点、起始点、线宽)
	static AcDbObjectId CreatePolyOneLine(const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, double width);
	//创建三维多段线（参数：点集）
	static AcDbObjectId Create3dPolyLine(const AcGePoint3dArray &points);
	//创建正多边形（参数：中心、边数、外接圆半径、旋转角度（弧度值）、线宽）（由多段线构建）
	static AcDbObjectId CreatePolygon(const AcGePoint2d &ptCenter, int number,double radius, double rotation, double width);
	//创建矩形（参数：两个角点、线宽）（由多段线构建）
	static AcDbObjectId CreatePolyRectangle(const AcGePoint2d &pt1, const AcGePoint2d &pt2, double width);
	//创建圆（参数：圆心、半径和线宽）（由多段线构建）
	static AcDbObjectId CreatePolyCircle(const AcGePoint2d &ptCenter, double radius, double width);
	//创建圆弧（参数：圆心、半径、起始角度、终止角度、线宽）（由多段线构建）
	static AcDbObjectId CreatePolyArc(const AcGePoint2d &ptCenter, double radius ,double angleStart, double angleEnd, double width);

	//创建椭圆(参数:椭圆中心、平面法向量、1/2长轴矢量、短轴/长轴比例)
	static AcDbObjectId CreateEillpse(const AcGePoint3d &ptCenter, const AcGeVector3d &vecNormal, const AcGeVector3d &majorAxis, double ratio);
	//创建椭圆（参数：外接矩形的两个角点）
	static AcDbObjectId CreateEillpse(const AcGePoint2d &pt1, const AcGePoint2d &pt2);


	//创建样条曲线（参数：拟合点、阶数、允许拟合误差）
	static AcDbObjectId CreateSpline(const AcGePoint3dArray &points, int order = 4, double fitTolerance = 0.0);
	//创建样条曲线（参数：拟合点、起点切线方向、终点切线方向、阶数、允许拟合误差）
	static AcDbObjectId CreateSpline(const AcGePoint3dArray &points, const AcGeVector3d &startTangent, const AcGeVector3d &endTangent, int order = 4, double fitTolerance = 0.0);

	
	//创建面域（参数：生成面域所需对象的ID数组）
	static AcDbObjectIdArray CreateRegion(const AcDbObjectIdArray &curveIds);


	//创建文字（参数：插入点、文字内容、文字样式ID、文字高度、文字的旋转角度）
	static AcDbObjectId CreateText(const AcGePoint3d &ptInsert, const TCHAR* text, AcDbObjectId style = AcDbObjectId::kNull , double height = 2.5, double rotation=0);
	//创建多行文字（参数：插入点、多行文字内容、文字样式ID、文字高度、宽度）
	static AcDbObjectId CreateMText(const AcGePoint3d &ptInsert, const TCHAR* text, AcDbObjectId style = AcDbObjectId::kNull, double height = 2.5, double width = 10);


	//创建填充（参数：填充边界ID数组、填充物名、填充比例）
	static AcDbObjectId CreateHatch(const AcDbObjectIdArray &loopIds, const TCHAR* patName, double patternScale);


	//创建对齐标注（参数：第一条尺寸边界线的起点、第二条尺寸边界线的起点、通过尺寸线的一点、标注文字、文字样式）
	static AcDbObjectId CreateDimAligned(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &ptLine, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建对齐标注（参数：第一条尺寸边界线的起点、第二条尺寸边界线的起点、通过尺寸线的一点、文字偏移量、标注文字、文字样式）
	static AcDbObjectId CreateDimAligned(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &ptLine,const AcGeVector3d &vecOffset=AcGeVector3d::kIdentity, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建转角标注（参数：标注的旋转角度、第一条尺寸边界线的起点、第二条尺寸边界线的起点、通过尺寸线的一点、标注文字、文字样式）
	static AcDbObjectId CreateDimRotated(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &ptLine, double rotation, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建半径标注（参数：曲线的中心点、引线附着坐标、引线长度、标注文字、文字样式）
	static AcDbObjectId CreateDimRadial(const AcGePoint3d &ptCenter, const AcGePoint3d &ptChord, double leaderLength, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建半径标注（参数：圆心、半径、尺寸线旋转角度、引线长度）
	static AcDbObjectId CreateDimRadial(const AcGePoint3d &ptCenter, double radius, double angle, double leaderLength = 5);
	//创建直径标注（参数：直径的两个端点，引线长度、标注文字、文字样式）
	static AcDbObjectId CreateDimDiameteric(const AcGePoint3d &ptChord1, const AcGePoint3d &ptChord2, double leaderLength, const TCHAR* dimText=NULL, AcDbObjectId dimStyle=AcDbObjectId::kNull);
	//创建直径标注（参数：圆心、半径、尺寸标注线的旋转角度、引线长度）
	static AcDbObjectId CreateDimDiameteric(const AcGePoint3d &ptCenter, double radius , double angle, double leaderLength=5);
	//创建角度标注（参数：根据两条直线的关系创建）
	static AcDbObjectId CreateDim2LineAngular(const AcGePoint3d &ptStart1, const AcGePoint3d &ptEnd1, const AcGePoint3d &ptStart2, const AcGePoint3d &ptEnd2, const AcGePoint3d &ptArc, const TCHAR* dimText, AcDbObjectId dimStyle);
	//创建角度标注（参数：顶点、起始点、终止点、标注尺寸线通过点）
	static AcDbObjectId CreateDim3PtAngular(const AcGePoint3d &ptCenter, const AcGePoint3d &ptEnd1, const AcGePoint3d &ptEnd2, const AcGePoint3d &ptArc, const TCHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建坐标标注（参数：是否是X轴标注、标注箭头的起始位置、标注箭头的终止位置、标注文字、文字样式）
	static AcDbObjectId CreateDimOrdinate(Adesk::Boolean xAxis, const AcGePoint3d &ptStart, const AcGePoint3d &ptEnd, const TCHAR* dimText=NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//创建坐标标注（同时创建一个点的X、Y坐标标注）
	static AcDbObjectIdArray CreateDimOrdinate(const AcGePoint3d &ptDef, const AcGePoint3d &ptTextX, const AcGePoint3d &ptTextY);
	//创建坐标标注（参数：标注插入点、X偏移量、Y偏移量）
	static AcDbObjectIdArray CreateDimOrdinate(const AcGePoint3d &ptDef, const AcGeVector3d &vecOffsetX, const AcGeVector3d &vecOffsetY);



};

