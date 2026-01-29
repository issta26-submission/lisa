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
//<ID> 546
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
    cJSON *name_item = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("example-name");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "items", arr);
    add_ok2 = cJSON_AddItemToObject(root, "name", name_item);
    add_ok3 = cJSON_AddItemToObject(root, "optional", null_item);

    // step 4: Operate
    is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "items"));
    retrieved_name = cJSON_GetObjectItem(root, "name");
    detached_item = cJSON_DetachItemViaPointer(root, retrieved_name);
    cJSON_AddItemToObject(root, "title", detached_item);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_array;
    (void)retrieved_name;
    (void)detached_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}