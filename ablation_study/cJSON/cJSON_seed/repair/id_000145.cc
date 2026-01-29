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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "example");

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", flag);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s0);
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    const char *copied_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "copied_name", copied_name);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    const char *v0 = cJSON_GetStringValue(first);
    const char *v1 = cJSON_GetStringValue(second);
    (void)v0;
    (void)v1;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}