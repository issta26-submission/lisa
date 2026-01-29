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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("world");
    cJSON_ReplaceItemViaPointer(root, greeting, replacement);
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_false);
    const char *current_greeting = cJSON_GetStringValue(replacement);
    (void)flag_was_false;
    (void)current_greeting;

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}