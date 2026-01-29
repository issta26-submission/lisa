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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *echo = (cJSON *)0;
    const char *extracted = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    nested = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "note", name_item);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddItemToObject(root, "nested", nested);

    // step 4: Operate
    child_str = cJSON_CreateString("child_value");
    cJSON_AddItemToObject(nested, "child_key", child_str);
    extracted = cJSON_GetStringValue(cJSON_GetObjectItem(root, "note"));

    // step 5: Validate
    echo = cJSON_CreateString(extracted);
    cJSON_AddItemToObject(root, "echo", echo);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}