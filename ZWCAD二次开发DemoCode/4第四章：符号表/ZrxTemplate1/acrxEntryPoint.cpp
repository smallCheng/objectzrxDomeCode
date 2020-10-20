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
class CZrxTemplate1App : public AcRxArxApp {
public:
	CZrxTemplate1App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: 如果有其他依赖项，请在这里加载

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: 初始化

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: 退出

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: 卸载依赖

		return (retCode);
	}

	virtual void RegisterServerComponents() {
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
		CArcUtil::Add(ptCenter, 100 * sqrt(2) / 2, 5 * CMathUtil::PI() / 4, 7 * CMathUtil::PI() / 4);

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
		AcDbObjectId blkDefId = CBlockUtil::GetBlkDefId(blkName);
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
		AcDbObjectId blkDefId = CBlockUtil::GetBlkDefId(blkName);
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
		vecXAxis.rotateBy(60 * atan(1) * 4 / 180, vecZAxis);
		vecYAxis.rotateBy(60 * atan(1) * 4 / 180, vecZAxis);

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
};

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