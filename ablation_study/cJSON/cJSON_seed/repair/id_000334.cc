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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON *it2 = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddStringToObject(meta, "name", "original");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_meta, "version", 2.0);
    cJSON_AddStringToObject(new_meta, "name", "replaced");
    cJSON_AddStringToObject(new_meta, "status", "ok");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *dup_first = cJSON_CreateString(cJSON_GetStringValue(first));
    cJSON_AddItemToArray(items, dup_first);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}