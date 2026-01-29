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
//<ID> 803
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, f);

    // step 2: Configure
    const char *json_text = "{\"name\":\"example\",\"old\":\"to_remove\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *nameRef = cJSON_CreateStringReference("referenced_name");
    cJSON_AddItemToObject(parsed, "ref", nameRef);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "old");
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "imported", dup);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}