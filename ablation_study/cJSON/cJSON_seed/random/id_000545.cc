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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool add_ok = 0;
    cJSON_bool is_array = 0;
    char *detached_str = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello-world");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "arr", child_arr);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddItemToObject(root, "maybe_null", null_item);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "arr");
    is_array = cJSON_IsArray(retrieved);
    retrieved = cJSON_GetObjectItem(root, "greeting");
    detached_item = cJSON_DetachItemViaPointer(root, retrieved);
    detached_str = cJSON_GetStringValue(detached_item);

    // step 5: Validate
    (void)add_ok;
    (void)is_array;
    (void)detached_str;
    (void)detached_item;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_item);

    // API sequence test completed successfully
    return 66;
}