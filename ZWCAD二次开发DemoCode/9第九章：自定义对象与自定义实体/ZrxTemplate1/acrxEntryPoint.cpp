//////////////////////////////////////////////////////////////////////////
// 本文件由向导生成
// 

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"



//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint

/**********自定义函数*****************/
#pragma region	自定义函数
//获取用户输入的线宽
ads_real GetWidth()
{
	ads_real width = 0;
	if (acedGetReal(_T("\n输入线宽:"), &width) == RTNORM)
	{
		return width;
	}
	else
	{
		return 0;
	}
}

//提示用户输入颜色索引值
int GetColorIndex()
{
	int colorIndex = 0;
	if (acedGetInt(_T("\n输入颜色索引值(0~256):"), &colorIndex) != RTNORM)
	{
		return 0;
	}
	//处理颜色索引值无效情况
	while (colorIndex < 0 || colorIndex>256)
	{
		acedPrompt(_T("\n输入了无效的颜色索引值."));
		if (acedGetInt(_T("\n输入颜色索引值(0~256)"), &colorIndex) != RTNORM)
		{
			return 0;
		}
	}
	return colorIndex;
}

// 根据多段线构建二维几何曲线
static bool PolyToGeCurve(AcDbPolyline *pPline, AcGeCurve2d *&pGeCurve)
{
	int nSegs;							// 多段线的段数
	AcGeLineSeg2d line, *pLine;			// 几何曲线的直线段部分
	AcGeCircArc2d arc, *pArc;			// 几何曲线的圆弧部分
	AcGeVoidPointerArray geCurves;		// 指向组成几何曲线各分段的指针数组										

	nSegs = pPline->numVerts() - 1;

	// 根据多段线创建对应的分段几何曲线
	for (int i = 0; i < nSegs; i++)
	{
		if (pPline->segType(i) == AcDbPolyline::kLine)
		{
			pPline->getLineSegAt(i, line);
			pLine = new AcGeLineSeg2d(line);
			geCurves.append(pLine);
		}
		else if (pPline->segType(i) == AcDbPolyline::kArc)
		{
			pPline->getArcSegAt(i, arc);
			pArc = new AcGeCircArc2d(arc);
			geCurves.append(pArc);
		}
	}

	// 处理闭合多段线最后一段是圆弧的情况
	if (pPline->isClosed() && pPline->segType(nSegs) == AcDbPolyline::kArc)
	{
		pPline->getArcSegAt(nSegs, arc);
		pArc = new AcGeCircArc2d(arc);
		pArc->setAngles(arc.startAng(), arc.endAng() -
			(arc.endAng() - arc.startAng()) / 100);
		geCurves.append(pArc);
	}

	// 根据分段的几何曲线创建对应的复合曲线
	if (geCurves.length() == 1)
	{
		pGeCurve = (AcGeCurve2d *)geCurves[0];
	}
	else
	{
		pGeCurve = new AcGeCompositeCurve2d(geCurves);
	}

	// 释放动态分配的内存
	if (geCurves.length() > 1)
	{
		for (int i = 0; i < geCurves.length(); i++)
		{
			delete geCurves[i];
		}
	}

	return true;
}

// 根据指定的一组点创建一个结果缓冲区链表
struct resbuf* BuildRbFromPtArray(const AcGePoint2dArray &arrPoints)
{
	struct resbuf *retRb = NULL;
	int count = arrPoints.length();
	if (count <= 1)
	{
		acedAlert(TEXT("函数BuildBbFromPtArray中, 点数组包含元素个数不足!"));
		return retRb;
	}

	// 使用第一个点来构建结果缓冲区链表的头节点
	ads_point adsPt;
	adsPt[X] = arrPoints[0].x;
	adsPt[Y] = arrPoints[0].y;
	retRb = acutBuildList(RTPOINT, adsPt, RTNONE);

	struct resbuf *nextRb = retRb;		// 辅助指针

	for (int i = 1; i < count; i++)			// 注意：不考虑第一个元素，因此i从1开始
	{
		adsPt[X] = arrPoints[i].x;
		adsPt[Y] = arrPoints[i].y;
		// 动态创建新的节点，并将其链接到原来的链表尾部
		nextRb->rbnext = acutBuildList(RTPOINT, adsPt, RTNONE);
		nextRb = nextRb->rbnext;
	}

	return retRb;
}

// 选择位于多段线内部的所有实体
static bool SelectEntInPoly(AcDbPolyline *pPline,
	AcDbObjectIdArray &ObjectIdArray, const ACHAR *selectMode, double approxEps)
{
	// 判断selectMode的有效性
	if (_tcscmp(selectMode, TEXT("CP")) != 0 && _tcscmp(selectMode, TEXT("WP")) != 0)
	{
		acedAlert(TEXT("函数SelectEntInPline中, 指定了无效的选择模式!"));
		return false;
	}

	// 清除数组中所有的ObjectId
	for (int i = 0; i < ObjectIdArray.length(); i++)
	{
		ObjectIdArray.removeAt(i);
	}

	AcGeCurve2d *pGeCurve;						// 多段线对应的几何曲线
	Adesk::Boolean bClosed = pPline->isClosed();	// 多段线是否闭合
	if (bClosed != Adesk::kTrue)				// 确保多段线作为选择边界时是闭合的
	{
		pPline->setClosed(!bClosed);
	}

	// 创建对应的几何类曲线
	PolyToGeCurve(pPline, pGeCurve);

	// 获得几何曲线的样本点
	AcGePoint2dArray SamplePtArray;				// 存储曲线的样本点
	AcGeDoubleArray ParamArray;					// 存储样本点对应的参数值
	AcGePoint2d ptStart, ptEnd;			// 几何曲线的起点和终点
	Adesk::Boolean bRet = pGeCurve->hasStartPoint(ptStart);
	bRet = pGeCurve->hasEndPoint(ptEnd);
	double valueSt = pGeCurve->paramOf(ptStart);
	double valueEn = pGeCurve->paramOf(ptEnd);
	pGeCurve->getSamplePoints(valueSt, valueEn, approxEps,
		SamplePtArray, ParamArray);

	delete pGeCurve;		// 在函数PolyToGeCurve中分配了内存	

	// 确保样本点的起点和终点不重合
	AcGeTol tol;
	tol.setEqualPoint(0.01);
	AcGePoint2d ptFirst = SamplePtArray[0];
	AcGePoint2d ptLast = SamplePtArray[SamplePtArray.length() - 1];
	if (ptFirst.isEqualTo(ptLast))
	{
		SamplePtArray.removeLast();
	}

	// 根据样本点创建结果缓冲区链表
	struct resbuf *rb;
	rb = BuildRbFromPtArray(SamplePtArray);

	// 使用acedSSGet函数创建选择集
	ads_name ssName;			// 选择集名称
	int rt = acedSSGet(selectMode, rb, NULL, NULL, ssName);
	if (rt != RTNORM)
	{
		acutRelRb(rb);			// 释放结果缓冲区链表
		return false;
	}

	// 将选择集中所有的对象添加到ObjectIdArray
	long length;
	acedSSLength(ssName, &length);
	for (int i = 0; i < length; i++)
	{
		// 获得指定元素的ObjectId
		ads_name ent;
		acedSSName(ssName, i, ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId, ent);

		// 获得指向当前元素的指针
		AcDbEntity *pEnt;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);

		// 选择到作为边界的多段线了，直接跳过该次循环
		if (es == Acad::eWasOpenForWrite)
		{
			continue;
		}

		ObjectIdArray.append(pEnt->objectId());

		pEnt->close();
	}

	// 释放内存
	acutRelRb(rb);								// 释放结果缓冲区链表
	acedSSFree(ssName);							// 删除选择集

	return true;
}

//获取AutoCAD程序的acad.exe的位置
bool GetAcadPath(CString &acadPath)
{
	DWORD dwRet = ::GetModuleFileName(acedGetAcadWinApp()->m_hInstance, acadPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	acadPath.ReleaseBuffer();
	if (dwRet == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//能够根据输入的实体ID获得相同参数的AcGeLineSeg2d对象
AcGeLineSeg2d GetGeLineObj(AcDbObjectId lineId)
{
	AcGeLineSeg2d geLine;
	AcDbLine *pLine=NULL;
	if(acdbOpenObject(pLine,lineId,AcDb::kForRead)==Acad::eOk)
	{
		geLine.set(CConvertUtil::ToPoint2d(pLine->startPoint()),CConvertUtil::ToPoint2d(pLine->endPoint()));
		pLine->close();
	}
	return geLine;
}

//使用一个点分数组来分隔多段线，将获得到分段线都添加的模型空间，并且从中删除第一段和最后一段曲线
void GetCurveBetweenIntPoints(AcDbPolyline *pPoly,const AcGePoint3dArray &intPoints)
{
	AcDbVoidPtrArray curves;
	pPoly->getSplitCurves(intPoints,curves);
	for(int i=0;i<curves.length();i++)
	{
		AcDbCurve *pCurve=static_cast<AcDbCurve*>(curves[i]);

		//删除首尾两条曲线，其他段的曲线添加到模型空间
		if(i==0||i==curves.length()-1)
		{
			delete pCurve;
		}
		else
		{
			AcDbObjectId curveId=CDwgDatabaseUtil::PostToModelSpace(pCurve);
		}
	}

}

// 根据用户输入的入口向量计算法向量
AcGeVector3d GetNormalByInVector(const AcGeVector3d &inVector)
{
	double tol = 1.0E-7;
	if (fabs(inVector.x) < tol && fabs(inVector.y) < tol)
	{
		if (inVector.z >= 0)
		{
			return AcGeVector3d(-1, 0, 0);
		}
		else
		{
			return AcGeVector3d::kXAxis;
		}
	}
	else
	{
		AcGeVector2d yAxis2d(inVector.x, inVector.y);
		yAxis2d = yAxis2d.rotateBy(CMathUtil::PI() * 0.5);
		AcGeVector3d yAxis(yAxis2d.x, yAxis2d.y, 0);
		AcGeVector3d normal = yAxis;
		normal = normal.rotateBy(CMathUtil::PI() * 0.5, inVector);
		return normal;
	}
}

// 获得从WCS到UCS的变换矩阵
AcGeMatrix3d GetTranslateMatrix(const AcGePoint3d &inPoint, const AcGeVector3d &inVector, const AcGeVector3d &normal)
{
	AcGeVector3d xAxis = inVector;
	xAxis.normalize();
	AcGeVector3d zAxis = normal;
	zAxis.normalize();
	AcGeVector3d yAxis(xAxis.x, xAxis.y, xAxis.z);
	yAxis = yAxis.rotateBy(CMathUtil::PI() * 0.5, zAxis);

	AcGeMatrix3d mat = AcGeMatrix3d::alignCoordSys(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis, AcGeVector3d::kZAxis, 
		inPoint, xAxis, yAxis, zAxis);

	return mat;
}

// 根据输入的管道起点、终点、宽、高来创建管道
void DrawPipe(const AcGePoint3d &startPoint, const AcGePoint3d &endPoint, double width, double height)
{
	// 获得变换矩阵
	AcGeVector3d inVector = endPoint - startPoint;      // 入口向量
	AcGeVector3d normal = GetNormalByInVector(inVector);       // 法向量
	AcGeMatrix3d mat = GetTranslateMatrix(startPoint, inVector, normal);

	// 顶面
	int colorIndex = 54;
	double z = 0.5 * height;
	double length = startPoint.distanceTo(endPoint);
	AcDbFace *pTopFace = new AcDbFace(AcGePoint3d(0, -0.5 * width, z), AcGePoint3d(length, -0.5 * width, z), 
		AcGePoint3d(length, 0.5 * width, z), AcGePoint3d(0, 0.5 * width, z));
	pTopFace->transformBy(mat);
	pTopFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pTopFace);

	// 底面
	z = -0.5 * height;
	AcDbFace *pBottomFace = new AcDbFace(AcGePoint3d(0, -0.5 * width, z), AcGePoint3d(length, -0.5 * width, z), 
		AcGePoint3d(length, 0.5 * width, z), AcGePoint3d(0, 0.5 * width, z));
	pBottomFace->transformBy(mat);
	pBottomFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pBottomFace);

	// 左侧面
	double y = 0.5 * width;
	AcDbFace *pLeftSideFace = new AcDbFace(AcGePoint3d(0, y, 0.5 * height), AcGePoint3d(length, y, 0.5 * height), 
		AcGePoint3d(length, y, -0.5 * height), AcGePoint3d(0, y, -0.5 * height));
	pLeftSideFace->transformBy(mat);
	pLeftSideFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pLeftSideFace);

	// 左侧面
	y = -0.5 * width;
	AcDbFace *pRightSideFace = new AcDbFace(AcGePoint3d(0, y, 0.5 * height), AcGePoint3d(length, y, 0.5 * height), 
		AcGePoint3d(length, y, -0.5 * height), AcGePoint3d(0, y, -0.5 * height));
	pRightSideFace->transformBy(mat);
	pRightSideFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pRightSideFace);
}

