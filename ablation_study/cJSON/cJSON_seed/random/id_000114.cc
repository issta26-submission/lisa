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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *greeting_copy = (cJSON *)0;
    const char *greeting_text = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool is_root_obj = 0;
    cJSON_bool is_greeting_obj = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddFalseToObject(root, "active");

    // step 3: Operate
    greeting_text = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    greeting_copy = cJSON_CreateString(greeting_text ? greeting_text : "");
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);

    // step 4: Validate
    is_root_obj = cJSON_IsObject(root);
    is_greeting_obj = cJSON_IsObject(cJSON_GetObjectItem(root, "greeting"));
    printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}