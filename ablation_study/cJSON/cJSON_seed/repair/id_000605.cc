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
//<ID> 605
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
    cJSON *n0 = cJSON_CreateNumber(8.0);
    cJSON *n1 = cJSON_CreateNumber(12.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddRawToObject(meta, "raw_config", "{\"enabled\":true,\"scale\":2}");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_obj);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    double s0 = cJSON_GetNumberValue(a0);
    double s1 = cJSON_GetNumberValue(a1);
    double s2 = cJSON_GetNumberValue(a2);
    double sum = s0 + s1 + s2;
    double average = sum / (double)count;
    cJSON *avg_item = cJSON_CreateNumber(average);
    cJSON_AddItemToObject(root, "average", avg_item);
    cJSON *avg_double = cJSON_CreateNumber(average * 2.0);
    cJSON_ReplaceItemInObject(root, "average", avg_double);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}