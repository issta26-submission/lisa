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
//<ID> 845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *placeholder = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "replaced", placeholder);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    double set_val = cJSON_SetNumberHelper(num_item, 3.14159);
    (void)set_val;
    cJSON_AddItemToArray(arr, num_item);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "name", "cJSON_example");
    cJSON *dup_obj = cJSON_Duplicate(obj, 1);
    cJSON_AddItemToArray(arr, dup_obj);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replaced", replacement);

    // step 3: Operate
    char *compact = cJSON_PrintUnformatted(root);
    size_t compact_len = strlen(compact);
    char *minified = (char *)cJSON_malloc(compact_len + 1);
    memcpy(minified, compact, compact_len + 1);
    cJSON_Minify(minified);

    // step 4: Validate & Cleanup
    int count = cJSON_GetArraySize(arr);
    (void)count;
    cJSON_Delete(obj);
    cJSON_free(compact);
    cJSON_free(minified);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}