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
//<ID> 814
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
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    const char *json_text = "{\"config\":{\"threshold\":7.25}, \"extra\":{\"value\":99}, \"arr\":[1,2,3,4]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *config = cJSON_GetObjectItemCaseSensitive(parsed, "config");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(config, "threshold");
    double threshold = cJSON_GetNumberValue(threshold_item);
    cJSON_AddNumberToObject(meta, "threshold_from_parsed", threshold);

    // step 3: Operate and Validate
    cJSON *extra_detached = cJSON_DetachItemFromObject(parsed, "extra");
    cJSON_AddItemToObject(root, "imported_extra", extra_detached);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(parsed, "arr");
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(meta, "parsed_arr_size", (double)arr_size);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}