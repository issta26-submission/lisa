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
//<ID> 801
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
    cJSON *ref = cJSON_CreateStringReference("ref_value");
    cJSON_AddItemToObject(root, "ref", ref);

    // step 2: Configure
    const char *json_text = "{\"keep\":42,\"remove\":\"to_be_removed\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *keep_item = cJSON_GetObjectItem(parsed, "keep");
    double keep_val = cJSON_GetNumberValue(keep_item);
    cJSON_AddNumberToObject(root, "imported_keep", keep_val);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove");
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first);
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(out_root);
    cJSON_free(out_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}