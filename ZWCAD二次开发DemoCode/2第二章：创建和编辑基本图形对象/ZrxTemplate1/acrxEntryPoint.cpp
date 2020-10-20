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
class CZrxTemplate1App : public AcRxArxApp{
public:
	CZrxTemplate1App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: 如果有其他依赖项，请在这里加载

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: 初始化

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: 退出

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: 卸载依赖

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void zwzw_CreateLine()
	{
		AcDbObjectId EntId;
		//创建直线的起点和终点
		AcGePoint3d ptStart(0, 0, 0);
		AcGePoint3d ptEnd(100, 100, 0);
		//创建直线
		EntId =ZrxCreateEntitys::CreateLine(ptStart,ptEnd);
		//修改直线颜色
		if (EntId != NULL)
		{
			acutPrintf(L"创建直线成功！\n");
			if (Acad::eOk == ZrxEditObject::SetColor(EntId, 1))
			{
				acutPrintf(L"修改直线颜色成功！\n");
			}
			else
			{
				acutPrintf(L"修改直线颜色失败！\n");
			}

		}
		else
		{
			acutPrintf(L"创建直线失败！\n");
		}
	}
	static void zwzw_CreateCircle()
	{
		//"圆心、半径"法创建一个圆
		AcGePoint3d ptCenter(100, 100, 0);
		ZrxCreateEntitys::CreateCircle(ptCenter, 20);

		//两点法创建一个圆
		AcGePoint3d pt1(70, 100, 0);
		AcGePoint3d pt2(130, 100,0);
		ZrxCreateEntitys::CreateCircle(pt1, pt2);

		//三点法创建一个圆
		AcGePoint2d pt01(60, 100);
		AcGePoint2d pt02(140, 100);
		AcGePoint2d pt03(100, 60);
		ZrxCreateEntitys::CreateCircle(pt01, pt02, pt03);
	}
	static void zwzw_CreateArc()
	{
		//创建位于XOY平面的圆弧
		AcGePoint2d ptCenter(50, 50);
		ZrxCreateEntitys::CreateArc(ptCenter, 100 * sqrt(2) / 2, 5 * GeoMetryUtil::PI() / 4, 7 * GeoMetryUtil::PI() / 4);
		
		//三点法创建圆弧
		AcGePoint2d ptStart(100, 0);
		AcGePoint2d ptCenter_or_OnArc(120, 50);
		AcGePoint2d ptEnd(100, 100);
		ZrxCreateEntitys::CreateArc(ptStart, ptCenter_or_OnArc, ptEnd, FALSE);
		
		//起点、圆心、终点创建圆弧
		ptStart.set(100, 100);
		ptCenter_or_OnArc.set(50, 50);
		ptEnd.set(0, 100);
		ZrxCreateEntitys::CreateArc(ptStart, ptCenter_or_OnArc, ptEnd);
		
		//起点 圆心 圆弧角度创建圆弧
		ptStart.set(0, 100);
		ptCenter.set(50, 50);
		ZrxCreateEntitys::CreateArc(ptStart, ptCenter, GeoMetryUtil::PI() / 2);

	}
	static void zwzw_CreatePolyLine()
	{
		//创建包含一段直线的多段线
		AcGePoint2d ptStart(0, 0), ptEnd(100, 100);
		ZrxCreateEntitys::CreatePolyOneLine(ptStart, ptEnd, 1);

		//创建三维多段线
		AcGePoint3d pt1(0, 0, 0), pt2(100, 0, 0), pt3(100, 100, 0);
		AcGePoint3dArray points;
		points.append(pt1);
		points.append(pt2);
		points.append(pt3);
		ZrxCreateEntitys::Create3dPolyLine(points);

		//创建正多边形
		ZrxCreateEntitys::CreatePolygon(AcGePoint2d::kOrigin, 6, 30, 0, 1);

		//创建矩形
		AcGePoint2d pt(60, 70);
		ZrxCreateEntitys::CreatePolyRectangle(pt, ptEnd, 1);

		//创建圆
		pt.set(50, 50);
		ZrxCreateEntitys::CreatePolyCircle(pt, 30, 1);

		//创建圆弧
		ZrxCreateEntitys::CreatePolyArc(pt, 50, GeoMetryUtil::AngleToRadian(45), GeoMetryUtil::AngleToRadian(255), 1);
	}
	static void zwzw_CreateEllipse()
	{
		//使用中点、所在平面、长轴矢量和短轴与长轴的比例创建椭圆
		AcGeVector3d vecNormal(0, 0, 1);
		AcGeVector3d majorAxis(40, 0, 0);
		AcDbObjectId entId;
		entId = ZrxCreateEntitys::CreateEillpse(AcGePoint3d::kOrigin, vecNormal, majorAxis, 0.5);

		//使用外接矩形角点来创建椭圆
		AcGePoint2d pt1(60, 80), pt2(140, 120);
		ZrxCreateEntitys::CreateEillpse(pt1, pt2);
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
		ZrxCreateEntitys::CreateSpline(points);

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
		ZrxCreateEntitys::CreateSpline(points, startTangent, endTangent);
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
		regionIds = ZrxCreateEntitys::CreateRegion(objIds);

		int number = regionIds.length();
		acutPrintf(L"\n已经创建 %d 个面域", number);

	}
	static void zwzw_CreateText()
	{
		//创建单行文字
		AcGePoint3d ptInsert(0, 4, 0);
		ZrxCreateEntitys::CreateText(ptInsert, L"武汉武大吉奥");

		//创建多行文字
		ptInsert.set(0, 0, 0);
		ZrxCreateEntitys::CreateMText(ptInsert, L"吉奥出品，必是精品！");

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
				acedSSName(ss, i,ent);
				AcDbObjectId objId;
				acdbGetObjectId(objId, ent);

				objIds.append(objId);
			}
		}
		acedSSFree(ss);
		ZrxCreateEntitys::CreateHatch(objIds, L"SOLID", 1.0);
	}
	static void zwzw_CreateDimension()
	{
		//指定起始点位置
		AcGePoint3d pt1(200, 160, 0);
		AcGePoint3d pt2 = GeoMetryUtil::RelativePoint(pt1, -40, 0);
		AcGePoint3d pt3 = GeoMetryUtil::PolarPoint(pt2, 7 * GeoMetryUtil::PI() / 6, 20);
		AcGePoint3d pt4 = GeoMetryUtil::RelativePoint(pt3, 6, -10);
		AcGePoint3d pt5 = GeoMetryUtil::RelativePoint(pt1, 0, -20);

		//绘制外轮廓线
		ZrxCreateEntitys::CreateLine(pt1, pt2);
		ZrxCreateEntitys::CreateLine(pt2, pt3);
		ZrxCreateEntitys::CreateLine(pt3, pt4);
		ZrxCreateEntitys::CreateLine(pt4, pt5);
		ZrxCreateEntitys::CreateLine(pt5, pt1);

		//绘制圆形
		AcGePoint3d ptCenter1, ptCenter2;
		ptCenter1 = GeoMetryUtil::RelativePoint(pt3, 16, 0);
		ptCenter2 = GeoMetryUtil::RelativePoint(ptCenter1, 25, 0);
		ZrxCreateEntitys::CreateCircle(ptCenter1, 3);
		ZrxCreateEntitys::CreateCircle(ptCenter2, 4);

		AcGePoint3d ptTemp1, ptTemp2;
		//水平标注
		ptTemp1 = GeoMetryUtil::RelativePoint(pt1, -20, 3);
		ZrxCreateEntitys::CreateDimRotated(pt1, pt2, ptTemp1, 0);

		//垂直标注
		ptTemp1 = GeoMetryUtil::RelativePoint(pt1, 4, 0);
		ZrxCreateEntitys::CreateDimRotated(pt1, pt5, ptTemp1, GeoMetryUtil::PI() / 2);

		//转角标注
		ptTemp1 = GeoMetryUtil::RelativePoint(pt3, -3, -6);
		ZrxCreateEntitys::CreateDimRotated(pt3, pt4, ptTemp1, 7 * GeoMetryUtil::PI() / 4);

		//对齐标志
		ptTemp1 = GeoMetryUtil::RelativePoint(pt2, -3, 4);
		ZrxCreateEntitys::CreateDimAligned(pt2, pt3, ptTemp1, AcGeVector3d(4, 10, 0), L"new position");

		//角度标注
		ptTemp1 = GeoMetryUtil::RelativePoint(pt5, -5, 5);
		ZrxCreateEntitys::CreateDim3PtAngular(pt5, pt1, pt4, ptTemp1);

		//半径标注
		ptTemp1 = GeoMetryUtil::PolarPoint(ptCenter1, GeoMetryUtil::PI() / 4, 3);
		ZrxCreateEntitys::CreateDimRadial(ptCenter1, ptTemp1, -3);

		//直径标注
		ptTemp1 = GeoMetryUtil::RelativePoint(ptCenter2, GeoMetryUtil::PI() / 4, 4);
		ptTemp2 = GeoMetryUtil::RelativePoint(ptCenter2, GeoMetryUtil::PI() / 4, -4);
		ZrxCreateEntitys::CreateDimDiameteric(ptTemp1, ptTemp2, 0);

		//坐标标注
		ZrxCreateEntitys::CreateDimOrdinate(ptCenter2, AcGeVector3d(0, -10, 0), AcGeVector3d(10, 0, 0));
	}
	static void zwzw_GetAllEntByLayer()
	{
		AcDbObjectIdArray allEntIds = CCADUtil::GetAllEntityIdsByLayerName(L"test");
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
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CZrxTemplate1App)
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



