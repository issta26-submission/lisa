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
//<ID> 1813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *s_greeting = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *greeting_item = (cJSON *)0;
    const char *strval = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    s_greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(child, "greeting", s_greeting);
    cJSON_AddItemToObject(root, "child", child);
    arr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    arr_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToArray(arr, arr_ref);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "arr");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    greeting_item = cJSON_GetObjectItem(item1, "greeting");
    strval = cJSON_GetStringValue(greeting_item);

    // step 4: Validate
    validation = (int)(retrieved_arr != (cJSON *)0) + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(greeting_item != (cJSON *)0) + (int)(strval != (const char *)0);
    (void)validation;

    // step 5: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)child;
    (void)s_greeting;
    (void)arr;
    (void)null_item;
    (void)arr_ref;
    (void)retrieved_arr;
    (void)item0;
    (void)item1;
    (void)greeting_item;
    (void)strval;
    (void)validation;

    // API sequence test completed successfully
    return 66;
}