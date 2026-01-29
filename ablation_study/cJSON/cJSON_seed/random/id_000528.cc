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
//<ID> 528
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
    cJSON *bool_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool is_bool = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    bool_item = cJSON_CreateTrue();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "items", arr);
    add_ok2 = cJSON_AddItemToArray(arr, bool_item);
    name_item = cJSON_AddStringToObject(root, "name", "cJSON_example");

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    is_bool = cJSON_IsBool((const cJSON *const)bool_item);

    // step 5: Validate
    (void)retrieved_name;
    (void)is_bool;
    (void)add_ok1;
    (void)add_ok2;
    (void)name_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}