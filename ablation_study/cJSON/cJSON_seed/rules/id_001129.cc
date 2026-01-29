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
//<ID> 1129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_flag = NULL;
    cJSON *replacement_array = NULL;
    cJSON *parsed = NULL;
    cJSON *arr_from_json = NULL;
    cJSON *first_str_item = NULL;
    const char *first_item_value = NULL;
    const char *parse_end = NULL;
    char *dynamic_str = NULL;
    const char *strs[3];
    const char json[] = "{\"items\":[\"alpha\",\"beta\"]}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", old_flag);
    dynamic_str = (char *)cJSON_malloc(8);
    memset(dynamic_str, 0, 8);
    dynamic_str[0] = 'd';
    dynamic_str[1] = 'y';
    dynamic_str[2] = 'n';
    dynamic_str[3] = '\0';
    strs[0] = "one";
    strs[1] = dynamic_str;
    strs[2] = "three";
    replacement_array = cJSON_CreateStringArray(strs, 3);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_flag, replacement_array);
    parsed = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    arr_from_json = cJSON_GetObjectItem(parsed, "items");
    first_item_value = cJSON_GetStringValue(cJSON_GetArrayItem(arr_from_json, 0));
    first_str_item = cJSON_CreateString(first_item_value);
    cJSON_AddItemToObject(root, "first_parsed", first_str_item);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(dynamic_str);
    // API sequence test completed successfully
    return 66;
}