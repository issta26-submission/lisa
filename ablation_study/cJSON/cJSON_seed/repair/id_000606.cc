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
//<ID> 606
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(4.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(8.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(12.0));

    // step 2: Configure
    cJSON_AddRawToObject(meta, "raw_config", "{\"enabled\":true,\"limit\":10}");
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "label", "replaced_meta");
    cJSON_AddNumberToObject(new_meta, "scale", 3.0);

    // step 3: Operate and Validate
    cJSON_bool was_meta_object = cJSON_IsObject(meta);
    cJSON_bool is_new_meta_object = cJSON_IsObject(new_meta);
    cJSON_ReplaceItemInObject(root, "meta", new_meta);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(got_items);
    cJSON *e0 = cJSON_GetArrayItem(got_items, 0);
    cJSON *e1 = cJSON_GetArrayItem(got_items, 1);
    cJSON *e2 = cJSON_GetArrayItem(got_items, 2);
    double v0 = cJSON_GetNumberValue(e0);
    double v1 = cJSON_GetNumberValue(e1);
    double v2 = cJSON_GetNumberValue(e2);
    double sum = v0 + v1 + v2;
    double average = sum / (double)count;
    double adjusted_average = average * (1.0 + (double)(is_new_meta_object));
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(average));
    cJSON_AddItemToObject(root, "adjusted_average", cJSON_CreateNumber(adjusted_average));
    cJSON_AddRawToObject(root, "root_raw", "{\"note\":\"added_at_root\"}");

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}