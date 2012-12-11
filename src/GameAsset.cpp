/*
 * GameObject.cpp
 *
 *  Created on: 03-Mar-2009
 *      Author: balor
 */

#include "GameAsset.h"

void GameAsset::common_init() {
  mv_matrix = Matrix4::identity();
  bbox = shared_ptr<BoundingBox>(new BoundingBox(Point3(0,0,0), 1.0, 1.0, 1.0)); // unit cube
}

GameAsset::GameAsset() {
  common_init();
  this->v_shader = "shaders/hello-gl.v.glsl";
  this->f_shader = "shaders/hello-gl.f.glsl";
}

GameAsset::GameAsset(const string & v_shader, const string & f_shader) {
  common_init();
  this->v_shader = v_shader;
  this->f_shader = f_shader;
}

GameAsset::~GameAsset() {
	// TODO Auto-generated destructor stub
}

bool GameAsset::collidesWith(GameAsset & a ) {
  return bbox->collidesWith((*a.bbox));
}

void GameAsset::draw() {
  glUseProgram(program);

  mv_matrix = Camera::getInstance().getCameraM() * mv_matrix;

  // horribe unpacking
  GLfloat foo [16] = {
    mv_matrix.getElem(0,0), mv_matrix.getElem(0,1), mv_matrix.getElem(0,2), mv_matrix.getElem(0,3),
    mv_matrix.getElem(1,0), mv_matrix.getElem(1,1), mv_matrix.getElem(1,2), mv_matrix.getElem(1,3),
    mv_matrix.getElem(2,0), mv_matrix.getElem(2,1), mv_matrix.getElem(2,2), mv_matrix.getElem(2,3),
    mv_matrix.getElem(3,0), mv_matrix.getElem(3,1), mv_matrix.getElem(3,2), mv_matrix.getElem(3,3)
  };
  glUniformMatrix4fv(mv_matrix_uniform, 1, false, foo);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(
			position_attrib,                  /* attribute */
			3,                                /* size */
			GL_FLOAT,                         /* type */
			GL_FALSE,                         /* normalized? */
			0,                                /* stride */
			(void*)0                          /* array buffer offset */
			);
  glEnableVertexAttribArray(position_attrib);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glDrawElements(
		 GL_TRIANGLES,
		 3 * this->num_triangles,
		 GL_UNSIGNED_SHORT,
		 (GLvoid*) 0
		 );
  
  glDisableVertexAttribArray(position_attrib);
}

/*
 * Functions for general purpose stuff
 */
GLchar * GameAsset::shader_file_contents(const string &filename, GLint * length)
{
  ifstream input_file;
  input_file.open(filename.c_str(), ios::in);

  input_file.seekg(0, ios::end);  // go to the end of the file
  *length = input_file.tellg();  // get length of the file
  input_file.seekg(0, ios::beg);  // go to beginning of the file

  GLchar * buffer = new GLchar[(*length)+1];
  input_file.read(buffer, *length);
  buffer[(*length)+1]='\0';  // Ensure null terminated

  input_file.close();
  return buffer;
}

/*
 * Functions for creating OpenGL objects:
 */
GLuint GameAsset::make_buffer(
    GLenum target,
    const void *buffer_data,
    GLsizei buffer_size
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

GLuint GameAsset::make_shader(GLenum type, const char *filename)
{
    GLint length;
    GLchar *source = shader_file_contents(filename, &length);
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    delete(source);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        cerr << "Failed to compile" << filename << " with error code " << shader_ok << endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint GameAsset::make_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;

    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        cerr<< "Failed to link shader program:" << endl;
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

/*
 * Load and create all of our resources:
 */
int GameAsset::make_resources(void)
{
    vertex_buffer = make_buffer(
        GL_ARRAY_BUFFER,
        g_vertex_buffer_data,
        3 * sizeof(GLfloat) * this->num_vertices
    );
    element_buffer = make_buffer(
        GL_ELEMENT_ARRAY_BUFFER,
        g_element_buffer_data,
        3 *  sizeof(GLushort) * this->num_triangles
    );

    vertex_shader = make_shader(
        GL_VERTEX_SHADER,
        this->v_shader.c_str()
    );
    if (vertex_shader == 0)
        return 0;

    fragment_shader = make_shader(
        GL_FRAGMENT_SHADER,
        this->f_shader.c_str()
    );
    if (fragment_shader == 0)
        return 0;
    
    program = make_program(vertex_shader, fragment_shader);
    if (program == 0)
        return 0;

    position_attrib = glGetAttribLocation(program, "position");
    mv_matrix_uniform = glGetUniformLocation(program, "mv_matrix");

    return 1;
}
