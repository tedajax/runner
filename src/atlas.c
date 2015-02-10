#include "atlas.h"
#include "snprintf.h"
#include <mxml.h>

void atlas_free_void(void* pself);

Hashtable atlasTable;
char rootDir[128];

void atlases_init() {
    hashtable_init(&atlasTable, 64, atlas_free_void);
    strcpy(rootDir, "assets/textures");
}

void atlases_terminate() {
    hashtable_free_contents(&atlasTable);
}

void atlas_free(Atlas* self) {
    hashtable_free_contents(&self->frameTable);
}

void atlas_free_void(void* pself) {
    Atlas* self = (Atlas*)pself;
    atlas_free(self);
    free(self);
}

int get_attr_int(mxml_node_t* node, const char* name) {
    char* v = mxmlElementGetAttr(node, name);
    if (v) {
        return strtol(v, NULL, 10);
    }
    return 0;
}

Rect get_sprite_frame(mxml_node_t* spriteNode) {
    Rect result;

    result.position.x = (f32)get_attr_int(spriteNode, "x");
    result.position.y = (f32)get_attr_int(spriteNode, "y");
    result.width = (f32)get_attr_int(spriteNode, "w");
    result.height = (f32)get_attr_int(spriteNode, "h");

    return result;
}

Vec2 get_sprite_offset(mxml_node_t* spriteNode) {
    Vec2 result;

    result.x = (f32)get_attr_int(spriteNode, "oX");
    result.y = (f32)get_attr_int(spriteNode, "oY");

    return result;
}

void atlas_load_data(Atlas* self, const char* dataFile) {
    FILE* pdata = fopen(dataFile, "rb");
    ASSERT(pdata, "Unable to load data file.");
    mxml_node_t* root = mxmlLoadFile(NULL, pdata, MXML_TEXT_CALLBACK);
    mxml_node_t* atlasRoot = mxmlFindElement(root, root, "TextureAtlas", NULL, NULL, MXML_DESCEND);

    char* textureName = mxmlElementGetAttr(atlasRoot, "imagePath");
    ASSERT(textureName, "Could not find texture name in atlas.");
    textures_load(textureName);
    self->texture = textures_get(textureName);
    ASSERT(self->texture, "Failed to load texture specified in atlas.");

    mxml_node_t* spriteNode = atlasRoot->child->next;
    while (spriteNode) {
        char* name = mxmlElementGetAttr(spriteNode, "n");
        ASSERT(name, "Failed to parse name.");

        SpriteFrame* spriteFrame = CALLOC(1, SpriteFrame);
        spriteFrame->frame = get_sprite_frame(spriteNode);
        spriteFrame->offset = get_sprite_offset(spriteNode);
        
        hashtable_insert(&self->frameTable, name, spriteFrame);

        spriteNode = spriteNode->next->next;
    }

    mxmlRelease(root);
    fclose(pdata);
}

void atlas_load(const char* name) {
    Atlas* self = CALLOC(1, Atlas);
    
    hashtable_init(&self->frameTable, 32, free);

    char dataFile[256];
    snprintf(dataFile, 256, "%s/%s.xml", rootDir, name);
    atlas_load_data(self, dataFile);

    hashtable_insert(&atlasTable, name, self);
}

void atlas_unload(const char* name) {
    hashtable_remove(&atlasTable, name);
}

Atlas* atlas_get(const char* name) {
    Atlas* result = (Atlas*)hashtable_get(&atlasTable, name);
    ASSERT(result, "Could not find atlas.");
    return result;
}

SpriteFrame* atlas_get_frame(Atlas* self, const char* spriteName) {
    return (SpriteFrame*)hashtable_get(&self->frameTable, spriteName);
}
