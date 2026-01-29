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
//<ID> 608
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
    cJSON *n0 = cJSON_CreateNumber(5.0);
    cJSON *n1 = cJSON_CreateNumber(15.0);
    cJSON *n2 = cJSON_CreateNumber(25.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_payload", "{\"note\":\"raw_data\",\"count\":3}");
    cJSON_bool root_is_object = cJSON_IsObject(root);
    int item_count = cJSON_GetArraySize(items);
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    double version_value = cJSON_GetNumberValue(version_item);
    double new_version_value = version_value * (1.0 + (double)root_is_object);
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_meta, "version", new_version_value);
    cJSON_AddStringToObject(new_meta, "status", "replaced");
    cJSON_ReplaceItemInObject(root, "meta", new_meta);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    double s0 = cJSON_GetNumberValue(a0);
    double s1 = cJSON_GetNumberValue(a1);
    double s2 = cJSON_GetNumberValue(a2);
    double sum = s0 + s1 + s2;
    double average = sum / (double)item_count;
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(average));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}