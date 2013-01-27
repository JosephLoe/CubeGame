#include "WallAsset.h"

WallAsset::WallAsset()

{
  WallAsset(0, 0, 0, 0, 0, 0, "LR","shaders/white.f.glsl");
}

WallAsset::WallAsset(float x, float y, float z, float wallX, float wallY, float wallZ, string(side), string(shader)) : GameAsset(
	      string("shaders/hello-gl.v.glsl")
	      , string(shader)
	      ){
  this->li = nullptr;
  this->pos = shared_ptr<Point3>(new Point3(x, y, z));
  // A default "unit" triangular pyramid
  num_vertices = 4;
  num_triangles = 2;
  if(side=="LR"){
  g_vertex_buffer_data = new GLfloat[num_vertices * 3]{
//	   X,  Y,  Z,
	   wallX, -wallY, -wallZ, //0
	   wallX, -wallY,  wallZ, //1
	   wallX,  wallY,  wallZ, //2
	   wallX,  wallY, -wallZ //3

  }; // three points per vertex
  g_element_buffer_data = new GLushort[num_triangles * 3]{
	  //linking anti clockwise
	  0,1,2,
	  2,3,0 //

  }; // three vertices per triangle
  bbox.reset();
  bbox = shared_ptr<BoundingBox>(new BoundingBox(Point3(x, y, z), 1, wallY, wallZ));
  }else if(side=="TB"){
	  g_vertex_buffer_data = new GLfloat[num_vertices * 3]{
	  //	   X,  Y,  Z,
	  	  -wallX,  wallY, -wallZ, //0
	  	  -wallX,  wallY,  wallZ, //1
	  	   wallX,  wallY,  wallZ, //2
	  	   wallX,  wallY, -wallZ //3

	    }; // three points per vertex
	    g_element_buffer_data = new GLushort[num_triangles * 3]{
	  	  //linking anti clockwise
	  	  0,1,2,
	  	  2,3,0 //

	    }; // three vertices per triangle
	    bbox.reset();
	    bbox = shared_ptr<BoundingBox>(new BoundingBox(Point3(x, y, z), wallX, 1, wallZ));
  } else{
	  cerr<< "Invalid string passed into WallAsset, Must be LR or TB" << endl;
  }

  mv_matrix = mv_matrix.translation( Vector3(x, y, z));

  make_resources();
}

WallAsset::~WallAsset() {
  // TODO: do something nice here.
}

void WallAsset::update() {

}

void WallAsset::setInterpolator(shared_ptr<IInterpolator> li) {
  this->li.swap(li);
}

void WallAsset::draw() {
  GameAsset::draw();
}

