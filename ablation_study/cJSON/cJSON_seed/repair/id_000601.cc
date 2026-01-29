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
//<ID> 601
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
    cJSON *n0 = cJSON_CreateNumber(4.0);
    cJSON *n1 = cJSON_CreateNumber(8.0);
    cJSON *n2 = cJSON_CreateNumber(12.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.5);

    // step 2: Configure
    const char *raw_json = "{\"note\":\"raw data\",\"flag\":true}";
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", raw_json);
    int count = cJSON_GetArraySize(items);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON *replacement_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement_meta, "average", average);
    cJSON_bool raw_is_obj = cJSON_IsObject(raw_item);
    cJSON_AddBoolToObject(replacement_meta, "raw_was_object", raw_is_obj);
    cJSON_AddNumberToObject(replacement_meta, "count", (double)count);
    cJSON_ReplaceItemInObject(root, "meta", replacement_meta);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}