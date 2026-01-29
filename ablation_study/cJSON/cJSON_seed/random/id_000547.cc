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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool is_arr_before = 0;
    cJSON_bool is_arr_after = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_str = cJSON_CreateString("filename");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "items", arr);
    add_ok2 = cJSON_AddItemToObject(root, "title", name_str);
    add_ok3 = cJSON_AddItemToObject(root, "placeholder", null_item);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "items");
    is_arr_before = cJSON_IsArray(retrieved);
    detached = cJSON_DetachItemViaPointer(root, name_str);
    add_ok4 = cJSON_AddItemToArray(arr, detached);
    is_arr_after = cJSON_IsArray(cJSON_GetObjectItem(root, "items"));

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)is_arr_before;
    (void)is_arr_after;
    (void)retrieved;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}