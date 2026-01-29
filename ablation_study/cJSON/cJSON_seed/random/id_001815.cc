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
//<ID> 1815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *greet_item = (cJSON *)0;
    const char *strval = (const char *)0;
    const char *greet_str = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s0 = cJSON_CreateString("elem0");
    null_item = cJSON_CreateNull();
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(child, "greeting", cJSON_CreateString("hello"));
    cJSON_AddItemToObject(child, "absent", null_item);
    cJSON_AddItemToObject(root, "child", child);
    arr_ref = cJSON_CreateArrayReference(s0);
    cJSON_AddItemToObject(root, "ref_arr", arr_ref);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_arr = cJSON_GetObjectItem(root, "ref_arr");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    strval = cJSON_GetStringValue(item0);
    greet_item = cJSON_GetObjectItem(retrieved_child, "greeting");
    greet_str = cJSON_GetStringValue(greet_item);

    // step 5: Validate
    validation = (int)(retrieved_child != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(item0 != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(greet_str != (const char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(s0);
    (void)root;
    (void)s0;
    (void)null_item;
    (void)child;
    (void)arr_ref;
    (void)retrieved_child;
    (void)retrieved_arr;
    (void)item0;
    (void)greet_item;
    (void)strval;
    (void)greet_str;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}