#pragma endregion
///////////////////////////////////////

class CZrxTemplate1App : public AcRxArxApp{
public:
	CZrxTemplate1App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: 如果有其他依赖项，请在这里加载

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: 初始化
		//初始化AcDbDoubleClickEdit类
		//AcDbDoubleClickEdit::rxInit();	//中望CAD不需要初始化
		//为AcDbCircle类添加协议扩展
		CDoubleClickCircle *pCircleEdit = new CDoubleClickCircle();
		AcDbCircle::desc()->addX(AcDbDoubleClickEdit::desc(), pCircleEdit);

		//自定义对象初始化
		DwgScale::rxInit();
		acrxBuildClassHierarchy();
		//自定义实体初始化
		ZfgkTriangle::rxInit();
		acrxBuildClassHierarchy();

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: 退出

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: 卸载依赖
		//删除AcDbCircle协议扩展
		AcDbCircle::desc()->delX(AcDbDoubleClickEdit::desc());

		//清除自定义对象
		deleteAcRxClass(DwgScale::desc());
		//清除自定义实体
		deleteAcRxClass(ZfgkTriangle::desc());


		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void zw_test()
	{
		acutPrintf(L"\ntest1\n");
		acutPrintf(L"\ntest2\n");
		acutPrintf(L"\ntest3\n");
		acutPrintf(L"\ntest4\n");
	}

	static void zwzw_CreateLine()
	{
		AcDbObjectId EntId;
		//创建直线的起点和终点
		AcGePoint3d ptStart(0, 0, 0);
		AcGePoint3d ptEnd(100, 100, 0);
		//创建直线
		EntId = CLineUtil::Add(ptStart, ptEnd);
		//修改直线颜色
		if (EntId != NULL)
		{
			acutPrintf(_T("\n创建直线成功！"));
			if (Acad::eOk == CEntityUtil::SetColor(EntId, 1))
			{
				acutPrintf(_T("\n修改直线颜色成功！"));
			}
			else
			{
				acutPrintf(_T("\n修改直线颜色失败！"));
			}

		}
		else
		{
			acutPrintf(_T("\n创建直线失败！"));
		}
	}
	static void zwzw_CreateCircle()
	{
		//"圆心、半径"法创建一个圆
		AcGePoint3d ptCenter(100, 100, 0);
		CCircleUtil::Add(ptCenter, 20);

		//两点法创建一个圆
		AcGePoint3d pt1(70, 100, 0);
		AcGePoint3d pt2(130, 100, 0);
		CCircleUtil::Add(pt1, pt2);

		//三点法创建一个圆
		AcGePoint2d pt01(60, 100);
		AcGePoint2d pt02(140, 100);
		AcGePoint2d pt03(100, 60);
		CCircleUtil::Add(pt01, pt02, pt03);
	}
	static void zwzw_CreateArc()
	{
		//创建位于XOY平面的圆弧
		AcGePoint2d ptCenter(50, 50);
		CArcUtil::Add(ptCenter, 100 * sqrt(2.0) / 2, 5 * CMathUtil::PI() / 4, 7 * CMathUtil::PI() / 4);

		//三点法创建圆弧
		AcGePoint2d ptStart(100, 0);
		AcGePoint2d ptOnArc(120, 50);
		AcGePoint2d ptEnd(100, 100);
		CArcUtil::Add(ptStart, ptOnArc, ptEnd);

		//起点、圆心、终点创建圆弧
		ptStart.set(100, 100);
		ptCenter.set(50, 50);
		ptEnd.set(0, 100);
		CArcUtil::AddSCE(ptStart, ptCenter, ptEnd);

		//起点 圆心 圆弧角度创建圆弧
		ptStart.set(0, 100);
		ptCenter.set(50, 50);
		CArcUtil::Add(ptStart, ptCenter, CMathUtil::PI() / 2);

	}
	static void zwzw_CreatePolyLine()
	{
		//创建包含一段直线的多段线
		AcGePoint2d ptStart(0, 0), ptEnd(100, 100);
		CPolylineUtil::Add(ptStart, ptEnd, 1);

		//创建三维多段线
		AcGePoint3d pt1(0, 0, 0), pt2(100, 0, 0), pt3(100, 100, 0);
		AcGePoint3dArray points;
		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		CPolylineUtil::Add3dPolyline(points);

		//创建正多边形
		CPolylineUtil::AddPolygon(AcGePoint2d::kOrigin, 6, 30, 0, 1);

		//创建矩形
		AcGePoint2d pt(60, 70);
		CPolylineUtil::AddPolyRectangle(pt, ptEnd, 1);

		//创建圆
		pt.set(50, 50);
		CPolylineUtil::AddPolyCircle(pt, 30, 1);

		//创建圆弧
		CPolylineUtil::AddPolyArc(pt, 50, CConvertUtil::AngleToRadian(45), CConvertUtil::AngleToRadian(255), 1);
	}
	static void zwzw_CreateEllipse()
	{
		//使用中点、所在平面、长轴矢量和短轴与长轴的比例创建椭圆
		AcGeVector3d vecNormal(0, 0, 1);
		AcGeVector3d majorAxis(40, 0, 0);
		AcDbObjectId entId;
		entId = CEllipseUtil::Add(AcGePoint3d::kOrigin, vecNormal, majorAxis, 0.5);

		//使用外接矩形角点来创建椭圆
		AcGePoint2d pt1(60, 80), pt2(140, 120);
		CEllipseUtil::Add(pt1, pt2);
	}
	static void zwzw_CreateSpline()
	{
		//使用样本点直接创建样条曲线
		AcGePoint3d pt1(0, 0, 0), pt2(10, 30, 0), pt3(60, 80, 0), pt4(100, 100, 0);
		AcGePoint3dArray points;
		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		points.append(pt4);
		CSplineUtil::Add(points);

		//指定起始点切线方向和终止点切线方向
		pt2.set(30, 10, 0);
		pt3.set(80, 60, 0);

		points.removeSubArray(0, 3);
		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		points.append(pt4);

		AcGeVector3d startTangent(5, 1, 0);
		AcGeVector3d endTangent(5, 1, 0);
		CSplineUtil::Add(points, startTangent, endTangent);
	}
	static void zwzw_CreateRegion()
	{
		//使用选择集，提示用户选择作为边界的对象
		ads_name ss;
		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);		//提示用户选择对象
		//根据选择集中的对象边界曲线ID数组
		AcDbObjectIdArray objIds;
		if (rt == RTNORM)
		{
			long length;
			acedSSLength(ss, &length);	//获取选择集中的对象个数
			for (int i = 0; i < length; i++)
			{
				ads_name ent;
				acedSSName(ss, i, ent);
				AcDbObjectId objId;
				acdbGetObjectId(objId, ent);

				objIds.append(objId);
			}
		}
		acedSSFree(ss);	//及时释放选择集

		AcDbObjectIdArray regionIds;
		regionIds = CRegionUtil::Add(objIds);

