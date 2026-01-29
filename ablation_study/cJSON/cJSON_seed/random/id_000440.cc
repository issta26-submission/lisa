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
//<ID> 440
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
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *dup_config = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    char *printed = (char *)0;
    int sz = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(7.0);
    n1 = cJSON_CreateNumber(13.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(config, "values", arr);
    flag_false = cJSON_AddFalseToObject(config, "enabled");

    // step 3: Operate
    sz = cJSON_GetArraySize(arr);
    dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);
    detached = cJSON_DetachItemViaPointer(arr, cJSON_GetArrayItem(arr, 0));
    cJSON_AddItemToObject(root, "detached", detached);

    // step 4: Validate
    is_false = cJSON_IsFalse(flag_false);
    (void)sz;
    (void)is_false;

    // step 5: Cleanup
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // step 6: API sequence test completed successfully
    return 66;
}