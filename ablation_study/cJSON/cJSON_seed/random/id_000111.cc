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
//<ID> 111
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
    char *json_text = (char *)0;
    cJSON_bool child_is_object = 0;
    cJSON_bool fetched_is_object = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddFalseToObject(root, "active");
    child_is_object = cJSON_IsObject(child);

    // step 3: Operate
    json_text = cJSON_Print(root);
    cJSON_free((void *)json_text);
    fetched_is_object = cJSON_IsObject(cJSON_GetObjectItem(root, "child"));

    // step 4: Validate
    cJSON_AddItemToObject(root, "child_is_object", cJSON_CreateNumber((double)child_is_object));
    cJSON_AddItemToObject(root, "fetched_is_object", cJSON_CreateNumber((double)fetched_is_object));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}