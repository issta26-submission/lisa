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
//<ID> 544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *null_child = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *greeting_val = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool is_detached_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    null_child = cJSON_CreateNull();
    str_item = cJSON_CreateString("hello-cJSON");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "null_child", null_child);
    add_ok2 = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate
    detached = cJSON_DetachItemViaPointer(root, null_child);
    is_detached_array = cJSON_IsArray(detached);
    greeting_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)is_detached_array;
    (void)greeting_val;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}