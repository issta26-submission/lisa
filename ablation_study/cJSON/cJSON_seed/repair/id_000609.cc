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
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "label", "initial");
    cJSON *raw = cJSON_AddRawToObject(meta, "info", "{\"x\":42,\"y\":[1,2]}");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *new_label = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(meta, "label", new_label);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(values);
    cJSON *a0 = cJSON_GetArrayItem(values, 0);
    cJSON *a1 = cJSON_GetArrayItem(values, 1);
    cJSON *a2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_AddNumberToObject(root, "average", average);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_obj);
    cJSON_bool raw_is_obj = cJSON_IsObject(raw);
    cJSON_AddBoolToObject(root, "raw_is_object", raw_is_obj);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}