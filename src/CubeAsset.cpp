#include "CubeAsset.h"

CubeAsset::CubeAsset()

{
  CubeAsset(0, 0, 0, 0, "shaders/hello-gl.f.glsl");
}

CubeAsset::CubeAsset(float x, float y, float z, float size, string shader) : GameAsset(
	      string("shaders/hello-gl.v.glsl")
	      , string(shader)
	      ){
  this->li = nullptr;
  this->pos = shared_ptr<Point3>(new Point3(x, y, z));
  // A default "unit" triangular pyramid
  num_vertices = 8;
  num_triangles = 12;
  g_vertex_buffer_data = new GLfloat[num_vertices * 3]{
//	   X,  Y,  Z,
	  -size, -size, -3, //0
	   size, -size, -3, //1
	   size,  size, -3, //2
	  -size,  size, -3, //3
	  -size, -size,  3, //4
	   size, -size,  3, //5
	   size,  size,  3, //6
	  -size,  size,  3  //7

  }; // three points per vertex
  g_element_buffer_data = new GLushort[num_triangles * 3]{
	  //linking anti clockwise
	  0,1,2,
	  2,3,0, //
	  0,3,4,
	  4,7,3, //
	  1,5,2,
	  2,5,6, //
	  0,4,5,
	  5,0,1, //
	  7,4,5,
	  5,6,7, //
	  3,2,7,
	  2,7,6  //

  }; // three vertices per triangle


  mv_matrix = mv_matrix.translation( Vector3(x, y, z));
  bbox.reset();
  bbox = shared_ptr<BoundingBox>(new BoundingBox(Point3(x, y, z), size*2, size*2, 3*2));

  make_resources();
}

CubeAsset::~CubeAsset() {
  // TODO: do something nice here.
}

//moveLeft

void CubeAsset::update() {
	//this->pos = this->pos + this->vector;
	//Vector3 vec2 = li->update();
	//this->pos += vec2;
	//shared_ptr<Point3>cubePos=shared_ptr<Point3>(new Point3(this->pos));
	//Vector3 vecPos(this->pos);
//	mv_matrix = Matrix4::identity();
	////////////////////////////////////////////////this->vecPos(this->pos);
	//cast point3 to vector3 then to matrix 4
	//new vector3(point3)
	Vector3 * vecPos = new Vector3(*pos);
	//Vector3 * vPos = new Vector3(*pos);

	if(nullptr!=li){
		Vector3 vec = li->update();
		vec.setX(vec.getX()+vecPos->getX());
		vec.setY(vec.getY()+vecPos->getY());
		vec.setZ(vec.getZ()+vecPos->getZ());
		mv_matrix = mv_matrix.translation(vec);

	}

}

void CubeAsset::setInterpolator(shared_ptr<IInterpolator> li) {
  this->li.swap(li);
}

void CubeAsset::draw() {
  GameAsset::draw();
}

/*
CubeAsset::move(<shared_ptr> a, int b){
CubeAsset(0.0,1.0,0.0)

}
*/

