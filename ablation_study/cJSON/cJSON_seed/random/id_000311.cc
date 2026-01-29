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
//<ID> 311
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
    cJSON *got_greeting = (cJSON *)0;
    cJSON *got_success = (cJSON *)0;
    cJSON *success_str = (cJSON *)0;
    const char *greeting_val = (const char *)0;
    cJSON_bool success_bool = (cJSON_bool)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddTrueToObject(root, "success");

    // step 4: Operate
    got_greeting = cJSON_GetObjectItem(root, "greeting");
    got_success = cJSON_GetObjectItem(root, "success");
    greeting_val = cJSON_GetStringValue(got_greeting);
    greeting_copy = cJSON_CreateString(greeting_val);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    success_bool = cJSON_IsTrue(got_success);
    success_str = cJSON_CreateString(success_bool ? "true" : "false");
    cJSON_AddItemToObject(root, "success_str", success_str);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}