		int number = regionIds.length();
		acutPrintf(_T("\n已经创建 %d 个面域"), number);

	}
	static void zwzw_CreateText()
	{
		//创建单行文字
		AcGePoint3d ptInsert(0, 4, 0);
		CTextUtil::AddText(ptInsert, _T("武汉武大吉奥"));

		//创建多行文字
		ptInsert.set(0, 0, 0);
		CTextUtil::AddMText(ptInsert, _T("吉奥出品，必是精品！"));

	}
	static void zwzw_CreateHatch()
	{
		//提示用户选择填充边界
		ads_name ss;
		int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);
		AcDbObjectIdArray objIds;
		//初始化填充边界的ID数组
		if (rt == RTNORM)
		{
			long length;
			acedSSLength(ss, &length);
			for (int i = 0; i < length; i++)
			{
				ads_name ent;
				acedSSName(ss, i, ent);
				AcDbObjectId objId;
				acdbGetObjectId(objId, ent);

				objIds.append(objId);
			}
		}
		acedSSFree(ss);
		CHatchUtil::Add(objIds, _T("SOLID"), 1.0);
	}
	static void zwzw_CreateDimension()
	{
		//指定起始点位置
		AcGePoint3d pt1(200, 160, 0);
		AcGePoint3d pt2 = CGePointUtil::RelativePoint(pt1, -40, 0);
		AcGePoint3d pt3 = CGePointUtil::PolarPoint(pt2, 7 * CMathUtil::PI() / 6, 20);
		AcGePoint3d pt4 = CGePointUtil::RelativePoint(pt3, 6, -10);
		AcGePoint3d pt5 = CGePointUtil::RelativePoint(pt1, 0, -20);

		//绘制外轮廓线
		CLineUtil::Add(pt1, pt2);
		CLineUtil::Add(pt2, pt3);
		CLineUtil::Add(pt3, pt4);
		CLineUtil::Add(pt4, pt5);
		CLineUtil::Add(pt5, pt1);

		//绘制圆形
		AcGePoint3d ptCenter1, ptCenter2;
		ptCenter1 = CGePointUtil::RelativePoint(pt3, 16, 0);
		ptCenter2 = CGePointUtil::RelativePoint(ptCenter1, 25, 0);
		CCircleUtil::Add(ptCenter1, 3);
		CCircleUtil::Add(ptCenter2, 4);

		AcGePoint3d ptTemp1, ptTemp2;
		//水平标注
		ptTemp1 = CGePointUtil::RelativePoint(pt1, -20, 3);
		CDimensionUtil::AddDimRotated(pt1, pt2, ptTemp1, 0);

		//垂直标注
		ptTemp1 = CGePointUtil::RelativePoint(pt1, 4, 0);
		CDimensionUtil::AddDimRotated(pt1, pt5, ptTemp1, CMathUtil::PI() / 2);

		//转角标注
		ptTemp1 = CGePointUtil::RelativePoint(pt3, -3, -6);
		CDimensionUtil::AddDimRotated(pt3, pt4, ptTemp1, 7 * CMathUtil::PI() / 4);

		//对齐标志
		ptTemp1 = CGePointUtil::RelativePoint(pt2, -3, 4);
		CDimensionUtil::AddDimAligned(pt2, pt3, ptTemp1, AcGeVector3d(4, 10, 0), _T("new position"));

		//角度标注
		ptTemp1 = CGePointUtil::RelativePoint(pt5, -5, 5);
		CDimensionUtil::AddDim3PtAngular(pt5, pt1, pt4, ptTemp1);

		//半径标注
		ptTemp1 = CGePointUtil::PolarPoint(ptCenter1, CMathUtil::PI() / 4, 3);
		CDimensionUtil::AddDimRadial(ptCenter1, ptTemp1, -3);

		//直径标注
		ptTemp1 = CGePointUtil::RelativePoint(ptCenter2, CMathUtil::PI() / 4, 4);
		ptTemp2 = CGePointUtil::RelativePoint(ptCenter2, CMathUtil::PI() / 4, -4);
		CDimensionUtil::AddDimDiameteric(ptTemp1, ptTemp2, 0);

		//坐标标注
		CDimensionUtil::AddDimOrdinate(ptCenter2, AcGeVector3d(0, -10, 0), AcGeVector3d(10, 0, 0));
	}
	static void zwzw_GetAllEntByLayer()
	{
		AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds(_T("test"));
		for (int i = 0; i < allEntIds.length(); i++)
		{
			AcDbLine *pLine = NULL;
			if (acdbOpenObject(pLine, allEntIds[i], AcDb::kForWrite) == Acad::eOk)
			{
				pLine->setColorIndex(1);
				pLine->close();
			}
		}
	}
	static void zwzw_CreateBlock()
	{
		//获得当前图形数据库
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);

		//创建新的块表记录
		AcDbBlockTableRecord *pBlkTblRcd = new AcDbBlockTableRecord();

		//根据用户的输入设置块表记录的名称
		TCHAR blkName[40];
		if (acedGetString(Adesk::kFalse, _T("\n输入图块的名称："), blkName) != RTNORM)
		{
			pBlkTbl->close();
			delete pBlkTblRcd;
			return;
		}

		pBlkTblRcd->setName(blkName);

		//将块表记录添加到块表中
		AcDbObjectId blbDefId;
		pBlkTbl->add(blbDefId, pBlkTblRcd);
		pBlkTbl->close();

		//向块表中添加实体
		AcGePoint3d ptStart(-10, 0, 0), ptEnt(10, 0, 0);
		AcDbLine *pLine1 = new AcDbLine(ptStart, ptEnt);	//创建一条直线
		ptStart.set(0, -10, 0);
		ptEnt.set(0, 10, 0);
		AcDbLine *pLine2 = new AcDbLine(ptStart, ptEnt);	//创建一条直线
		AcGeVector3d vecNormal(0, 0, 1);
		AcDbCircle *pCircle = new AcDbCircle(AcGePoint3d::kOrigin, vecNormal, 6);

		AcDbObjectId entId;
		pBlkTblRcd->appendAcDbEntity(entId, pLine1);
		pBlkTblRcd->appendAcDbEntity(entId, pLine2);
		pBlkTblRcd->appendAcDbEntity(entId, pCircle);

		//关闭块表记录
		pLine1->close();
		pLine2->close();
		pCircle->close();
		pBlkTblRcd->close();

	}
	static void zwzw_InsertBlock()
	{
		//获取用户输入的块定义名称
		TCHAR blkName[40];
		if (acedGetString(Adesk::kFalse, _T("\n输入图块的名称:"), blkName) != RTNORM)
		{
			return;
		}
		//获取当前数据库的块表
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);
		//查找用户指定的块定义是否存在
		CString strBlkDef;
		strBlkDef.Format(_T("%s"), blkName);
		if (!pBlkTbl->has(strBlkDef))
		{
			acutPrintf(_T("\n当前图形中未包含指定名称的块定义！"));
			pBlkTbl->close();
			return;
		}
		//获得用户输入的块参照的插入点
		ads_point pt;
		if (acedGetPoint(NULL, _T("\n输入块参照的插入点:"), pt) != RTNORM)
		{
			pBlkTbl->close();
			return;
		}
		AcGePoint3d ptInsert = asPnt3d(pt);

		//获取用户指定的块表记录
		AcDbObjectId blkDefId;
		pBlkTbl->getAt(strBlkDef, blkDefId);
		pBlkTbl->close();

		//创建参照对象
		AcDbBlockReference *pBlkRef = new AcDbBlockReference(ptInsert, blkDefId);

		//将块参照添加到模型空间
		CDwgDatabaseUtil::PostToModelSpace(pBlkRef);
	}
	static void zwzw_InsertBlock2()
	{
		//获取用户指定的块定义是否存在
		TCHAR blkName[40];
		if (acedGetString(Adesk::kFalse, _T("\n输入图块的名称:"), blkName) != RTNORM)
		{
			return;
		}
		AcDbObjectId blkDefId = CBlockUtil::GetBlkDefId(blkName,acdbHostApplicationServices()->workingDatabase());
		if (blkDefId.isNull())
		{
			acutPrintf(_T("\n当前图形中未包含指定名称的块定义"));
			return;
		}

		//获取用户输入的块参照的插入点
		ads_point pt;
		if (acedGetPoint(NULL, _T("\n输入块参照的插入点"), pt) != RTNORM)
		{
			return;
		}
		AcGePoint3d ptInsert = asPnt3d(pt);

		//创建块参照对象
		AcDbBlockReference *pBlkRef = new AcDbBlockReference(ptInsert, blkDefId);

		//将块参照添加到模型空间
		CDwgDatabaseUtil::PostToModelSpace(pBlkRef);
	}
	static void zwzw_InsertBlock3()
	{
		//获取用户输入的块定义名称
		TCHAR blkName[40];
		if (acedGetString(Adesk::kFalse, _T("\n输入图块名称:"), blkName) != RTNORM)
		{
			return;
		}
		//查找用户指定的块定义是否存在
		AcDbObjectId blkDefId = CBlockUtil::GetBlkDefId(blkName,acdbHostApplicationServices()->workingDatabase());
		if (blkDefId.isNull())
		{
			acutPrintf(_T("\n当前图形中未包含指定名称的块定义！"));
			return;
		}
		ads_point pt;
		if (acedGetPoint(NULL, _T("\n输入块参照的插入点:"), pt) != RTNORM)
		{
			return;
		}
		AcGePoint3d ptInsert = asPnt3d(pt);

		//插入块参照（属性值使用默认值）
		AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blkDefId, ptInsert, 1, 0);

		//设置参数值
		AcDbBlockReference *pBlkRef = NULL;
		if (acdbOpenObject(pBlkRef, blkRefId, AcDb::kForWrite) == Acad::eOk)
		{
			CBlockUtil::SetBlockRefAttribute(pBlkRef, _T("名称"), _T("矩形板"));
			CBlockUtil::SetBlockRefAttribute(pBlkRef, _T("重量"), _T("2.3Kg"));
			pBlkRef->close();
		}
	}
	static void zwzw_BlockIntersectWith()
	{
		//获取图形数据库中需要处理的块参照
		AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
		AcDbObjectId blkRefId;
		for (int i = 0; i < allEntIds.length(); i++)
		{
			AcDbBlockReference *pBlkRef = NULL;
			if (acdbOpenObject(pBlkRef, allEntIds[i], AcDb::kForRead) == Acad::eOk)
			{
				blkRefId = pBlkRef->objectId();
				pBlkRef->close();
			}
		}
		//块参照和直线计算真实交点
		AcDbBlockReference *pBlkRef = NULL;
		if (acdbOpenObject(pBlkRef, blkRefId, AcDb::kForRead) == Acad::eOk)
		{
			for (int i = 0; i < allEntIds.length(); i++)
			{
				if (allEntIds[i] != blkRefId)
				{
					AcDbLine *pLine = NULL;
					if (acdbOpenObject(pLine, allEntIds[i], AcDb::kForRead) == Acad::eOk)
					{
						AcGePoint3dArray intPoints;
						CBlockUtil::IntersectWith(pBlkRef, pLine, intPoints, false, false, false);
						//将交点用红色的圆绘制出来
						for (int j = 0; j < intPoints.length(); j++)
						{
							double radius = 4;
							AcDbObjectId circleId = CCircleUtil::Add(intPoints[j], radius);
							CEntityUtil::SetColor(circleId, 1);
						}
						pLine->close();
					}
				}
			}
			pBlkRef->close();
		}

	}
	static void zwzw_NewLayer()
	{
		//提示用户输入图层名称
		TCHAR layerName[100];
		if (acedGetString(Adesk::kFalse, _T("\n输入图层的名称:"), layerName) != RTNORM)
		{
			return;
		}
		//新建图层
		AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
		if (layerId.isValid())
		{
			acutPrintf(_T("\n图形中已存在同名的图层."));
		}
		else
		{
			CLayerUtil::Add(layerName);
			acutPrintf(_T("\n成功创建图层."));
		}
	}
	static void zwzw_LayerColor()
	{
		//提示用户输入图层名称
		TCHAR layerName[100];
		if (acedGetString(Adesk::kFalse, _T("\n输入图层的名称:"), layerName) != RTNORM)
		{
			return;
		}
		//新建图层
		AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
		if (layerId.isValid())
		{
			//获取图层当前颜色
			AcCmColor oldColor;
			AcDbLayerTableRecord *pLayerTblRcd = NULL;
			if (acdbOpenObject(pLayerTblRcd, layerId, AcDb::kForRead) == Acad::eOk)
			{
				oldColor = pLayerTblRcd->color();
				pLayerTblRcd->close();
			}
			int nCurColor = oldColor.colorIndex();	//图层修改前的颜色
			int nNewColor = oldColor.colorIndex();	//用户选择的颜色

			//弹出“颜色”对话框
			if (acedSetColorDialog(nNewColor, Adesk::kFalse, nCurColor))
			{
				CLayerUtil::SetColor(layerName, nNewColor);
			}
		}
		else
		{
			acutPrintf(_T("\n图形中不存在指定名称图层."));
		}
	}
	static void zwzw_DeleteLayer()
	{
		//提示用户输入图层名称
		TCHAR layerName[100];
		if (acedGetString(Adesk::kFalse, _T("\n输入图层的名称:"), layerName) != RTNORM)
		{
			return;
		}
		//新建图层
		AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
		if (layerId.isValid())
		{
			//删除图层
			AcDbLayerTableRecord *pLayerTblRcd = NULL;
			if (acdbOpenObject(pLayerTblRcd, layerId, AcDb::kForWrite) == Acad::eOk)
			{
				pLayerTblRcd->erase();
				pLayerTblRcd->close();
				acutPrintf(_T("\n删除图层：%s 成功."), layerName);
			}
		}
		else
		{
			acutPrintf(_T("\n图形中不存在指定名称图层."));
		}
	}
	static void zwzw_ExportLayer()
	{
		//获取图层列表
		AcDbObjectIdArray layerIds;
		CLayerUtil::GetLayerList(layerIds);

		//遍历图层列表，构建需要写入到文本文件的字符串集合
		std::vector<CString> lines;
		for (int i = 0; i < layerIds.length(); i++)
		{
			AcDbLayerTableRecord *pLayerTblRcd = NULL;
			if (acdbOpenObject(pLayerTblRcd, layerIds[i], AcDb::kForRead) == Acad::eOk)
			{
				//输出图层的信息
				std::vector<CString> layerInfos;
				TCHAR *szLayerName;	//图层名称
				pLayerTblRcd->getName(szLayerName);
				layerInfos.push_back(szLayerName);
				acutDelString(szLayerName);

				AcCmColor color = pLayerTblRcd->color();	//图层颜色
				layerInfos.push_back(CConvertUtil::ToString(color.colorIndex()));

				AcDbLinetypeTableRecord *pLinetypeTblRcd = NULL;	//图层线型
				acdbOpenObject(pLinetypeTblRcd, pLayerTblRcd->linetypeObjectId(), AcDb::kForRead);
				TCHAR *szLinetypeName;
				pLinetypeTblRcd->getName(szLinetypeName);
				layerInfos.push_back(szLinetypeName);
				acutDelString(szLinetypeName);

				AcDb::LineWeight lineWeight = pLayerTblRcd->lineWeight();	//图层线宽
				int nVal = (int)lineWeight;		//枚举转换为整型
				layerInfos.push_back(CConvertUtil::ToString(nVal));

				CString strLayerInfo = CStringUtil::Jion(layerInfos, _T(","));	//要输出图层汇总信息

				lines.push_back(strLayerInfo);

				pLayerTblRcd->close();
			}
		}

		//写入文本文件
		CString fileNmae = _T("D:\\layers.txt");
		CTextFileUtil::Save(fileNmae, lines);

	}
	static void zwzw_ImportLayer()
	{
		//读取文本中的内容
		CString fileName = _T("D:\\layers.txt");
		if (_taccess(fileName, 0) != -1)
		{
			std::vector<CString> lines;
			CTextFileUtil::Load(fileName, lines);

			//遍历每一个字符串，处理当前图形中的图层
			for (int i = 0; i < lines.size(); i++)
			{
				std::vector<CString> layerInfos;
				CStringUtil::Split(lines[i], _T(","), layerInfos, false);
				if (layerInfos.size() == 4)
				{
					CString layerName = layerInfos[0];
					AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
					if (layerId.isNull())
					{
						CLayerUtil::Add(layerName);
						layerId = CLayerUtil::GetLayerId(layerName);
					}

					AcDbLayerTableRecord *pLayerTblRcd = NULL;
					if (acdbOpenObject(pLayerTblRcd, layerId, AcDb::kForWrite) == Acad::eOk)
					{
						//设置颜色
						AcCmColor color;
						Adesk::UInt16 colorIndex = _ttoi(layerInfos[1]);
						color.setColorIndex(colorIndex);
						pLayerTblRcd->setColor(color);

						//设置线型
						AcDbLinetypeTable *pLinetypeTbl = NULL;
						AcDbObjectId linetypeId;
						acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);
						if (pLinetypeTbl->has(layerInfos[2]))
						{
							pLinetypeTbl->getAt(layerInfos[2], linetypeId);
							pLayerTblRcd->setLinetypeObjectId(linetypeId);
						}
						pLinetypeTbl->close();

						//设置线宽
						AcDb::LineWeight lineWeight = (AcDb::LineWeight)_ttoi(layerInfos[3]);

						pLayerTblRcd->setLineWeight(lineWeight);

						pLayerTblRcd->close();
					}
				}
			}
		}
		else
		{
			acutPrintf(_T("\n未找到指定的文件."));
		}
	}
	static void zwzw_AddTextStyle()
	{
		CString textStyleName = _T("测试字体");
		AcDbObjectId textStyleId = CTextStyleUtil::GetAt(textStyleName);
		if (textStyleId.isNull())
		{
			CTextStyleUtil::Add(textStyleName, _T("txt.shx"), _T("hztxt.shx"));
		}

	}
	static void zwzw_ZoomWindow()
	{
		//提示用户选择定义缩放窗口的两个角点
		ads_point pt1, pt2;
		if (acedGetPoint(NULL, _T("\n输入第一个角点:"), pt1) != RTNORM)
			return;
		if (acedGetCorner(pt1, _T("\n输入第二个角点:"), pt2) != RTNORM)
			return;

		//将两个角点的坐标转换到世界坐标
		AcGePoint3d pt1Wcs = CConvertUtil::WcsToDcsPoint(asPnt3d(pt1));
		AcGePoint3d pt2Wcs = CConvertUtil::WcsToDcsPoint(asPnt3d(pt2));

		CViewUtil::Set(pt1Wcs, pt2Wcs, 1.0);
	}
	static void zwzw_ZoomExtents()
	{
		CViewUtil::ZoomExtent();
	}
	static void zwzw_ZoomEntity()
	{
		//寻找图形中的抗原性的包围面
		AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
		bool bFound = false;
		AcDbExtents ext;
		for (int i = 0; i < allEntIds.length(); i++)
		{
			AcDbCircle *pCircle = NULL;
			if (acdbOpenObject(pCircle, allEntIds[i], AcDb::kForRead) == Acad::eOk)
			{
				pCircle->getGeomExtents(ext);
				bFound = true;
				pCircle->close();
			}
		}

		//将图形中的圆形显示在图形窗口中心
		if (bFound)
		{
			CViewUtil::Set(ext.minPoint(), ext.maxPoint(), 5.0);
		}
	}
	static void zwzw_Create4VPotrs()
	{
		//获取视口表
		AcDbViewportTable *pVPortTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getViewportTable(pVPortTbl, AcDb::kForWrite);

		//分别创建四个视口
		AcGePoint2d pt1, pt2;

		AcDbViewportTableRecord *pVPortTblRcd1 = new AcDbViewportTableRecord();
		pt1.set(0, 0);
		pt2.set(0.5, 0.5);
		pVPortTblRcd1->setLowerLeftCorner(pt1);
		pVPortTblRcd1->setUpperRightCorner(pt2);
		pVPortTblRcd1->setName(_T("4VPorts"));

		AcDbViewportTableRecord *pVPortTblRcd2 = new AcDbViewportTableRecord();
		pt1.set(0.5, 0);
		pt2.set(1.0, 0.5);
		pVPortTblRcd2->setLowerLeftCorner(pt1);
		pVPortTblRcd2->setUpperRightCorner(pt2);
		pVPortTblRcd2->setName(_T("4VPorts"));

		AcDbViewportTableRecord *pVPortTblRcd3 = new AcDbViewportTableRecord();
		pt1.set(0, 0.5);
		pt2.set(0.5, 1);
		pVPortTblRcd3->setLowerLeftCorner(pt1);
		pVPortTblRcd3->setUpperRightCorner(pt2);
		pVPortTblRcd3->setName(_T("4VPorts"));

		AcDbViewportTableRecord *pVPortTblRcd4 = new AcDbViewportTableRecord();
		pt1.set(0.5, 0.5);
		pt2.set(1.0, 1.0);
		pVPortTblRcd4->setLowerLeftCorner(pt1);
		pVPortTblRcd4->setUpperRightCorner(pt2);
		pVPortTblRcd4->setName(_T("4VPorts"));

		pVPortTbl->add(pVPortTblRcd1);
		pVPortTbl->add(pVPortTblRcd2);
		pVPortTbl->add(pVPortTblRcd3);
		pVPortTbl->add(pVPortTblRcd4);

		pVPortTbl->close();
		pVPortTblRcd1->close();
		pVPortTblRcd2->close();
		pVPortTblRcd3->close();
		pVPortTblRcd4->close();

		//判断当前的空间
		struct resbuf rb;
		acedGetVar(_T("TILEMODE"), &rb);
		if (rb.resval.rint == 1)	//当前空间是模型空间
		{
			acedCommand(RTSTR, _T(".-VPORTS"), RTSTR, _T("R"), RTSTR, _T("4VPorts"), RTNONE);
		}
		else  //当前空间是图纸空间
		{
			acedCommand(RTSTR, _T(".-VPORTS"), RTSTR, _T("R"), RTSTR, _T("4VPorts"), RTSTR, _T(""), RTNONE);
		}
	}
	static void zwzw_CreateVPotrInSpace()
	{
		//指定当前布局
		Acad::ErrorStatus es = acdbHostApplicationServices()->layoutManager()->setCurrentLayout(_T("布局1"));

		if (es != Acad::eOk)
		{
			return;
		}

		//获得表
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

		//获得图纸空空间的块表记录
		AcDbBlockTableRecord *pBlkTblRcd = NULL;
		pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd, AcDb::kForWrite);
		pBlkTbl->close();

		//创建新的布局对象
		AcDbViewport *pViewport = new AcDbViewport();
		pViewport->setCenterPoint(AcGePoint3d(100, 50, 0));
		pViewport->setHeight(80);
		pViewport->setWidth(120);

		//将新的布局对象添加到图纸空间块表记录中
		AcDbObjectId viewportId;
		pBlkTblRcd->appendAcDbEntity(viewportId, pViewport);

		pViewport->close();
		pBlkTblRcd->close();

		//将新建的视口作为当前视口
		AcDbViewport *pVP = NULL;
		if (acdbOpenObject(pVP, viewportId, AcDb::kForWrite) == Acad::eOk)
		{
			pViewport->setOn();
			acedSetCurrentVPort(pVP);
			pVP->close();
		}
	}
	static void zwzw_NewUCS()
	{
		//获得当前的UCS表
		AcDbUCSTable *pUcsTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getUCSTable(pUcsTbl, AcDb::kForWrite);

		//定义UCS的参数
		AcGePoint3d ptOrigin(0, 0, 0);
		AcGeVector3d vecXAxis(1, 1, 0);
		AcGeVector3d vecYAxis(-1, 1, 0);

		//创建新的UCS表记录
		AcDbUCSTableRecord *pUCSTblRcd = new AcDbUCSTableRecord();

		//设置UCS的参数
		Acad::ErrorStatus es = pUCSTblRcd->setName(_T("NewUcs"));
		if (es != Acad::eOk)
		{
			delete pUCSTblRcd;
			pUcsTbl->close();
			return;
		}

		pUCSTblRcd->setOrigin(ptOrigin);
		pUCSTblRcd->setXAxis(vecXAxis);
		pUCSTblRcd->setYAxis(vecYAxis);

		//将创建的UCS表记录添加到UCS表中
		es = pUcsTbl->add(pUCSTblRcd);
		if (es != Acad::eOk)
		{
			delete pUCSTblRcd;
			pUcsTbl->close();
			return;
		}

		//关闭对象
		pUCSTblRcd->close();
		pUcsTbl->close();
	}
	static void zwzw_SetCurUcs()
	{
		//提示用户输入UCS的名称
		TCHAR ucsName[40];
		if (acedGetString(NULL, _T("\n输入用户坐标系的名称:"), ucsName) != RTNORM)
			return;

		//获取指定的UCS表记录
		AcDbUCSTable *pUcsTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getUCSTable(pUcsTbl, AcDb::kForRead);
		if (!pUcsTbl->has(ucsName))
		{
			pUcsTbl->close();
			return;
		}
		AcDbUCSTableRecord *pUcsTblRcd;
		pUcsTbl->getAt(ucsName, pUcsTblRcd, AcDb::kForRead);

		//获得UCS的变换矩阵
		AcGeMatrix3d mat;
		AcGeVector3d vecXAxis, vecYAxis, vecZAxis;
		vecXAxis = pUcsTblRcd->xAxis();
		vecYAxis = pUcsTblRcd->yAxis();
		vecZAxis = vecXAxis.crossProduct(vecYAxis);
		mat.setCoordSystem(pUcsTblRcd->origin(), vecXAxis, vecYAxis, vecZAxis);

		//关闭UCS表和UCS表记录
		pUcsTblRcd->close();
		pUcsTbl->close();

		//设置当前UCS
		acedSetCurrentUCS(mat);

	}
	static void zwzw_MoveUcsOrigin()
	{
		//获得当前UCS的变换矩阵
		AcGeMatrix3d mat;
		Acad::ErrorStatus es = acedGetCurrentUCS(mat);

		//根据变换矩阵获得UCS参数
		AcGePoint3d ptOrigin;
		AcGeVector3d vecXAxis, vecYAxis, vecZAxis;
		mat.getCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);

		//移动UCS的原点
		AcGeVector3d vec(100, 100, 0);
		ptOrigin += vec;

		//更新变换矩阵
		mat.setCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);

		//应用更新的UCS
		acedSetCurrentUCS(mat);
	}
	static void zwzw_RotateUcs()
	{
		//获得当前UCS的变换矩阵
		AcGeMatrix3d mat;
		Acad::ErrorStatus es = acedGetCurrentUCS(mat);

		//根据变换矩阵获得UCS参数
		AcGePoint3d ptOrigin;
		AcGeVector3d vecXAxis, vecYAxis, vecZAxis;
		mat.getCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);

		//绕Z轴旋转60°
		vecXAxis.rotateBy(60 * atan(1.0) * 4 / 180, vecZAxis);
		vecYAxis.rotateBy(60 * atan(1.0) * 4 / 180, vecZAxis);

		//更新变换矩阵
		mat.setCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);

		//应用跟新的UCS
		acedSetCurrentUCS(mat);
	}
	static void zwzw_AddEntInUcs()
	{
		//提示用户输入直线的起点和终点
		ads_point pt1, pt2;
		if (acedGetPoint(NULL, _T("\n拾取直线的起点："), pt1) != RTNORM)
			return;
		if (acedGetPoint(NULL, _T("\n拾取直线的终点："), pt2) != RTNORM)
			return;

		//按照UCS坐标创建实体
		AcDbLine *pLIne = new AcDbLine(asPnt3d(pt1), asPnt3d(pt2));

		//对实体进行几何转换
		AcGeMatrix3d mat;
		Acad::ErrorStatus es = acedGetCurrentUCS(mat);
		pLIne->transformBy(mat);

		//加入到模型空间
		CDwgDatabaseUtil::PostToModelSpace(pLIne);
	}
	static void zwzw_AddCircle1()
	{
		///声明ADS变量
		ads_point ptCenter = { 0, 0, 0 };	//圆形
		ads_real radius = 10;	//半径

		//调用acedCommand函数创建圆
		acedCommand(RTSTR, _T("Circle"), RTPOINT, ptCenter, RTREAL, radius, RTNONE);

	}
	static void zwzw_AddCircle2()
	{
		struct resbuf *rb;	//结果缓冲区
		int rc = RTNORM;	//返回值

		//创建结果缓冲区链表
		ads_point ptCenter = { 30,0,0 };
		ads_real radius = 10;
		rb = acutBuildList(RTSTR, _T("Circle"), RTPOINT, ptCenter, RTREAL, radius, RTNONE);

		//创建圆
		if (rb != NULL)
		{
			rc = acedCmd(rb);
		}

		//检验返回值
		if (rc != RTNORM)
		{
			acutPrintf(_T("\n创建圆失败！"));
		}

		acutRelRb(rb);

		//进行缩放
		acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("E"), RTNONE);

	}
	static void zwzw_EntInfo()
	{
		//提示用户选择实体
		ads_name entName;
		ads_point pt;
		if (acedEntSel(_T("\n选择实体:"), entName, pt) != RTNORM)
		{
			return;
		}

		struct resbuf *rbEnt;	//保存实体数据的结果缓冲区
		struct resbuf *rb;		//用于遍历reEnt的结果缓冲区

		//从entName获得保存实体数据的结果缓冲区
		rbEnt = acdbEntGet(entName);
		rb = rbEnt;

		while (rb != NULL)
		{
			switch (rb->restype)
			{
			case -1:
				acutPrintf(_T("\n图元名：%x"), rb->resval.rstring);
				break;
			case 0:
				acutPrintf(_T("\n图元类型：%s"), rb->resval.rstring);
				break;
			case 8:
				acutPrintf(_T("\n图层：%s"), rb->resval.rstring);
				break;
			case 10:
				acutPrintf(_T("\n圆心：%.2f,%.2f,%.2f"), rb->resval.rpoint[X], rb->resval.rpoint[Y], rb->resval.rpoint[Z]);
				break;
			case 40:
				acutPrintf(_T("\n半径：%.4f"), rb->resval.rreal);
				break;
			case 210:
				acutPrintf(_T("\n平面的法向量：（%.2f,%.2f,%.2f）"), rb->resval.rpoint[X], rb->resval.rpoint[Y], rb->resval.rpoint[Z]);
				break;
			default:
				break;
			}
			rb = rb->rbnext;
		}
		if (rbEnt != NULL)
		{
			acutRelRb(rbEnt);
		}

	}
	static void zwzw_AddPolyBasic()
	{
		int index = 2;//当前输入点的次数
		AcGePoint3d ptStart;	//起点
		if (!CGetInputUtil::GetPoint(_T("\n输入第一个点:"), ptStart))
		{
			return;
		}
		AcGePoint3d ptPrevious, ptCurrent;	//前一个参考点，当前拾取的点
		ptPrevious = ptStart;
		AcDbObjectId polyId;	//多段线ID
		while (CGetInputUtil::GetPoint(ptPrevious, _T("\n输入下一点:"), ptCurrent))
		{
			if (index == 2)
			{
				polyId = CPolylineUtil::Add(CConvertUtil::ToPoint2d(ptPrevious), CConvertUtil::ToPoint2d(ptCurrent));
			}
			else if (index > 2)
			{
				AcDbPolyline *pPoly = NULL;
				if (acdbOpenObject(pPoly, polyId, AcDb::kForWrite) == Acad::eOk)
				{
					pPoly->addVertexAt(index - 1, CConvertUtil::ToPoint2d(ptCurrent));
					pPoly->close();
				}
			}
			ptPrevious = ptCurrent;
			index++;
		}
	}
	static void zwzw_GetPointKeyword()
	{
		int rc;		//返回值
		TCHAR Kword[20];	//关键字
		AcGePoint3d pt;
		acedInitGet(RSG_NONULL, _T("K W"));
		rc = CGetInputUtil::GetPointReturnCode(_T("\n输入第一个点或[Keyword1/KeyWord2]:"), pt);
		switch (rc)
		{
		case RTKWORD:	//输入了关键字
			if (acedGetInput(Kword) != RTNORM)
			{
				return;
			}
			if (_tcscmp(Kword, _T("K")) == 0)
			{
				acutPrintf(_T("\n选择的关键字是Keyword1"));
			}
			else if (_tcscmp(Kword, _T("W")) == 0)
			{
				acutPrintf(_T("\n选择的关键字是Keyword2"));
			}
			break;
		case RTNORM:
			acutPrintf(_T("\n输入点的坐标是（%.2f,%.2f,%.2f）"), pt.x, pt.y, pt.z);
		default:
			break;
		}

	}
	static void zwzw_AddPoly()
	{
		int colorIndex = 0;		//颜色索引值
		ads_real width = 0;		//多段线的线宽
		int index = 2;		//当前输入点的次数

		//提示用户输入起点
		AcGePoint3d ptStart;		//起点
		if (!CGetInputUtil::GetPoint(_T("\n输入第一点:"), ptStart))
		{
			return;
		}
		AcGePoint3d ptPrevious, ptCurrent;	//前一个参考点，当前拾取点
		ptPrevious = ptStart;
		AcDbObjectId polyId;		//多段线的ID

		//输入第二点
		acedInitGet(NULL, _T("W C O"));
		int rc = CGetInputUtil::GetPointReturnCode(ptPrevious, _T("\n输入下一点[线宽(W)/颜色(C)/完成(O)]"), ptCurrent);
		while (rc == RTNORM || rc == RTKWORD)
		{
			if (rc == RTKWORD)
			{
				TCHAR Kword[20];
				if (acedGetInput(Kword) != RTNORM)
					return;
				if (_tcscmp(Kword, _T("W")) == 0)
				{
					width = GetWidth();
				}
				else if (_tcscmp(Kword, _T("C")) == 0)
				{
					colorIndex = GetColorIndex();
				}
				else if (_tcscmp(Kword, _T("O")) == 0)
				{
					return;
				}
				else
				{
					acutPrintf(_T("\n无效的关键字."));
				}
			}
			else if (rc == RTNORM)	//用户输入了点
			{
				if (index == 2)
				{
					//创建多段线
					polyId = CPolylineUtil::Add(CConvertUtil::ToPoint2d(ptPrevious), CConvertUtil::ToPoint2d(ptCurrent));
					AcDbPolyline *pPoly = NULL;
					if (acdbOpenObject(pPoly, polyId, AcDb::kForWrite) == Acad::eOk)
					{
						//修改多段线的颜色和线宽
						pPoly->setConstantWidth(width);
						pPoly->setColorIndex(colorIndex);
						pPoly->close();
					}
				}
				else if (index > 2)
				{
					//修改多段线，添加最后一个顶点
					AcDbPolyline *pPoly = NULL;
					if (acdbOpenObject(pPoly, polyId, AcDb::kForWrite) == Acad::eOk)
					{
						pPoly->addVertexAt(index - 1, CConvertUtil::ToPoint2d(ptCurrent));
						//修改多段线的颜色和线宽
						pPoly->setConstantWidth(width);
						pPoly->setColorIndex(colorIndex);

						pPoly->close();
					}
				}
				ptPrevious = ptCurrent;
				index++;

			}
			//提示用户输入新的节点
			acedInitGet(NULL, _T("W C O"));
			rc = CGetInputUtil::GetPointReturnCode(ptPrevious, _T("\n输入下一点[线宽(W)/颜色(C)/完成(O)]"), ptCurrent);
		}
	}
	static void zwzw_SelectFile()
	{
		CString title = _T("选择图形文件");
		CString path = _T("C:\\");
		struct resbuf *fileName = acutNewRb(RTSTR);
		if (acedGetFileD(title, path, _T("dwg;dxf"), 0, fileName) == RTNORM)
		{
			acedAlert(fileName->resval.rstring);
		}
		acutRelRb(fileName);
	}
	static void zwzw_CreateSSet()
	{
		ads_name sset;	//选择集名字
		//选择图形数据库中所有的实体
		acedSSGet(_T("A"), NULL, NULL, NULL, sset);
		long length;
		acedSSLength(sset, &length);
		acutPrintf(_T("\n实体数:%d"), length);

		acedSSFree(sset);
	}
	static void zwzw_SelectLast()
	{
		ads_name sset;	//选择集名字
		//选择最近一次创建的选择集
		acedSSGet(_T("P"), NULL, NULL, NULL, sset);
		long length;
		acedSSLength(sset, &length);
		acutPrintf(_T("\n实体数:%d"), length);

		acedSSFree(sset);

	}
	static void zwzw_SelectEnt()
	{
		ads_point pt1, pt2, pt3, pt4;
		struct resbuf *pointlist;	//结果缓冲区链表
		ads_name ssname;			//选择集的图元名
		pt1[X] = pt1[Y] = pt1[Z] = 0.0;
		pt2[X] = pt2[Y] = 5.0; pt2[Z] = 0.0;
		long length;


		//如果已选择到了实体，就获取当前的PICKFIRST选择集，否则提示用户选择实体
		//acedSSGet(NULL, NULL, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n（NULL）选择集中实体数:%d"), length);

		//如果存在，就获取当前的PickFirst选择集
		//acedSSGet(_T("I"), NULL, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n（I）选择集中实体数:%d"), length);

		//选择最近一次创建的选择集
		//acedSSGet(_T("P"), NULL, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n（P）选择集中实体数:%d"), length);

		//选择最后一次创建的可见实体
		//acedSSGet(_T("L"), NULL, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n（L）选择集中实体数:%d"), length);

		//通过点（5，5）的所有实体
		//acedSSGet(NULL, pt2, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n通过点（5，5）实体数:%d"), length);

		//选择位于角点（0，0）和（5，5）组成的窗口所有实体
		//acedSSGet(_T("W"), pt1, pt2, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n位于角点（0，0）和（5，5）组成的窗口所有实体数:%d"), length);

		//选择被指定的多边形包围的所有实体
		//pt3[X] = 10.0; pt3[Y] = 5.0; pt3[Z] = 0.0;
		//pt4[X] = 5.0; pt4[Y] = pt4[Z] = 0.0;
		//pointlist = acutBuildList(RTPOINT, pt1, RTPOINT, pt2, RTPOINT, pt3, RTPOINT, pt4, 0);
		//acedSSGet(_T("WP"), pointlist, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n被指定的多边形包围的所有实体数:%d"), length);

		//选择与角点（0，0）和（5，5）组成的区域相交的所有实体
		//acedSSGet(_T("C"), pt1, pt2, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n与角点（0，0）和（5，5）组成的区域相交的所有实体数:%d"), length);

		//选择与指定多边形区域相交的所有实体
		//acedSSGet(_T("CP"), pointlist, NULL, NULL, ssname);
		//acutRelRb(pointlist);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n与指定多边形区域相交的所有实体数:%d"), length);

		//选择与选择栏相交的是所有对象
		//pt4[Y] = 15.0; pt4[Z] = 0.0;
		//pointlist = acutBuildList(RTPOINT, pt1, RTPOINT, pt2, RTPOINT, pt3, RTPOINT, pt4, 0);
		//acedSSGet(_T("F"), pointlist, NULL, NULL, ssname);
		//acedSSLength(ssname, &length);
		//acutPrintf(_T("\n与选择栏相交的是所有实体数:%d"), length);

		//acutRelRb(pointlist);
		acedSSFree(ssname);
	}
	static void zwzw_SelectInPoly()
	{
		//提示用户选择多段线
		ads_name entName;
		ads_point pt;
		if (acedEntSel(_T("\n选择多段线:"), entName, pt) != RTNORM)
		{
			return;
		}
		AcDbObjectId entId;
		acdbGetObjectId(entId, entName);

		//判断选择的实体是否是多段线
		AcDbEntity *pEnt;
		acdbOpenObject(pEnt, entId, AcDb::kForWrite);
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
			AcDbObjectIdArray ObjectIdArray;//选择到的实体ID集合
			SelectEntInPoly(pPoly, ObjectIdArray, _T("CP"), 1);
			acutPrintf(_T("\n选择到 %d 个实体."), ObjectIdArray.length());

		}
		pEnt->close();
	}
	static void zwzw_Filter1()
	{
		struct resbuf *rb;	//结果缓冲区链表
		ads_name ssname;

		rb = acutBuildList(RTDXF0, _T("TEXT"),	//实体类型
			8, _T("0,图层1"),	//图层
			1, _T("*cadhelp*"),		//包含的字符串
			RTNONE);

		//选择符合要求的文字
		acedSSGet(_T("X"), NULL, NULL, rb, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数：%d"), length);

		acutRelRb(rb);
		acedSSFree(ssname);
	}
	static void zwzw_Filter2()
	{
		struct resbuf *rb;	//结果缓冲区链表
		ads_name ssname;

		rb = acutBuildList(-4, _T("<OR"),	//逻辑运算符开始
			RTDXF0, _T("TEXT"),	//一个条件
			RTDXF0, _T("MTEXT"),	//另一个条件
			-4, _T("OR>"),	//逻辑运算符结束
			RTNONE);
		//选择符合要求的文字
		acedSSGet(_T("X"), NULL, NULL, rb, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数：%d"), length);

		acutRelRb(rb);
		acedSSFree(ssname);

	}
	static void zwzw_Filter3()
	{
		struct resbuf *rb;	//结果缓冲区链表
		ads_name ssname;

		rb = acutBuildList(RTDXF0, _T("CIRCLE"),	//实体类型
			-4, _T(">="),	//关系运算符
			40, 30,	//半径
			RTNONE);
		//选择符合要求的文字
		acedSSGet(_T("X"), NULL, NULL, rb, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数：%d"), length);

		acutRelRb(rb);
		acedSSFree(ssname);

	}
	static void zwzw_Filter4()
	{
		struct resbuf *rb;	//结果缓冲区链表
		ads_name ssname;

		ads_point pt1 = { 0,0,0 };
		ads_point pt2 = { 100,100,0 };
		rb = acutBuildList(RTDXF0, _T("CIRCLE"),	//实体类型
			-4, _T(">,>,*"),	//关系运算符和通配符
			10, pt1,	//圆心
			-4, _T("<,<,*"),	//关系运算符和通配符
			10, pt2,	//圆心
			RTNONE);
		//选择符合要求的文字
		acedSSGet(_T("X"), NULL, NULL, rb, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数：%d"), length);

		acutRelRb(rb);
		acedSSFree(ssname);
	}
	static void zwzw_Filter5()
	{

		struct resbuf *rb;	//结果缓冲区链表
		ads_name ssname;

		rb = acutBuildList(1001, _T("XData"),	//扩展属性的应用程序名
			RTNONE);

		//选择符合要求的文字
		acedSSGet(_T("X"), NULL, NULL, rb, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数：%d"), length);

		acutRelRb(rb);
		acedSSFree(ssname);
	}
	static void zwzw_Filter6()
	{

		struct resbuf *rb;	//结果缓冲区链表
		ads_name ssname;

		rb = acutBuildList(1000, _T("Road"),	//扩展数据中ASCII字符串
			RTNONE);

		//选择符合要求的文字
		acedSSGet(_T("X"), NULL, NULL, rb, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数：%d"), length);

		acutRelRb(rb);
		acedSSFree(ssname);
	}
	static void zwzw_Test2()
	{
		ads_name ssname;
		ads_point pt1, pt2;
		pt1[X] = pt1[Y] = pt1[Z] = 0.0;
		pt2[X] = pt2[Y] = 100;
		pt2[Z] = 0.0;

		//选择图形中与pt1和pt2组成的窗口相交的所有对象
		acedSSGet(_T("C"), pt1, pt2, NULL, ssname);
		long length;
		acedSSLength(ssname, &length);
		acutPrintf(_T("\n实体个数: %d"), length);

		acedSSFree(ssname);
	}
	static void zwzw_EntInfo01()
	{
		acDocManager->sendStringToExecute(acDocManager->curDocument(), _T("(entget(car(entsel))) "));
	}
	static void zwzw_SelectEntity()
	{
		//提示用户选择一个圆，将其颜色修改为红色
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *desc = AcDbCircle::desc();

		if (CSelectUtil::PromptSelectEntity(_T("\n选择圆:"), desc, pEnt, pickPoint))
		{
			pEnt->setColorIndex(1);
			pEnt->close();
		}
	}
	static void zwzw_SelectEntitys()
	{
		//提示用户选择多个圆或直线，将其颜色修改为红色
		std::vector<AcRxClass*> descs;
		descs.push_back(AcDbLine::desc());
		descs.push_back(AcDbCircle::desc());
		AcDbObjectIdArray entIds;
		if (CSelectUtil::PromptSelectEnts(_T("\n 选择圆和直线:"), descs, entIds))
		{
			for (int i = 0; i < entIds.length(); i++)
			{
				AcDbEntity *pEnt;
				if (acdbOpenObject(pEnt, entIds[i], AcDb::kForWrite) == Acad::eOk)
				{
					pEnt->setColorIndex(1);
					pEnt->close();
				}
			}
		}
	}
	static void zwzw_DrawSequareJig()
	{
		//提示用户输入中心点
		AcGePoint3d centerPoint;
		if (CGetInputUtil::GetPoint(_T("\n指定正方形的中心点："), centerPoint))
		{
			//进入拖动状态
			CDrawSquareJig jig;
			AcDbObjectId polyId;
			if (jig.doIt(centerPoint, polyId))
			{
				//成功创建之后，可以进行其他修改
				CEntityUtil::SetColor(polyId, 1);
			}
			else
			{
				//用户取消，删除已经创建的实体
				CEntityUtil::Erase(polyId);
			}
		}
	}
	static void zwzw_ArcBlockJig()
	{
		// 选择一个块参照，用于沿圆弧插入
		AcDbEntity *pEnt = NULL;
		AcDbObjectId blkDefId;
		AcGePoint3d pickPoint;
		AcRxClass *desc = AcDbBlockReference::desc();
		if (CSelectUtil::PromptSelectEntity(TEXT("\n选择一个块参照用于沿圆弧插入:"), desc, pEnt, pickPoint))
		{
			AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
			blkDefId = pBlkRef->blockTableRecord();
			pEnt->close();
		}
		if (blkDefId.isNull())
		{
			return;
		}

		// 提示用户拾取第一点
		AcGePoint3d startPoint;
		if (!CGetInputUtil::GetPoint(TEXT("\n拾取第一点:"), startPoint))
		{
			return;
		}

		// 提示用户拾取第二点
		AcGePoint3d secondPoint;
		if (!CGetInputUtil::GetPoint(startPoint, TEXT("\n拾取第二点:"), secondPoint))
		{
			return;
		}

		// 开始拖动
		CArcBlockJig jig;
		int blockCount = 4;
		jig.doIt(startPoint, secondPoint, secondPoint, blkDefId, blockCount);
	}
	static void zwzw_EntTipOn()
	{
		if (!DocVars.docData().m_pIPM)
		{
			new CEntInfoTipHandler;
		}
	}
	static void zwzw_EntTipOff()
	{
		if (DocVars.docData().m_pIPM)
		{
			delete DocVars.docData().m_pIPM;
			DocVars.docData().m_pIPM = NULL;
		}
	}
	static void zwzw_AddXData()
	{
		//提示用户选择所要添加扩展数据的图形对象
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *pDesc = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEntity(_T("\n选择所要添加扩展数据的实体:"), pDesc, pEnt, pickPoint))
		{
			CString appName = _T("XDataApp");
			//注册应用程序名称
			acdbRegApp(appName);
			//创建结果缓冲区链表
			struct resbuf *rb = acutBuildList(
				AcDb::kDxfRegAppName, appName,	//应用程序名
				AcDb::kDxfXdAsciiString, _T("字符串测试数据"),	//字符串
				AcDb::kDxfXdInteger32, 2,	//整数
				AcDb::kDxfXdReal, 3.14,	//实数
				AcDb::kDxfXdWorldXCoord, asDblArray(pickPoint),	//坐标值
				RTNONE
				);

			//未选择的实体添加扩展数据
			pEnt->setXData(rb);
			acutRelRb(rb);

			acutPrintf(_T("\n成功为实体添加了扩展数据."));

			pEnt->close();
		}
	}
	static void zwzw_ViewXData()
	{
		//提示用户选择所要查看扩展数据的图形对象
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *pdesc = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEntity(_T("\n选择所要查看扩展数据的实体:"), pdesc, pEnt, pickPoint))
		{
			CString appName = _T("XDataApp");
			//获取扩展数据
			struct resbuf *pRb = pEnt->xData(appName);
			if (pRb != NULL)
			{
				//在命令行显示所有的扩展属性
				struct resbuf *pTemp = pRb;	//使用临时的结果缓冲区指针进行遍历,pRb不修改便于释放

				//首先要跳过应用程序的名称这一项
				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n字符串类型的扩展数据是：%s"), pTemp->resval.rstring);

				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n整数类型的扩展数据是：%d"), pTemp->resval.rint);

				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n实数类型的扩展数据是：%.2f"), pTemp->resval.rreal);

				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n点坐标类型的扩展数据是：( %.2f , %.2f , %.2f )"), pTemp->resval.rpoint[X], pTemp->resval.rpoint[X], pTemp->resval.rpoint[Y]);

				acutRelRb(pRb);

			}
			else
			{
				acutPrintf(_T("\n所选的实体不包含任何的扩展数据！"));
			}
			pEnt->close();
		}

	}
	static void zwzw_DeleteXData()
	{
		//提示用户选择所要删除扩展数据的图形对象
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *pdesc = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEntity(_T("\n选择所要删除扩展数据的实体:"), pdesc, pEnt, pickPoint))
		{
			CString appName = _T("XDataApp");
			//注册应用程序名称
			acdbRegApp(appName);

			//创建结果缓冲区链表
			struct resbuf *rb = acutBuildList(AcDb::kDxfRegAppName, appName, RTNONE);

			//设置内容为空的结果缓冲区链表就是删除扩展数据
			pEnt->setXData(rb);

			acutRelRb(rb);

			acutPrintf(_T("\n成功为实体删除了扩展属性."));

			pEnt->close();
		}

	}
	static void zwzw_AddXRecord()
	{
		//提示用户选择所需要添加扩展记录的图形对象
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *pDesc = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEntity(_T("\n选择所要添加扩展记录的实体:"), pDesc, pEnt, pickPoint))
		{
			//向实体中添加扩展字典
			pEnt->createExtensionDictionary();
			AcDbObjectId dictObjId = pEnt->extensionDictionary();
			pEnt->close();

			//创建扩展记录
			AcDbXrecord *pXrec = new AcDbXrecord();

			//向扩展字典中添加一条记录
			AcDbObjectId xRecordId;
			AcDbDictionary *pDic;
			if (acdbOpenObject(pDic, dictObjId, AcDb::kForWrite)==Acad::eOk)
			{
				pDic->setAt(_T("XRecord"), pXrec, xRecordId);
				pDic->close();
			}

			//设置扩展记录的内容
			struct resbuf *pRb = acutBuildList(
				AcDb::kDxfText, _T("测试测试字符串"),
				AcDb::kDxfInt32, 12,
				AcDb::kDxfReal, 3.14,
				AcDb::kDxfXdXCoord, asDblArray(pickPoint),
				RTNONE
				);
			pXrec->setFromRbChain(*pRb);	//扩展数据内容填充扩展记录对象
			pXrec->close();
			acutRelRb(pRb);
		}
	}
	static void zwzw_ViewXRecord()
	{
		//提示用户选择所需查看扩展记录的图形对象
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *pDesc = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEntity(_T("\n选择所要查看扩展记录的实体:"), pDesc, pEnt, pickPoint))
		{
			//获得实体的扩展字典
			AcDbObjectId dictObjId = pEnt->extensionDictionary();
			pEnt->close();
			if (dictObjId.isNull())
			{
				acutPrintf(_T("\n所选择的实体不包含扩展字典！ "));
				return;
			}

			//打开扩展字典，获得与关键字"XRecord关联的扩展记录"
			AcDbDictionary *pDic = NULL;
			AcDbXrecord *pXrec = NULL;
			struct resbuf *pRb = NULL;
			if (acdbOpenObject(pDic, dictObjId, AcDb::kForRead)==Acad::eOk)
			{
				pDic->getAt(_T("XRecord"), (AcDbObject*&)pXrec, AcDb::kForRead);

				//获得扩展记录的数据链表并关闭扩展记录对象
				pXrec->rbChain(&pRb);
				pXrec->close();

				pDic->close();
			}

			if (pRb != NULL)
			{
				//在命令行显示扩展数据记录内容
				struct resbuf *pTemp = pRb;

				acutPrintf(_T("\n字符串类型的扩展数据是： %s"), pTemp->resval.rstring);

				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n整数类型的扩展数据是：%d"), pTemp->resval.rint);

				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n实数类型的扩展数据是：%.2f"), pTemp->resval.rreal);

				pTemp = pTemp->rbnext;
				acutPrintf(_T("\n点坐标的扩展数据是： (%.2f,%.2f,%.2f)"), pTemp->resval.rpoint[X], pTemp->resval.rpoint[Y], pTemp->resval.rpoint[Z]);

				acutRelRb(pRb);
			}
		}

	}
	static void zwzw_AddNameDict()
	{
		//获得又名对象字典，向其中添加指定的字典项
		AcDbDictionary *pNameObjDic = NULL;
		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObjDic, AcDb::kForWrite);

		//检查所要添加的字典项是否已经存在
		AcDbObjectId dicObjId;
		AcDbDictionary *pDic = NULL;
		if (pNameObjDic->getAt(_T("MyDict"), (AcDbObject*&)pDic, AcDb::kForWrite) == Acad::eKeyNotFound)
		{
			pDic = new AcDbDictionary();
			pNameObjDic->setAt(_T("MyDict"), pDic, dicObjId);
			pDic->close();
		}
		pNameObjDic->close();

		//向新的字典中添加一个图层对象
		if (acdbOpenObject(pDic, dicObjId, AcDb::kForWrite) == Acad::eOk)
		{
			AcDbLayerTableRecord *pLayer = new AcDbLayerTableRecord();
			AcCmColor color;
			color.setColorIndex(20);
			pLayer->setColor(color);
			pLayer->setIsOff(true);
			pLayer->setIsFrozen(true);
			AcDbObjectId layerId;
			Acad::ErrorStatus es = pDic->setAt(_T("LayerKey"), pLayer, layerId);
			pLayer->close();
			pDic->close();
		}

	}
	static void zwzw_ViewNameDict()
	{
		//获得对象有名字典中指定的字典项
		AcDbDictionary *pNameObject = NULL;
		Acad::ErrorStatus es;
		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObject, AcDb::kForRead);
		AcDbDictionary *pDict = NULL;
		es = pNameObject->getAt(_T("MyDict"), (AcDbObject*&)pDict, AcDb::kForRead);
		pNameObject->close();

		//如果不存在指定的字典项，则推出程序
		if (es == Acad::eKeyNotFound)
		{
			acutPrintf(_T("\n不存在字典项MyDict."));
			return;
		}

		//获得指定的对象字典中的对象
		AcDbLayerTableRecord *pLayer = NULL;
		if (pDict->getAt(_T("LayerKey"), (AcDbObject*&)pLayer, AcDb::kForRead) == Acad::eOk)
		{
			//获得实体信息
			int colorIndex = pLayer->color().colorIndex();
			bool bOff = pLayer->isOff();
			bool bFrozen = pLayer->isFrozen();

			//在命令行打印实体
			acutPrintf(_T("\n字典中的图层颜色索引：%d"), colorIndex);
			acutPrintf(_T("\n字典中图层是否是关闭状态：%s"), bOff ? _T("是") : _T("否"));
			acutPrintf(_T("\n字典中图层是否是冻结状态：%s"), bFrozen ? _T("是") : _T("否"));
			pLayer->close();
		}
		pDict->close();
	}
	static void zwzw_AddGroup()
	{
		//提示用户选择多个实体
		AcDbObjectIdArray entIds;
		AcRxClass *pDec = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEnts(_T("\n选择要组成的实体:"), pDec, entIds))
		{
			//创建组
			AcDbGroup *pGroup = new AcDbGroup();
			for (int i = 0; i < entIds.length(); i++)
			{
				pGroup->append(entIds[i]);
			}
			//将组添加到有名对象字典的组字典中
			AcDbDictionary *pGroupDict = NULL;
			acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);

			//如果组名为*，则会创建匿名组
			CString groupName = _T("*");
			AcDbObjectId groupId;
			pGroupDict->setAt(groupName, pGroup, groupId);
			pGroupDict->close();
			pGroup->close();
		}
	}
	static void zwzw_DeleteGroup()
	{
		AcDbObjectIdArray entIds;
		AcRxClass *pDec = AcDbEntity::desc();
		if (CSelectUtil::PromptSelectEnts(_T("\n选择要分解的组中实体:"), pDec, entIds))
		{
			//遍历组字典，判断是否包含了给定的实体
			AcDbDictionary *pGroupDict = NULL;
			acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
			AcDbDictionaryIterator *it = pGroupDict->newIterator();
			for (; !it->done(); it->next())
			{
				AcDbGroup *pGroup = NULL;
				if (it->getObject((AcDbObject*&)pGroup, AcDb::kForWrite) == Acad::eOk)
				{
					AcDbObjectIdArray groupEntIds;
					pGroup->allEntityIds(groupEntIds);
					bool bFound = false;
					for (int i = 0; i < entIds.length(); i++)
					{
						if (groupEntIds.contains(entIds[i]))
						{
							bFound = true;
							break;
						}
					}
					//删除包含选择集中实体的组
					if (bFound)
					{
						pGroup->erase();
					}
					pGroup->close();
				}
			}
			pGroupDict->close();
		}
	}
	static void zwzw_CreateDwg()
	{
		//创建新的图形数据库，分配内存空间
		AcDbDatabase *pDb = new AcDbDatabase(true, false);

		AcDbBlockTable *pBlkTbl = NULL;
		pDb->getBlockTable(pBlkTbl, AcDb::kForRead);

		AcDbBlockTableRecord *pBlkTblRcd = NULL;
		pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForWrite);
		pBlkTbl->close();

		//创建两个圆
		AcDbCircle *pCircle1 = new AcDbCircle(AcGePoint3d(1, 1, 1), AcGeVector3d(0, 0, 1), 1.0);
		pBlkTblRcd->appendZcDbEntity(pCircle1);
		pCircle1->close();
		AcDbCircle *pCircle2 = new AcDbCircle(AcGePoint3d(4, 4, 4), AcGeVector3d(0, 0, 1), 2.0);
		pBlkTblRcd->appendZcDbEntity(pCircle2);
		pCircle2->close();
		pBlkTblRcd->close();

		//获取acad.exe的位置
		CString acadPath;
		GetAcadPath(acadPath);

		//去掉路径最后的“acad.exe”字符串，得到AutoCAD安装路径
		acadPath = acadPath.Left(acadPath.GetLength() - 9);
		CString filePath = acadPath + _T("test.dwg");

		//使用SaveAs成员函数时，必须指定包含dwg扩展名的文件名称
		pDb->saveAs(filePath);

		delete pDb;

	}
	static void zwzw_ReadDwg()
	{
		//使用false作为构造函数的参数，创建一个空的图形数据库
		//这样保证图形数据库仅仅包含读入的内容
		AcDbDatabase *pDb = new AcDbDatabase(false, false);

		//AcDbDatabase::ReadDwgFile()函数可以自家添加dwg扩展名
		CString acadPath;
		GetAcadPath(acadPath);
		//去掉路劲最后的"acad.exe"字符串
		acadPath = acadPath.Left(acadPath.GetLength() - 9);
		CString fileName = acadPath + _T("test.dwg");
		pDb->readDwgFile(fileName, _SH_DENYWR);

		//获取模型空间的所有实体
		AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds(NULL, pDb);
		for (int i = 0; i < allEntIds.length(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk)
			{
				acutPrintf(_T("\n类名称:%s"), (pEnt->isA()->name()));
				pEnt->close();
			}
		}
		//删除图形数据库
		delete pDb;
	}
	static void zwzw_CreateDwg2()
	{
		//创建新的图形数据库，分配内存空间
		AcDbDatabase *pDb = new AcDbDatabase(true, false);

		//创建两个圆
		AcDbCircle *pCircle = new AcDbCircle(AcGePoint3d(1, 1, 1), AcGeVector3d(0, 0, 1), 1.0);
		CDwgDatabaseUtil::PostToModelSpace(pCircle, pDb);
		AcDbCircle *pCircle2 = new AcDbCircle(AcGePoint3d(4, 4, 4), AcGeVector3d(0, 0, 1), 2.0);
		CDwgDatabaseUtil::PostToModelSpace(pCircle2, pDb);

		//调整DWG文件的视图
		CViewUtil::DwgZoomExtent(pDb);

		//获得acad.exe的位置
		CString acadPath;
		GetAcadPath(acadPath);
		//去掉路径最后的"acad.exe"字符串，得到AutoCAD安装路径
		acadPath = acadPath.Left(acadPath.GetLength() - 9);
		CString fileName = acadPath + _T("test2.dwg");

		//使用saveAs成员函数时，必须指定包含dwg扩展名的文件名称
		pDb->saveAs(fileName);

		delete pDb;	//pDb不是数据库的常驻对象，必须手动销毁
	}
	static void zwzw_InsertBlkrefFromOtherDwg()
	{
		CString fileName = _T("d:\\包含图块.dwg");
		if (_taccess(fileName, 0) != -1)
		{
			AcDbObjectId blkDefId = CBlockUtil::CopyBlockDefFromOtherDwg(fileName, _T("测试图块"));
			if (blkDefId.isValid())
			{
				CBlockUtil::InsertBlockRef(blkDefId, AcGePoint3d(100, 100, 0), 1, 0);
			}
		}
		else
		{
			acutPrintf(_T("\n指定的图形文件不存在."));
		}
	}
	static void zwzw_InsertDwgBlockRef()
	{
		CString fileName = _T("d:\\作为图块的文件.dwg");
		if (_taccess(fileName, 0) != -1)
		{
			AcDbObjectId blkDefId = CBlockUtil::InsertDwgBlockDef(fileName, _T("外部DWG图块"), true);
			if (blkDefId.isValid())
			{
				CBlockUtil::InsertBlockRef(blkDefId, AcGePoint3d(200, 100, 0), 1, 0);
			}
		}
		else
		{
			acutPrintf(_T("\n指定的图形文件不存在."));
		}
	}
	static void zwzw_NewDocDrawEnts()
	{
		//1.当前图形中的计算
		//必须锁定文档才能访问图形数据库的内容
		acDocManager->lockDocument(acDocManager->curDocument());
		//找出图形中直线的数量
		AcDbObjectIdArray allEntIds=CDwgDatabaseUtil::GetAllEntityIds();
		int lineCount=0;
		for(int i=0;i<allEntIds.length();i++)
		{
			AcDbLine *pLIne=NULL;
			if(acdbOpenObject(pLIne,allEntIds[i],AcDb::kForRead)==Acad::eOk)
			{
				lineCount++;
				pLIne->close();
			}
		}
		//解锁文档，返回应用程序范围
		acDocManager->unlockDocument(acDocManager->curDocument());

		//2.创建一个新图形文档并作为当前文档
		Acad::ErrorStatus es=acDocManager->appContextNewDocument(NULL);

		//3.在新的图形文档中绘图
		//必须锁定文档才能访问数据库的内容
		acDocManager->lockDocument(acDocManager->curDocument());

		//绘制与原来图形中相同数量的直线
		for(int i=0;i<lineCount;i++)
		{
			double y=i*20;
			AcGePoint3d startPoint(0,y,0);
			AcGePoint3d endPoint(100,y,0);
			CLineUtil::Add(startPoint,endPoint);
		}

		//解锁文档，返回应用程序范围
		acDocManager->unlockDocument(acDocManager->curDocument());
	}
	static void zwzw_SaveDwgOpenDoc()
	{
		//1.当前图形中的计算
		//必须锁定文档才能访问图形数据库的内容
		acDocManager->lockDocument(acDocManager->curDocument());
		
		//找出图形中直线的数量和圆的数量
		AcDbObjectIdArray allEntIds=CDwgDatabaseUtil::GetAllEntityIds();
		int lineCount=0,circleCount=0;
		for(int i=0;i<allEntIds.length();i++)
		{
			AcDbEntity *pEnt=NULL;
			if(acdbOpenObject(pEnt,allEntIds[i],AcDb::kForRead)==Acad::eOk)
			{
				if(pEnt->isKindOf(AcDbLine::desc()))
				{
					lineCount++;
				}
				else if(pEnt->isKindOf(AcDbCircle::desc()))
				{
					circleCount++;
				}
				pEnt->close();
			}
		}

		//创建两个新DWG文件
		AcDbDatabase *pDb1=new AcDbDatabase(true,false);
		for(int i=0;i<lineCount;i++)
		{
			double y=i*20;
			AcGePoint3d startPoint(0,y,0);
			AcGePoint3d endPoint(100,y,0);
			CLineUtil::Add(startPoint,endPoint,pDb1);
		}
		CString fileName1=CAppDirectoryUtil::GetCurrentDirctory()+_T("\\db1.dwg");
		pDb1->saveAs(fileName1);
		delete pDb1;
		AcDbDatabase *pDb2=new AcDbDatabase(true,false);
		for(int i=0;i<circleCount;i++)
		{
			double y=i*20;
			AcGePoint3d center(0,y,0);
			CCircleUtil::Add(center,5,pDb2);
		}
		CString fileName2=CAppDirectoryUtil::GetParentDirectory()+_T("\\db2.dwg");
		pDb2->saveAs(fileName2);
		delete pDb2;

		//解锁文档，返回应用程序范围
		acDocManager->unlockDocument(acDocManager->curDocument());

		//2.打开已经存在的DWG文件，并切换为当前文档
		Acad::ErrorStatus es=acDocManager->appContextOpenDocument(fileName1);
		es=acDocManager->appContextOpenDocument(fileName2);
	}
	static void zwzw_IntersectWith()
	{
		//创建所要计算交点的几何类对象
		AcGeCircArc2d geArc(AcGePoint2d::kOrigin,50,0,6);
		AcGeLine2d geLine(AcGePoint2d::kOrigin,AcGePoint2d(10,10));

		//计算并输出交点
		AcGePoint2d point1,point2;
		int num;
		if(geArc.intersectWith(geLine,num,point1,point2))
		{
			acutPrintf(_T("\n直线和圆弧有%d个交点."),num);
			if(num>0)
			{
				acutPrintf(_T("\n交点1坐标：（%.4f,%.4f）."),point1.x,point1.y);
			}
			if(num>1)
			{
				acutPrintf(_T("\n交点2坐标：（%.4f,%.4f）."),point2.x,point2.y);
			}
		}
	}
	static void zwzw_LineDistance()
	{
		//提示用户选择所要计算距离的两条直线
		AcDbObjectIdArray lineIds;
		AcRxClass *pDesc=AcDbLine::desc();
		if(CSelectUtil::PromptSelectEnts(_T("\n选择两条直线："),pDesc,lineIds))
		{
			if(lineIds.length()!=2)
			{
				acutPrintf(_T("\n必须选择两条直线."));
				return;
			}

			//将两条直线转换成对应的几何类对象
			AcGeLineSeg2d geLine1=GetGeLineObj(lineIds[0]);
			AcGeLineSeg2d geLine2=GetGeLineObj(lineIds[1]);

			//计算并输出两者之间的最短距离
			double distance=geLine1.distanceTo(geLine2);
			acutPrintf(_T("\n两条直线之间的最短距离为：%.4f."),distance);
		}
	}
	static void zwzw_CurveBoolean()
	{
		//选择所要操作的两条多段线
		AcDbObjectIdArray polyIds;
		AcRxClass *pDesc=AcDbPolyline::desc();
		if(CSelectUtil::PromptSelectEnts(_T("\n选择两条多段线："),pDesc,polyIds))
		{
			if(polyIds.length()!=2)
			{
				acutPrintf(_T("\n必须选择两条直线."));
				return;
			}

			//获得两条多段线的交点
			bool bOk=false;
			AcDbPolyline *pPoly1=NULL,*pPoly2=NULL;
			if(acdbOpenObject(pPoly1,polyIds[0],AcDb::kForWrite)==Acad::eOk)
			{
				if(acdbOpenObject(pPoly2,polyIds[1],AcDb::kForWrite)==Acad::eOk)
				{
					AcGePoint3dArray intPoints;
					pPoly1->intersectWith(pPoly2,AcDb::kOnBothOperands,intPoints);
					if(intPoints.length()>=2)
					{
						bOk=true;
					}
					else
					{
						acutPrintf(_T("\n多段线之间交点少于2个，无法进行计算."));
					}

					//根据交点和参数值获得交点之间的曲线
					if(bOk)
					{
						GetCurveBetweenIntPoints(pPoly1,intPoints);
						GetCurveBetweenIntPoints(pPoly2,intPoints);

						pPoly2->erase();
					}
					pPoly2->close();
				}
				if(bOk)
				{
					pPoly1->erase();
				}
				pPoly1->close();
			}
		}
	}
	static void zwzw_Drawpipe()
	{
		// 提示用户输入起点、终点
		AcGePoint3d startPoint, endPoint;
		if (CGetInputUtil::GetPoint(TEXT("\n输入起点:"), startPoint) && 
			CGetInputUtil::GetPoint(startPoint, TEXT("\n输入终点:"), endPoint))
		{
			// 绘制管道
			DrawPipe(startPoint, endPoint, 100, 70);
		}
	}
	static void zwzw_TestPtInPoly()
	{
		int count = 100000;		// 随机点测试的数量
		// 提示用户选择一条多段线
		AcDbEntity *pEnt = NULL;
		AcGePoint3d pickPoint;
		AcRxClass *pDesc=AcDbPolyline::desc();
		if (CSelectUtil::PromptSelectEntity(TEXT("\n选择需要进行测试的闭合多段线:"), pDesc, pEnt, pickPoint))
		{
			// 在多段线包围框范围内随机生成点，测试点和多段线的位置关系
			AcDbExtents ext;
			pEnt->getGeomExtents(ext);
			double margin = 10;
			double xmin = ext.minPoint().x - margin;
			double ymin = ext.minPoint().y - margin;
			double xSpan = ext.maxPoint().x - ext.minPoint().x + 2 * margin;
			double ySpan = ext.maxPoint().y - ext.minPoint().y + 2 * margin;

			AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
			srand((unsigned)time(NULL));
			for (int i = 0; i < count; i++)
			{
				// 在0～maxRand之间产生随机数
				int maxRand = 100000;
				int xRand = CMathUtil::GetRand(0, maxRand);
				int yRand = CMathUtil::GetRand(0, maxRand);

				double x = xmin + ((double)xRand / maxRand) * xSpan;
				double y = ymin + ((double)yRand / maxRand) * ySpan;
				int relation = CPolylineUtil::PtRelationToPoly(pPoly, AcGePoint2d(x, y), 1.0E-4);

				int colorIndex = 0;
				switch (relation)
				{
				case -1:
					colorIndex = 1;
					break;
				case 0:
					colorIndex = 5;
					break;
				case 1:
					colorIndex = 6;
					break;
				default:
					break;
				}
				AcDbPoint *pPoint = new AcDbPoint(AcGePoint3d(x, y, 0));
				pPoint->setColorIndex(colorIndex);
				CDwgDatabaseUtil::PostToModelSpace(pPoint);
			}

			pEnt->close();
		}
	}
	static void zwzw_SetDwgScale()
	{
		AcDbDictionary *pNameObject=NULL;
		Acad::ErrorStatus es;
		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObject,AcDb::kForWrite);
		DwgScale *pDwgScale=new DwgScale();
		pDwgScale->Set(1,100);
		AcDbObjectId id;
		es=pNameObject->setAt(_T("DWG_SCALE"),pDwgScale,id);
		pDwgScale->close();
		pNameObject->close();
	}
	static void zwzw_GetDwgScale()
	{
		AcDbDictionary *pNameObject=NULL;
		Acad::ErrorStatus es;
		acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObject,AcDb::kForRead);
		DwgScale *pDwgScale=NULL;
		es=pNameObject->getAt(_T("DWG_SCALE"),(AcDbObject*&)pDwgScale,AcDb::kForRead);
		pNameObject->close();

		//如果不存在指定字典项，则推迟程序
		if(es==Acad::eKeyNotFound)
		{
			acutPrintf(_T("\n不存在指定字典项%s.",_T("DWG_SCALE")));
			return;
		}
		acutPrintf(_T("\n%d-%d"),pDwgScale->GetInfoScale(),pDwgScale->GetLableScale());
		pDwgScale->close();
	}
	static void zwzw_AddTriangle(void)
	{
		// Add your code for command CZLCadCommon._AddTriangel here
		ZfgkTriangle *pTriangle=new ZfgkTriangle(AcGePoint3d(0,0,0),AcGePoint3d(100,0,0),AcGePoint3d(100,100,30));
		CDwgDatabaseUtil::PostToModelSpace(pTriangle);
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CZrxTemplate1App)
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, _test, test, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)


