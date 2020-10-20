#include "stdafx.h"
#include "PolylineUtil.h"


CPolylineUtil::CPolylineUtil()
{
}


CPolylineUtil::~CPolylineUtil()
{
}

AcDbObjectId CPolylineUtil::Add(const AcGePoint2dArray & points, double width)
{
	int numVertices = points.length();
	AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		pPoly->addVertexAt(i, points.at(i), 0, width, width);
	}
	return CDwgDatabaseUtil::PostToModelSpace(pPoly);
}

AcDbObjectId CPolylineUtil::Add(const AcGePoint2d & ptStart, const AcGePoint2d & ptEnd, double width)
{
	AcGePoint2dArray points;
	points.append(ptStart);
	points.append(ptEnd);
	return  Add(points, width);
}

AcDbObjectId CPolylineUtil::Add3dPolyline(const AcGePoint3dArray & points)
{
	AcGePoint3dArray verts = points;
	AcDb3dPolyline *pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, verts);
	return CDwgDatabaseUtil::PostToModelSpace(pPoly3d);
}

AcDbObjectId CPolylineUtil::AddPolygon(const AcGePoint2d & ptCenter, int number, double radius, double rotation, double width)
{
	double angle = 2 * CMathUtil::PI() / (double)number;
	AcGePoint2dArray points;
	for (int i = 0; i < number; i++)
	{
		AcGePoint2d pt;
		pt.x = ptCenter.x + radius*cos(i*angle);
		pt.y = ptCenter.y + radius*sin(i*angle);
		points.append(pt);
	}
	AcDbObjectId polyId = Add(points, width);

	//将其关闭
	AcDbEntity *pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, polyId, AcDb::kForWrite) == Acad::eOk)
	{
		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
		if (pPoly != NULL)
		{
			pPoly->setClosed(Adesk::kTrue);
		}
		pEnt->close();
	}
	CEntityUtil::Rotate(polyId, ptCenter, rotation);
	return polyId;
}

AcDbObjectId CPolylineUtil::AddPolyRectangle(const AcGePoint2d & pt1, const AcGePoint2d & pt2, double width)
{
	//提取两个角点的坐标值
	double x1 = pt1.x, x2 = pt2.x;
	double y1 = pt1.y, y2 = pt2.y;

	//计算矩形的角点
	AcGePoint2d ptLeftBottom(min(x1, x2), min(y1, y2));
	AcGePoint2d ptRightBottm(max(x1, x2), min(y1, y2));
	AcGePoint2d ptRightTop(max(x1, x2), max(y1, y2));
	AcGePoint2d ptLeftTop(min(x1, x2), max(y1, y2));

	//创建对应的多段线
	AcDbPolyline *pPoly = new AcDbPolyline(4);
	pPoly->addVertexAt(0, ptLeftBottom, 0, width, width);
	pPoly->addVertexAt(1, ptRightBottm, 0, width, width);
	pPoly->addVertexAt(2, ptRightTop, 0, width, width);
	pPoly->addVertexAt(3, ptLeftTop, 0, width, width);
	pPoly->setClosed(Adesk::kTrue);

	//将多段线添加到模型空间
	return CDwgDatabaseUtil::PostToModelSpace(pPoly);
}

AcDbObjectId CPolylineUtil::AddPolyCircle(const AcGePoint2d & ptCenter, double radius, double width)
{
	//计算顶点的位置
	AcGePoint2d pt1, pt2, pt3;
	pt1.x = ptCenter.x + radius;
	pt1.y = ptCenter.y;
	pt2.x = ptCenter.x - radius;
	pt2.y = ptCenter.y;
	pt3.x = ptCenter.x + radius;
	pt3.y = ptCenter.y;

	//创建多段线
	AcDbPolyline *pPoly = new AcDbPolyline(3);
	pPoly->addVertexAt(0, pt1, 1, width, width);
	pPoly->addVertexAt(1, pt2, 1, width, width);
	pPoly->addVertexAt(2, pt3, 1, width, width);
	pPoly->setClosed(Adesk::kTrue);

	//将闭合多段线添加到模型空间
	return CDwgDatabaseUtil::PostToModelSpace(pPoly);
}

AcDbObjectId CPolylineUtil::AddPolyArc(const AcGePoint2d & ptCenter, double radius, double angleStart, double angleEnd, double width)
{
	//计算顶点的位置
	AcGePoint2d pt1, pt2;
	pt1.x = ptCenter.x + radius*cos(angleStart);
	pt1.y = ptCenter.y + radius*sin(angleStart);
	pt2.x = ptCenter.x + radius*cos(angleEnd);
	pt2.y = ptCenter.y + radius*sin(angleEnd);

	//创建多段线
	AcDbPolyline *pPoly = new AcDbPolyline(3);
	pPoly->addVertexAt(0, pt1, tan((angleEnd - angleStart) / 4), width, width);
	pPoly->addVertexAt(1, pt2, 0, width, width);

	//将多段线添加到模型空间
	return CDwgDatabaseUtil::PostToModelSpace(pPoly);
}
