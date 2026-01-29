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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *orig = (cJSON *)0;
    cJSON *rep = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool replace_ok = (cJSON_bool)0;
    cJSON_bool item_is_string = (cJSON_bool)0;
    cJSON_bool flag_is_false = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("original_value");

    // step 3: Configure
    cJSON_AddItemToObject(root, "item", orig);
    flag = cJSON_AddFalseToObject(root, "enabled");

    // step 4: Operate
    rep = cJSON_CreateString("updated_value");
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "item", rep);

    // step 5: Validate
    retrieved_item = cJSON_GetObjectItem(root, "item");
    retrieved_flag = cJSON_GetObjectItem(root, "enabled");
    item_is_string = cJSON_IsString(retrieved_item);
    flag_is_false = cJSON_IsFalse(retrieved_flag);
    cJSON_AddItemToObject(root, "item_is_string", cJSON_CreateNumber((double)item_is_string));
    cJSON_AddItemToObject(root, "flag_is_false", cJSON_CreateNumber((double)flag_is_false));
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}