#include "Gun.h"

Gun::Gun()
{
  Gun(0, 0, 0);
}

Gun::Gun(float x, float y, float z)   : GameAsset(
	      string("shaders/hello-gl.v.glsl")
	      , string("shaders/red.f.glsl")){
  this->li = nullptr;
  this->pos = shared_ptr<Point3>(new Point3(x, y, z));
  // A default "unit" triangular pyramid
  num_vertices = 8;
  num_triangles = 12;
  g_vertex_buffer_data = new GLfloat[num_vertices * 3]{
//	   X,  Y,  Z,
	  -2, -1, -1, //0
	   2, -1, -1, //1
	   2,  1, -1, //2
	  -2,  1, -1, //3
	  -2, -1,  1, //4
	   2, -1,  1, //5
	   2,  1,  1, //6
	  -2,  1,  1  //7

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
  bbox = shared_ptr<BoundingBox>(new BoundingBox(Point3(x, y, z), 1.0, 1.0, 1.0));

  make_resources();
}

Gun::~Gun() {
  // TODO: do something nice here.
}

void Gun::update() {
	//GameAsset::draw();
}

void Gun::setInterpolator(shared_ptr<IInterpolator> li) {
  this->li.swap(li);
}

void Gun::draw() {
  GameAsset::draw();
}

/*
Gun::move(<shared_ptr> a, int b){
Gun(0.0,1.0,0.0)

}
*/

