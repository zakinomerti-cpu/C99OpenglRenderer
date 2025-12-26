#ifndef COMPONENTCLASS_H
#define COMPONENTCLASS_H

typedef enum ComponentVTable {
	CMP_CUSTOM,
	CMP_SHADER,
	CMP_TEXTURE,
	CMP_MESH,
	CMP_SCRIPT,
	CMP_CAMERA,
} ComponentVTable;

typedef struct Entity Entity;
typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;

typedef struct dataArr dataArr;

typedef struct Component {
	void (*Init)(struct Component*);
	void (*Bind)(struct Component*);
	void (*UnBind)(struct Component*);
	void (*AddChild)(struct Component*, struct Component*);
	void (*RemoveChild)(struct Component*, struct Component*);
	void (*DeleteComponent)(struct Component*);

	struct Component* parentCmp;
	Entity* parentEntity;

	dataArr* InData;
	dataArr* child;
	dataArr* LocData;
} Component;

Component* MeshComponent_new(Component* prnt, Entity* ent, dataArr* InData);
Component* ShaderComponent_new(Component* prnt, Entity* ent, dataArr* InData);
Component* TextureComponent_new(Component* prnt, Entity* ent, dataArr* InData);
Component* RenderComponent_new(Component* prnt, Entity* ent, dataArr* InData);

#endif