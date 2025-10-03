#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#define MAX_FILE_STRING_SEPARATIONS 4
#define DIMENSIONS 3
#define MAX_LINE_SIZE 512

#endif // FILEMANAGER_H_INCLUDED

void loadObjectsFromFile(char *fileLocation, SceneObject *sceneObjects, Player *player, int *qtdObjects, int max_objects);
