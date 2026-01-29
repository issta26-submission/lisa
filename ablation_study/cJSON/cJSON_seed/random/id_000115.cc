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
//<ID> 115
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
    cJSON *s_name = (cJSON *)0;
    cJSON *s_value = (cJSON *)0;
    cJSON *name_copy = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool is_child_object = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    s_name = cJSON_CreateString("first");
    s_value = cJSON_CreateString("second");
    cJSON_AddItemToObject(child, "name", s_name);
    cJSON_AddItemToObject(child, "value", s_value);

    // step 3: Configure / Operate
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddFalseToObject(root, "active");
    is_child_object = cJSON_IsObject(cJSON_GetObjectItem(root, "child"));
    name_copy = cJSON_CreateString(cJSON_GetStringValue(cJSON_GetObjectItem(child, "name")));
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}