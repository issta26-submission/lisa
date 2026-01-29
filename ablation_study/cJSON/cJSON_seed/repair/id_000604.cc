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
//<ID> 604
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
    cJSON *n0 = cJSON_CreateNumber(4.0);
    cJSON *n1 = cJSON_CreateNumber(8.0);
    cJSON *n2 = cJSON_CreateNumber(12.0);
    cJSON_AddItemToArray(data, n0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.5);
    cJSON_AddStringToObject(meta, "label", "initial");
    cJSON_AddRawToObject(root, "config_raw", "{\"threshold\":7,\"mode\":\"auto\"}");

    // step 2: Configure
    int count = cJSON_GetArraySize(data);
    cJSON *a0 = cJSON_GetArrayItem(data, 0);
    cJSON *a1 = cJSON_GetArrayItem(data, 1);
    cJSON *a2 = cJSON_GetArrayItem(data, 2);
    double s0 = cJSON_GetNumberValue(a0);
    double s1 = cJSON_GetNumberValue(a1);
    double s2 = cJSON_GetNumberValue(a2);
    double sum = s0 + s1 + s2;
    double average = sum / (double)count;
    cJSON_AddNumberToObject(root, "average", average);

    // step 3: Operate and Validate
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_meta, "version", 2.0);
    cJSON_AddStringToObject(new_meta, "label", "replaced");
    cJSON_ReplaceItemInObject(root, "meta", new_meta);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "root_is_object_flag", (double)root_is_obj);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}