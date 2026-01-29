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
//<ID> 809
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", array);

    // step 2: Configure
    const char *json_text = "[{\"name\":\"one\"},{\"name\":\"two\"}]";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *str_ref = cJSON_CreateStringReference("ref_value");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, false_item);
    cJSON_AddItemToArray(array, str_ref);
    cJSON *first_parsed = cJSON_GetArrayItem(parsed, 0);
    cJSON *imported_dup = cJSON_Duplicate(first_parsed, 1);
    cJSON_AddItemToObject(root, "imported", imported_dup);

    // step 3: Operate and Validate
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    int arr_size = cJSON_GetArraySize(array);
    cJSON *arr_item0 = cJSON_GetArrayItem(array, 0);
    cJSON_bool item0_is_false = cJSON_IsFalse(arr_item0);
    (void)arr_size;
    (void)item0_is_false;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "imported");
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}