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
//<ID> 816
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_AddArrayToObject(cfg, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.5));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.75));

    // step 2: Configure
    const char *json_text = "{\"threshold\":7.25,\"nested\":{\"x\":42}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(parsed, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);
    cJSON_AddNumberToObject(cfg, "threshold", threshold_value);
    cJSON *detached_nested = cJSON_DetachItemFromObject(parsed, "nested");
    int values_count = cJSON_GetArraySize(values);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    (void)values_count;
    (void)snapshot_root;
    (void)snapshot_parsed;

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(detached_nested);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}