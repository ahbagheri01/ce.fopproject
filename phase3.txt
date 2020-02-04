 /** the phase3 but not complete */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#define VALUE 1000
#define NAME 1000
#define  TXT 1000
#define MAX 1000
/**
*this cJSON struct is the main object that is holding our information
valuestring holds the string of the object and the name is type
and the text is the content and the children is the holder for
array of cJSON and its size is size of array
*/
typedef struct cJSON
{
    char *valuestring;
    char name[MAX][MAX];
    char text[MAX][MAX];
    struct cJSON *children[MAX];
    int size_of_children;
} cJSON;
int size_of_cjson = sizeof(cJSON);
/**
* this function will allocate in memory as needed for saving the informations safely
and due to there is no array still size_of_children is going to be zero
*/
struct cJSON * cJSON_CreateObject()
{
    cJSON * object = calloc(1,size_of_cjson);
    for(size_t i = 0 ; i < MAX ; i++){
        object->children[i] = NULL;
    }
    object->size_of_children = 0;
    return object;
}
/**
*
this will put the string in the value string of appropriate node of cJSON
it will allocate for valuestring and then copy it to value and note that string is constant because it shouldn't be changed
*/
cJSON *cJSON_CreateString(const char *string)
{
    cJSON *object = cJSON_CreateObject();
    object->valuestring = calloc(1,VALUE);
    strcpy(object->valuestring, string);
    return object;
}
/**
*this is like the object at first but then the things are added to array
*/
cJSON *cJSON_CreateArray()
{
    cJSON *object = cJSON_CreateObject();
    return object;
}
/**
* as you see down the child of struct is now not NULL and it refer to somewhere
and the size is +ed
*/
void cJSON_AddItemToArray(cJSON *Array,cJSON *Item)
{
    Array->children[Array->size_of_children] = Item;
    (Array->size_of_children)++;
    return;
}
cJSON *cJSON_GetObjectItem(cJSON *object, char *named_item)
{
    cJSON *inside = (cJSON *) calloc(1, size_of_cjson);
    for (size_t i = 0; i < MAX; i++) {
        if (strcmp(inside->name[i], named_item)== 0) {
            strcpy(inside->valuestring, object->text[i]);
            return inside;
        }
    }
}

cJSON *cJSON_GetArrayItem(cJSON *Array, int i) {
    cJSON *object = (cJSON *) calloc(1, size_of_cjson);
    if (i < Array->size_of_children)
        return NULL ;
    object = Array->children[i];
    return object;
}
cJSON *cJSON_AddItemToObject(cJSON *object,const char *txt, cJSON *Item) {
    if (Item->children[0] == NULL)    // if its array or not
    {
        for (size_t i = 0; i < MAX; i++) {
            if (strcmp(object->name[i], "")==0)
            {
                strcpy(object->name[i], txt);
                strcpy(object->text[i], Item->valuestring);
                return object;
            }
        }
    }
    return NULL;
}
char *cJSON_PrintUnformatted(cJSON *object)
{
    if (object->children[0] ==NULL)
    {
        char *pack = (char *) calloc(MAX, sizeof(char));
        strcat(pack,"{");
        for (size_t i = 0; i < MAX; i++) {
            if (strcmp(object->name[i], "")) {
                strcat(pack, "\"");
                strcat(pack, object->name[i]);
                strcat(pack, "\"");
                if (strcmp(object->text[i], "")) {
                    strcat(pack, ":\"");
                    strcat(pack, object->text[i]);
                    strcat(pack, "\"");
                }
                if (strcmp(object->name[i + 1], ""))
                    strcat(pack, ",");
            }
        }
        strcat(pack,"}");
        return pack;
    } else
    {
        char *pack = (char *) calloc(MAX, sizeof(char));
        strcat(pack, "{\"");
        strcat(pack, object->name[0]);
        strcat(pack, "\":\"");
        strcat(pack, object->text[0]);
        strcat(pack, "\"");
        strcat(pack, ",");
        strcat(pack, "\"");
        strcat(pack, object->name[1]);
        strcat(pack, "\":");
        strcat(pack, "[");
        for (size_t i = 0;i < object->size_of_children; i++)
        {
            cJSON *tmp = object->children[i];
            if (strcmp(tmp->valuestring, "")) {
                strcat(pack, "\"");
                strcat(pack, tmp->valuestring);
                strcat(pack, "\"");
                if (i != object->size_of_children - 1) {
                    cJSON *check = object->children[i + 1];
                    if (strcmp(check->valuestring, ""))
                        strcat(pack, ",");
                }
            }
            if (i > 0 && strcmp(tmp->name[0],""))
                strcat(pack, "{");
            for (size_t j = 0; j < MAX; j++) {
                if (strcmp(tmp->name[j], "")) {
                    strcat(pack, "\"");
                    strcat(pack, tmp->name[j]);
                    strcat(pack, "\":\"");
                    strcat(pack, tmp->text[j]);
                    strcat(pack, "\"");
                    if (strcmp(tmp->name[j + 1], ""))
                        strcat(pack, ",");
                }
            }
            if (i > 0)
                strcat(pack, "}");
            if (i < object->size_of_children - 1 && i > 0 && strcmp(tmp->name[0], ""))
                strcat(pack, ",");
        }
        strcat(pack, "]}");
        return pack;
    }
}
