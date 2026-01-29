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
//<ID> 600
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(4.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(6.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 2.0);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_blob", "{\"ok\":true,\"n\":3}");
    cJSON_bool meta_is_object = cJSON_IsObject(meta);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_meta, "version", 2.0);
    cJSON_AddNumberToObject(new_meta, "average", average);
    cJSON_AddNumberToObject(new_meta, "meta_was_object", (double)meta_is_object);
    cJSON_ReplaceItemInObject(root, "meta", new_meta);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}