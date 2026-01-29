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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_child = NULL;
    cJSON *replacement = NULL;
    cJSON *arr = NULL;
    cJSON *parsed = NULL;
    cJSON *first = NULL;
    cJSON *str_item = NULL;
    const cJSON *items = NULL;
    const char *first_val = NULL;
    char *printed = NULL;
    char *dyn = NULL;
    cJSON_bool isstr = 0;
    const char json[] = "{\"items\":[\"p_one\",\"p_two\"]}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_child = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "key", old_child);
    dyn = (char *)cJSON_malloc(8);
    memset(dyn, 0, 8);
    dyn[0] = 'b';
    dyn[1] = 'e';
    dyn[2] = 't';
    dyn[3] = 'a';
    dyn[4] = '\0';
    const char *strs[2];
    strs[0] = "one";
    strs[1] = dyn;
    arr = cJSON_CreateStringArray(strs, 2);
    cJSON_AddItemToObject(root, "array", arr);
    replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemViaPointer(root, old_child, replacement);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(json);
    items = cJSON_GetObjectItem(parsed, "items");
    first = cJSON_GetArrayItem((cJSON *)items, 0);
    first_val = cJSON_GetStringValue(first);
    str_item = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_parsed", str_item);
    isstr = cJSON_IsString(cJSON_GetObjectItem(root, "key"));
    cJSON_AddBoolToObject(root, "key_is_string", isstr);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(dyn);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}