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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *list = cJSON_AddArrayToObject(root, "items");
    cJSON_AddNumberToObject(config, "factor", 2.5);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(30.0));

    // step 2: Configure
    const char *json_text = "{\"threshold\": 3.14, \"nested\": {\"value\": 99}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_threshold = cJSON_GetObjectItemCaseSensitive(parsed, "threshold");
    double threshold = cJSON_GetNumberValue(parsed_threshold);
    cJSON_AddNumberToObject(config, "threshold", threshold);

    // step 3: Operate and Validate
    cJSON *detached_nested = cJSON_DetachItemFromObject(parsed, "nested");
    cJSON_AddItemToObject(root, "imported_nested", detached_nested);
    cJSON *root_thresh_item = cJSON_GetObjectItemCaseSensitive(config, "threshold");
    double root_thresh_val = cJSON_GetNumberValue(root_thresh_item);
    cJSON_AddNumberToObject(config, "threshold_double", root_thresh_val * 2.0);
    int items_count = cJSON_GetArraySize(list);
    cJSON_AddNumberToObject(root, "items_count", (double)items_count);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}