/*第二章：创建和编辑基本图形对象*/
//创建直线
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateLine, CreateLine, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建圆
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateCircle, CreateCircle, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建圆弧
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateArc, CreateArc, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建多段线
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreatePolyLine, CreatePolyLine, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建椭圆
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateEllipse, CreateEllipse, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建样条曲线
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateSpline, CreateSpline, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建面域
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateRegion, CreateRegion, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建文字和多行文字
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateText, CreateText, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建填充
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateHatch, CreateHatch, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建坐标标注
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateDimension, CreateDimension, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//获取指定图层上所有实体  修改直线颜色为红色
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_GetAllEntByLayer, GetAllEntByLayer, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

/*第三章：块和属性*/
//创建块定义
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateBlock, CreateBlock, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//插入块参照
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_InsertBlock, InsertBlock, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//插入块参照
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_InsertBlock2, InsertBlock2, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//插入带属性块参照
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_InsertBlock3, InsertBlock3, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//计算图形中直线与块参照的真实交点
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_BlockIntersectWith, BlockIntersectWith, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)



/*第四章：符号表*/
//创建新图层
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_NewLayer, NewLayer, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//修改指定图层的颜色
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_LayerColor, LayerColor, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//删除指定图层
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_DeleteLayer, DeleteLayer, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//将当前图形文件中存在的所有图层及其特性导出到一个文本中
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ExportLayer, ExportLayer, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//根据文本中图层的特性在CAD中创建图层
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ImportLayer, ImportLayer, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)


