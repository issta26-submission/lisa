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
//<ID> 525
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
    cJSON *msg_item = (cJSON *)0;
    cJSON_bool added_ok1 = 0;
    cJSON_bool added_ok2 = 0;
    cJSON_bool is_bool = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    bool_item = cJSON_CreateTrue();

    // step 3: Configure
    added_ok1 = cJSON_AddItemToArray(arr, cJSON_CreateString("first_entry"));
    added_ok2 = cJSON_AddItemToArray(arr, bool_item);
    (void)added_ok1;
    (void)added_ok2;
    msg_item = cJSON_AddStringToObject(root, "status", "ok");
    (void)msg_item;
    cJSON_AddItemToObject(root, "flags", arr);

    // step 4: Operate
    is_bool = cJSON_IsBool((const cJSON *const)bool_item);

    // step 5: Validate
    (void)is_bool;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}