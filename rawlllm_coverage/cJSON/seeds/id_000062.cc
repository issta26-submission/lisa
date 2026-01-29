#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API usages:
    // - create objects/arrays/primitives
    // - add items to objects/arrays
    // - duplicate a tree
    // - serialize to string
    // - parse JSON string back
    // - inspect and validate values
    // - cleanup all allocated memory
    //
    // Returns 66 on success, non-66 on failure.

    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *dup = NULL;
    cJSON *parsed = NULL;
    char *s_root = NULL;
    char *s_dup = NULL;
    int ret = -1;

    // 1) Create root object
    root = cJSON_CreateObject();
    if (!root) goto done;

    // 2) Add some properties: string and number
    if (!cJSON_AddStringToObject(root, "name", "cJSON Test")) goto done;
    if (!cJSON_AddNumberToObject(root, "version", 1.7)) goto done;

    // 3) Create an array and populate it
    items = cJSON_CreateArray();
    if (!items) goto done;
    if (!cJSON_AddItemToArray(items, cJSON_CreateString("one"))) goto done;
    if (!cJSON_AddItemToArray(items, cJSON_CreateNumber(2))) goto done;
    if (!cJSON_AddItemToArray(items, cJSON_CreateFalse())) goto done;

    // 4) Attach array to root
    cJSON_AddItemToObject(root, "items", items); // ownership of items transferred to root
    items = NULL; // avoid double-delete

    // 5) Duplicate the whole tree (deep copy)
    dup = cJSON_Duplicate(root, 1); // recursive = 1
    if (!dup) goto done;

    // 6) Modify the duplicate: bump version and add a boolean
    {
        cJSON *ver = cJSON_GetObjectItemCaseSensitive(dup, "version");
        if (ver && cJSON_IsNumber(ver)) {
            ver->valuedouble = 2.0;
            ver->valueint = 2;
        } else {
            goto done;
        }
    }
    if (!cJSON_AddTrueToObject(dup, "ok")) goto done;

    // 7) Serialize both original and duplicate (compact)
    s_root = cJSON_PrintUnformatted(root);
    s_dup  = cJSON_PrintUnformatted(dup);
    if (!s_root || !s_dup) goto done;

    // 8) Parse the serialized original back into an object
    parsed = cJSON_Parse(s_root);
    if (!parsed) goto done;

    // 9) Validate fields in parsed object
    {
        cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (!pname || !cJSON_IsString(pname) || !pname->valuestring) goto done;
        // check string content
        if (strcmp(pname->valuestring, "cJSON Test") != 0) goto done;

        cJSON *parr = cJSON_GetObjectItemCaseSensitive(parsed, "items");
        if (!parr || !cJSON_IsArray(parr)) goto done;
        int asz = cJSON_GetArraySize(parr);
        if (asz != 3) goto done;
        // check first array element is string "one"
        cJSON *a0 = cJSON_GetArrayItem(parr, 0);
        if (!a0 || !cJSON_IsString(a0) || strcmp(a0->valuestring, "one") != 0) goto done;
        // check second is number 2
        cJSON *a1 = cJSON_GetArrayItem(parr, 1);
        if (!a1 || !cJSON_IsNumber(a1) || a1->valueint != 2) goto done;
        // check third is false
        cJSON *a2 = cJSON_GetArrayItem(parr, 2);
        if (!a2 || cJSON_IsTrue(a2)) goto done; // expect false
    }

    // If we reached here all checks passed
    ret = 66;

done:
    // Cleanup: free serialized strings and cJSON trees
    if (s_root) { free(s_root); s_root = NULL; }
    if (s_dup)  { free(s_dup);  s_dup  = NULL; }
    if (root)   { cJSON_Delete(root);   root = NULL; }
    if (dup)    { cJSON_Delete(dup);    dup = NULL; }
    if (parsed) { cJSON_Delete(parsed); parsed = NULL; }
    if (items)  { cJSON_Delete(items);  items = NULL; } // in case not attached

    return ret;
}