//创建新的字体文字样式表
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddTextStyle, AddTextStyle, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

//模拟窗口缩放功能
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ZoomWindow, ZoomWindow, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//模拟Zoom E
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ZoomExtents, ZoomExtents, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//以圆心为中点，缩放至圆的包围面
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ZoomEntity, ZoomEntity, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)


//创建4个等大的视口
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Create4VPotrs, Create4VPotrs, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 在图纸空间创建一个新的视口
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateVPotrInSpace, CreateVPotrInSpace, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

// 创建一个新的UCS，并添加到UCS表中
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_NewUCS, NewUCS, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 设置当前UCS
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SetCurUcs, SetCurUcs, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 设置当前UCS
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_MoveUcsOrigin, MoveUcsOrigin, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 旋转当前UCS
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_RotateUcs, RotateUcs, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 根据UCS中的坐标创建实体
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddEntInUcs, AddEntInUcs, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)


/*第五章：ADSRX和用户交互*/
//使用acedCommand创建一个圆
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddCircle1, AddCircle1, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//使用acedCmd创建一个圆
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddCircle2, AddCircle2, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//提示用户选择一个实体，显示实体的参数
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_EntInfo, EntInfo, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 实现基本的交互绘制多段线
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddPolyBasic, AddPolyBasic, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 实现acedGetPoint函数中使用关键字的基本方法
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_GetPointKeyword, GetPointKeyword, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// 提示用户输入多段线的节点、线宽和颜色，完成多段线的创建
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddPoly, AddPoly, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//提示用户输入一个文件名
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SelectFile, SelectFile, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

