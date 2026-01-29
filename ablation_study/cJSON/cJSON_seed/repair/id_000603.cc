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
//<ID> 603
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *n0 = cJSON_CreateNumber(2.0);
    cJSON *n1 = cJSON_CreateNumber(4.0);
    cJSON *n2 = cJSON_CreateNumber(6.0);
    cJSON_AddItemToArray(data, n0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "label", "batch_A");
    cJSON_AddNumberToObject(meta, "version", 1.5);

    // step 2: Configure
    int count = cJSON_GetArraySize(data);
    cJSON *a0 = cJSON_GetArrayItem(data, 0);
    cJSON *a1 = cJSON_GetArrayItem(data, 1);
    cJSON *a2 = cJSON_GetArrayItem(data, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    double multiplier = 1.0 + (double)meta_is_obj;

    // step 3: Operate and Validate
    cJSON_AddNumberToObject(root, "average", average * multiplier);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_info", "{\"flag\":true,\"num\":42}");
    cJSON_bool raw_is_obj = cJSON_IsObject(raw_item);
    cJSON_AddNumberToObject(root, "raw_is_object", (double)raw_is_obj);
    cJSON *replacement = cJSON_CreateString("replaced_meta");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInObject(root, "meta", replacement);
    cJSON_AddBoolToObject(root, "replace_succeeded", replaced_ok);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}