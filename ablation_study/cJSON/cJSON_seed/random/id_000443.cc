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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *dup_obj = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    inner = cJSON_AddObjectToObject(config, "inner");
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(config, "values", arr);
    cJSON_AddItemToObject(config, "flag", flag_false);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    is_false = cJSON_IsFalse(flag_false);
    dup_obj = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_obj);
    detached = cJSON_DetachItemViaPointer(config, arr);
    cJSON_AddItemToObject(root, "detached_values", detached);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)arr_size;
    (void)is_false;
    (void)printed;
    (void)inner;
    (void)dup_obj;
    (void)detached;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}