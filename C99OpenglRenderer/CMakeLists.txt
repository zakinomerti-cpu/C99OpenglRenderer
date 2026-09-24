cmake_minimum_required(VERSION 4.0)

project(C99OGL)

include (FetchContent)
FetchContent_Declare(
	noownContainers
	GIT_REPOSITORY	"https://github.com/zakinomerti-cpu/nonowning_containers"
	GIT_TAG			main
)
FetchContent_MakeAvailable(noownContainers)

add_executable(myapp
	main.c
	platform.c
	MeshObjects.c
	DynamicString.c
	Texture.c
	Render.c
	EntityCube.c
	Scene.c
	Shader.c
	Mesh.c
)

target_link_libraries(myapp PRIVATE GLEW glut GL GLU m noownContainers)

