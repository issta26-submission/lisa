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
//<ID> 1553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json = "{\"greeting\":\"hello\",\"num\":7}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *greeting = cJSON_GetObjectItem(parsed, "greeting");
    cJSON_bool greeting_is_string = cJSON_IsString(greeting);
    cJSON *orig = cJSON_CreateString("original");
    cJSON_AddItemToObjectCS(root, "replace_me", orig);

    // step 2: Configure
    cJSON *case_item = cJSON_CreateString("case_sensitive_value");
    cJSON_AddItemToObjectCS(root, "caseKey", case_item);
    cJSON *is_str_num = cJSON_CreateNumber((double)greeting_is_string);
    cJSON_AddItemToObjectCS(root, "greeting_is_string", is_str_num);
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, orig, replacement);

    // step 3: Operate and Validate
    cJSON *greet_dup = cJSON_Duplicate(greeting, 1);
    cJSON_AddItemToObjectCS(root, "greeting_dup", greet_dup);
    (void)replaced_ok;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}