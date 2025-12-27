#ifndef COMPONENTCLASS_H
#define COMPONENTCLASS_H

typedef struct Component Component;

typedef struct Entity Entity;
typedef struct dataArr dataArr;
typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Mesh Mesh;

typedef enum ComponentTagTable {
	CMP_CUSTOM = 1010,
	CMP_SHADER = 1011,
	CMP_TEXENT = 1012,
	CMP_RENDER = 1013,
	CMP_MSHENT = 1014,
	CMP_SCRIPT = 1015,
	CMP_CAMERA = 1016,
	CMP_GUIENT = 1017,
	CMP_LGCENT = 1018,
} ComponentTagTable;

typedef struct Component {
	ComponentTagTable TagValue;
	void (*Init)(struct Component*);
	void (*Bind)(struct Component*);
	void (*UnBind)(struct Component*);
	void (*DeleteComponent)(struct Component*);
	Entity* parentEntity;

	void (*AddChild)(struct Component*, struct Component*);

	dataArr* InData;
	dataArr* LocData;
	dataArr* Object;
	dataArr* child;

	int isReady;
	int chdCount;
	int SelectedObjet;

} Component;

Component* MeshComponent_new(Component* prnt, Entity* ent, dataArr* InData);
Component* ShaderComponent_new(Component* prnt, Entity* ent, dataArr* InData);
Component* TextureComponent_new(Component* prnt, Entity* ent, dataArr* InData);
Component* RenderComponent_new(Component* prnt, Entity* ent, dataArr* InData);

#endif