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
//<ID> 805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root_array = cJSON_CreateArray();
    cJSON *obj = cJSON_CreateObject();
    cJSON *false_item = cJSON_CreateFalse();
    const char *json_text = "[{\"name\":\"alpha\"},{\"name\":\"beta\"}]";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *strref = cJSON_CreateStringReference("ref_value");

    // step 2: Configure
    cJSON_AddItemToObject(obj, "ref", strref);
    cJSON_AddItemToObject(obj, "flag", false_item);
    cJSON_AddItemToArray(root_array, obj);
    cJSON_AddItemToArray(root_array, parsed);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(root_array);
    cJSON *first = cJSON_GetArrayItem(root_array, 0);
    cJSON *second = cJSON_GetArrayItem(root_array, 1);
    cJSON *ref_value_item = cJSON_GetObjectItemCaseSensitive(first, "ref");
    const char *ref_str = cJSON_GetStringValue(ref_value_item);
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(first, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON_DeleteItemFromObjectCaseSensitive(first, "ref");

    // step 4: Cleanup
    cJSON_Delete(root_array);
    // API sequence test completed successfully
    return 66;
}