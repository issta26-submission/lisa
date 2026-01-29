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
//<ID> 806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 2: Configure
    const char *json_text = "{\"name\":\"parser\",\"remove_me\":\"temp\",\"nested\":{\"inner\":1}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *sref = cJSON_CreateStringReference("external_ref");
    cJSON_AddItemToObject(parsed, "ref", sref);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove_me");
    cJSON *moved_name = cJSON_DetachItemFromObject(parsed, "name");
    cJSON_AddItemToArray(arr, moved_name);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON *nested = cJSON_GetObjectItem(parsed, "nested");
    cJSON *inner = cJSON_GetObjectItem(nested, "inner");
    double inner_value = cJSON_GetNumberValue(inner);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}