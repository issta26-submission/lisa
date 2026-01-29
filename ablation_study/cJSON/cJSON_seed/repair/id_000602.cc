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
//<ID> 602
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
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(data, n0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_meta", "{\"raw\":true}");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "label", cJSON_CreateString("sample"));
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_meta, "version", 2.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "meta", new_meta);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(data);
    cJSON *a0 = cJSON_GetArrayItem(data, 0);
    cJSON *a1 = cJSON_GetArrayItem(data, 1);
    cJSON *a2 = cJSON_GetArrayItem(data, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_AddNumberToObject(root, "average", average);
    cJSON_AddNumberToObject(root, "meta_replaced_flag", (double)replaced);
    cJSON_AddNumberToObject(root, "meta_is_object_flag", (double)cJSON_IsObject(new_meta));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}