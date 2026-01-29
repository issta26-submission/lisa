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
//<ID> 549
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
    cJSON *name = (cJSON *)0;
    cJSON *nil = (cJSON *)0;
    cJSON *detached_arr = (cJSON *)0;
    char *name_val = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_arr = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name = cJSON_CreateString("example-name");
    nil = cJSON_CreateNull();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "items", arr);
    add_ok2 = cJSON_AddItemToObject(root, "title", name);
    add_ok3 = cJSON_AddItemToObject(root, "nothing", nil);

    // step 4: Operate
    detached_arr = cJSON_DetachItemViaPointer(root, arr);
    is_arr = cJSON_IsArray(detached_arr);
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_arr;
    (void)name_val;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_arr);

    // API sequence test completed successfully
    return 66;
}