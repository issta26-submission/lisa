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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greeting", greet);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("hi");
    cJSON_ReplaceItemViaPointer(child, greet, replacement);
    cJSON *greeting_item = cJSON_GetObjectItem(child, "greeting");
    const char *greeting_val = cJSON_GetStringValue(greeting_item);
    cJSON *greeting_copy = cJSON_CreateString(greeting_val);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "flag_numeric", (double)flag_is_false);

    // step 3: Operate
    char *printed = cJSON_Print(root);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}