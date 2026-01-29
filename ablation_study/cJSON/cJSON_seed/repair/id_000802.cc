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
//<ID> 802
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
    cJSON_AddItemToObject(root, "items", array);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, false_item);
    const char *ref_literal = "referenced_string";
    cJSON *str_ref = cJSON_CreateStringReference(ref_literal);
    cJSON_AddItemToArray(array, str_ref);

    // step 2: Configure and Operate
    const char *json_text = "{\"keep\":\"yes\",\"remove\":\"to_be_removed\",\"nested\":{\"key\":\"val\"}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *detached_nested = cJSON_DetachItemFromObject(parsed, "nested");
    cJSON_AddItemToObject(root, "imported_nested", detached_nested);

    // step 3: Validate
    int array_size = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    const char *second_str = cJSON_GetStringValue(second);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove");
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}