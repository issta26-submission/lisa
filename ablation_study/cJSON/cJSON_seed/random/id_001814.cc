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
//<ID> 1814
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
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    const char *strval = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddItemToObject(root, "nothing", null_item);
    arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "letters_ref", arr_ref);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    strval = cJSON_GetStringValue(item1);

    // step 5: Validate
    validation = (int)(retrieved_arr != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(arr_ref != (cJSON *)0) + (int)(null_item != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)null_item;
    (void)arr_ref;
    (void)retrieved_arr;
    (void)item1;
    (void)strval;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}