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
//<ID> 819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_AddArrayToObject(data, "items");
    cJSON *item0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, item0);
    cJSON *item1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, item1);

    // step 2: Configure
    const char *json_text = "{\"settings\":{\"threshold\":42.5},\"unused\":true}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(meta, "version");
    double ver_value = cJSON_GetNumberValue(ver_item);
    cJSON *settings = cJSON_GetObjectItemCaseSensitive(parsed, "settings");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(settings, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "unused");
    cJSON *imported = cJSON_AddObjectToObject(root, "imported");
    cJSON_AddNumberToObject(imported, "threshold", threshold_value);
    int arr_size = cJSON_GetArraySize(items);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(detached);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}