//演示选择集的创建和删除
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateSSet, CreateSSet, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//最近一次创建的选择集
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SelectLast, SelectLast, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//使用多种不同的模式创建选择集
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SelectEnt, SelectEnt, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//提示用户选择一条多段线，命令行提示相交实体的个数   (在中望CAD中异常)
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SelectInPoly, SelectInPoly, ACRX_CMD_TRANSPARENT, NULL)
//带有通配符的过滤器
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Filter1, Filter1, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//带有逻辑运算符的过滤器
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Filter2, Filter2, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//带有关系运算符的过滤器
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Filter3, Filter3, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//复合过滤器
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Filter4, Filter4, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//过滤扩展数据的过滤器
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Filter5, Filter5, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//过滤扩展数据的过滤器
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Filter6, Filter6, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Test2, Test2, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//获取组码
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_EntInfo01, EntInfo01, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//测试用户选择一个实体
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SelectEntity, SelectEntity, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//测试用户选择多个实体
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SelectEntitys, SelectEntitys, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//JIG
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_DrawSequareJig, DrawSequareJig, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// JIG(自定义实体)
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ArcBlockJig, ArcBlockJig, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//动态显示实体信息
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_EntTipOn, EntTipOn, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//关闭动态显示实体信息
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_EntTipOff, EntTipOff, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

