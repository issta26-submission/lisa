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
//<ID> 1088
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"meta\":\"ok\"}";
    size_t json_len = sizeof(json) - 1;
    const char * parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *merged = cJSON_AddObjectToObject(root, "merged");

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    int original_count = cJSON_GetArraySize(items);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(merged, "items_copy", dup_items);
    cJSON *flag = cJSON_AddFalseToObject(root, "valid");
    (void)flag;

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_raw = cJSON_IsRaw(first);
    int dup_count = cJSON_GetArraySize(dup_items);
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    (void)first_is_raw;
    (void)original_count;
    (void)dup_count;
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}