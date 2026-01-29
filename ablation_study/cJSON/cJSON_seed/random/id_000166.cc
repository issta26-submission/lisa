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
//<ID> 166
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
    cJSON *num_value = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_value = (cJSON *)0;
    cJSON *is_obj_num = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool child_is_object = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_value = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(child, "value", num_value);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    child_is_object = cJSON_IsObject(got_child);
    got_value = cJSON_GetObjectItem(got_child, "value");
    is_obj_num = cJSON_CreateNumber((double)child_is_object);

    // step 5: Validate
    cJSON_AddItemToObject(root, "child_is_object", is_obj_num);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}