/*第五章：扩展数据、扩展记录和对象字典*/
//追加扩展数据
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddXData, AddXData, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//查看扩展数据
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ViewXData, ViewXData, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//删除扩展数据
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_DeleteXData, DelelteXData, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//追加实体扩展字典扩展记录
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddXRecord, AddXRecord, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//查看实体扩展字典扩展记录
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ViewXRecord, ViewXRecord, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//向有名对象字典中添加根字典
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddNameDict, AddNameDict, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//查看有名对象字典中指定根字典中的内容
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ViewNameDict, ViewNameDict, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建组
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddGroup, AddGroup, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//删除组
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_DeleteGroup, DeleteGroup, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)


/*第六章：图形数据库和文档*/
//创建一个新的图形文件，并保存在AutoCAD的安装路径下
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateDwg, CreateDwg, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//读取dwg文件
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_ReadDwg, ReadDwg, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建一个新的图形文件，并保存在AutoCAD的安装路径下,调整视图范围
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CreateDwg2, CreateDwg2, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//将外部图形文件中块定义复制到当前图形文件，然后再这个窗口中插入这个块定义的块参照
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_InsertBlkrefFromOtherDwg, InsertBlkrefFromOtherDwg, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//将某个DWG文件作为图块插入到当前的模型空间
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_InsertDwgBlockRef, InsertDwgBlockRef, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//根据当前图形数据库的内容计算，将计算结果绘制在新建的空白图形中
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_NewDocDrawEnts, NewDocDrawEnts, ACRX_CMD_SESSION | ACRX_CMD_USEPICKSET, NULL)
//根据当前图形数据库的内容计算，将计算结果绘制并保存两个后台的DWG文件,然后打开这两个DWG文件，切换为当前文档
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SaveDwgOpenDoc, SaveDwgOpenDoc, ACRX_CMD_SESSION | ACRX_CMD_USEPICKSET, NULL)


/*第七章：图形数据库和文档*/
//用于计算几何类和圆弧几何类的交点
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_IntersectWith, IntersectWith, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//计算两条直线之间的最短距离
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_LineDistance, LineDistance, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//用于获得两条曲线相交之后形成的边界线
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_CurveBoolean, CurveBoolean, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

//根据用户输入的起点、终点绘制矩形管道
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_Drawpipe, Drawpipe, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//判断点与多段线的位置关系
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_TestPtInPoly, TestPtInPoly, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)


/*第八章：自定义对象与自定义实体*/
//设置自定义对象
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_SetDwgScale, SetDwgScale, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//读取自定义对象
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_GetDwgScale, GetDwgScale, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
//创建自定义实体
ACED_ARXCOMMAND_ENTRY_AUTO(CZrxTemplate1App, zw, zw_AddTriangle, AddTriangle, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)









