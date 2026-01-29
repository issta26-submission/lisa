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
//<ID> 1149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_item = NULL;
    cJSON *replacement_array = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_name_item = NULL;
    const char *parsed_name = NULL;
    cJSON_bool parsed_name_is_string = 0;
    char *printed = NULL;
    char *dyn = NULL;
    const char *arr_strs[3];
    const char json[] = "{\"name\":\"original\",\"list\":[\"one\",\"two\"]}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_item = cJSON_CreateString("to_be_replaced");
    cJSON_AddItemToObject(root, "replace_me", old_item);
    dyn = (char *)cJSON_malloc(16);
    memset(dyn, 0, 16);
    dyn[0] = 'd';
    dyn[1] = 'y';
    dyn[2] = 'n';
    dyn[3] = '\0';
    arr_strs[0] = "one";
    arr_strs[1] = dyn;
    arr_strs[2] = "three";
    replacement_array = cJSON_CreateStringArray(arr_strs, 3);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_item, replacement_array);
    parsed = cJSON_ParseWithLength(json, (size_t)(sizeof(json) - 1));
    parsed_name_item = cJSON_GetObjectItem(parsed, "name");
    parsed_name_is_string = cJSON_IsString(parsed_name_item);
    parsed_name = cJSON_GetStringValue(parsed_name_item);
    cJSON_AddItemToObject(root, "from_parsed", cJSON_CreateString(parsed_name));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(dyn);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}