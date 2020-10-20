#pragma once


using namespace System;
using namespace Autodesk::AutoCAD::Geometry;
using namespace Autodesk::AutoCAD::DatabaseServices;

class ZfgkTriangle;

namespace zfgk
{
	namespace TrangleEntityDB
	{
		//定义一个Entity继承的托管类，其中封装了zfgkTriangle类的多个属性
		[Autodesk::AutoCAD::Runtime::Wrapper("zfgkTriangle")]
		public __gc class MgTriangle:public Autodesk::AutoCAD::DatabaseServices::Entity
		{
		public:
			MgTriangle(void);
			MgTriangle(Point3d pt1,Point3d pt2,Point3d pt3);

		public private:
			MgTriangle(System::IntPtr unmanagedPointer,bool autoDelete);

			//获取肥托管实体指针
			inline ZfgkTriangle * GetImpObj()
			{
				//UnmangedObject() 方法是一个AutoCAD托管包装方法，他提供一中直接访问VC++中ARX实体方法
				return static_cast<ZfgkTriangle*>(UnmanagedObject.ToPointer());
			}

		public:
			//获得所有顶点
			void GetVerts([Runtime::InteropServices::Out] Autodesk::AutoCAD::Geometry::Point3dCollection *&verts);

			//设置某个顶点
			void SetVertAt(int index,Point3d point);

			//三角形面积
			__property double get_Area();

		};
	}
}

