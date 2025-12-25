#ifndef COMPONENTCLASS_H
#define COMPONENTCLASS_H

typedef struct Entity Entity;

typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;

typedef struct Component {
	void (*Init)(struct Component*);
	void (*Use)(struct Component*);
	void (*addChild)(struct Component*, struct Component*);
	void (*removeChild)(struct Component*, struct Component*);

	struct Component* parentCmp;
	Entity* parentEntity;

	int size;
	int capacity;
	struct Component** child;

	void* InData;
	void** LocData;
} Component;

Component* MeshComponent_new(Entity*, Mesh*);
Component* ShaderComponent_new(Entity*, Shader*);
Component* TextureComponent_new(Entity*, Shader*);
Component* RenderComponent_new(Entity*);

#endif