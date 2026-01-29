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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    cJSON_bool replace_ok = (cJSON_bool)0;
    cJSON_bool flag_is_string = (cJSON_bool)0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "greeting", greeting);
    flag_item = cJSON_AddFalseToObject(root, "flag");

    // step 4: Operate
    replacement = cJSON_CreateString("replaced_flag");
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement);

    // step 5: Validate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_greeting = cJSON_GetObjectItem(retrieved_child, "greeting");
    flag_is_string = cJSON_IsString(retrieved_flag);
    flag_is_false = cJSON_IsFalse(retrieved_flag);
    cJSON_AddItemToObject(root, "flag_is_string", cJSON_CreateString(flag_is_string ? "true" : "false"));
    cJSON_AddItemToObject(root, "flag_is_false", cJSON_CreateString(flag_is_false ? "true" : "false"));
    cJSON_AddItemToObject(root, "greeting_value", cJSON_CreateString(cJSON_GetStringValue(retrieved_greeting)));
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}