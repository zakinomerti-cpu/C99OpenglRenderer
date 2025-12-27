#ifndef COMPONENTCLASS_H
#define COMPONENTCLASS_H

typedef struct SYSTEM SYSTEM;
typedef struct Component Component;
typedef struct dataArr dataArr;
typedef struct Texture Texture;
typedef struct Shader Shader;
typedef struct Entity Entity;
typedef struct Mesh Mesh;

typedef enum ComponentTagTable {
	CMP_CUSTOM = 1011,
	CMP_SHDOBJ = 1012,
	CMP_TEXOBJ = 1013,
	CMP_RNDOBJ = 1014,
	CMP_MSHOBJ = 1015,
	CMP_SCRIPT = 1016,
	CMP_CAMOBJ = 1017,
	CMP_GUIOBJ = 1018,
	CMP_LGCOBJ = 1019,
} ComponentTagTable;

// один компонент, один тип 
// ресурса! не пихай все в один
typedef struct Component {
	char* m_pName;
	ComponentTagTable TagValue;
	void (*Init)(struct Component*);
	void (*Bind)(struct Component*);
	void (*UnBind)(struct Component*);
	void (*SystemCreateEvent)(struct Component*, SYSTEM*);
	void (*SystemUpdateEvent)(struct Component*, SYSTEM*);
	void (*AddObject)(struct Component*, dataArr* InData);
	void (*AddChild)(struct Component*, struct Component*);
	void (*RemoveChild)(struct Component*, int index);

	void (*SelectObject)(struct Component*, char* objName);
	void (*GetChdCount)(struct Component*);
	dataArr* m_Object;

	// private Block
	Entity* m_pParentEntity;
	dataArr* m_InData;
	dataArr* m_LocData;
	dataArr* m_child;

	int m_iIsReady;

	//если m_bChdCount = -1 то
	//компонент не может иметь детей
	//...
	//блять, да не в этом смысле...
	int m_bChdCount;
	int m_iSelectedObjet;

} Component;

//имя копируется внутрь обьекта
//InData при добавлении: arr[0] - vertices(float*), arr[1] - vertexCount(int) 
//arr[2] - indices(unsigned char*), arr[3] - indexCount(int)
Component* MeshComponent_new(Component* prnt, Entity* ent, char* CmpName);
void DeleteMeshComponent(Component* cmp);

Component* ShaderComponent_new(Component* prnt, Entity* ent, char* CmpName);
void DeleteShaderComponent(Component* cmp);

Component* TextureComponent_new(Component* prnt, Entity* ent, char* CmpName);
void DeleteTextureComponent(Component* cmp);

Component* RenderComponent_new(Component* prnt, Entity* ent, char* CmpName);
void DeleteRenderComponent(Component* cmp);

#endif