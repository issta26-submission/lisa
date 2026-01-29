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
//<ID> 808
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
    cJSON *falsy = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, falsy);
    cJSON *sref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(arr, sref);
    const char *json_text = "{\"to_remove\":\"bye\",\"import_me\":{\"value\":123}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *imported = cJSON_DetachItemFromObject(parsed, "import_me");
    cJSON_AddItemToObject(root, "imported", imported);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "to_remove");

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_false = cJSON_IsFalse(first_item);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    const char *second_str = cJSON_GetStringValue(second_item);
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