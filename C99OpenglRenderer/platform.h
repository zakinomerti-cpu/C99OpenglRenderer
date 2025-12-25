#ifndef PLATFORMCLASS_H
#define PLATFORMCLASS_H

typedef struct Platform {
	void (*PlatformInit)(struct Platform*);
	void (*render)(struct Platform*);
	void (*setStartFunc)(struct Platform*, void (*)(void));
	void (*setDisplayFunc)(struct Platform*, void (*)(void));
	unsigned char (*shouldClose)(struct Platform*);

	void (*shutDown)(struct Platform*);
	void (*setShutDownFunc)(struct Platform*, void(*)(void));
	void (*createWindow)(struct Platform*, const char*, int, int);

	//private
	void(*startFunc)(void);

} Platform;

Platform* GlutPlatform_new(int, char**);

#endif