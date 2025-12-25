#ifndef COMPONENTCLASS_H
#define COMPONENTCLASS_H

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

	void* InData;
	dataArr* child;
	dataArr* LocData;
} Component;

Component* MeshComponent_new(Component* prnt, Entity* ent, Mesh* mesh);
Component* ShaderComponent_new(Entity*, Shader*);
Component* TextureComponent_new(Entity*, Texture*);
Component* RenderComponent_new(Entity*);

#endif