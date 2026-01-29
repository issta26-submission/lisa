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
//<ID> 312
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
    cJSON *greet = (cJSON *)0;
    cJSON *status = (cJSON *)0;
    const char *greeting_val = (const char *)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    greet = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(child, "greeting", greet);
    cJSON_AddTrueToObject(child, "isActive");
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    greeting_val = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "child"), "greeting"));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    status = cJSON_CreateString(greeting_val ? greeting_val : "");
    cJSON_AddItemToObject(root, "extracted_greeting", status);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}