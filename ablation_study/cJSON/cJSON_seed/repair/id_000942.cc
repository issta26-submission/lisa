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
//<ID> 942
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("first_item");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(items, 0, first);
    cJSON_InsertItemInArray(items, 1, num);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("unit_test_node");
    cJSON_AddItemToObject(meta, "name", name);

    // step 3: Operate and Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool identical = cJSON_Compare(root, dup, 1);
    cJSON_AddNumberToObject(root, "meta_is_object", (double)meta_is_obj);
    cJSON_AddNumberToObject(root, "structure_identical", (double)identical);
    cJSON *queried_name = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *queried_name_str = cJSON_GetStringValue(queried_name);
    cJSON_AddStringToObject(root, "name_copy", queried_name_str);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(dup, buffer, buflen, 1);
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}