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
//<ID> 1811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *retrieved_container = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    char *strval = (char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    container = cJSON_CreateObject();
    list = cJSON_CreateArray();
    s0 = cJSON_CreateString("element_one");
    s1 = cJSON_CreateString("element_two");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(list, s0);
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToObject(container, "items", list);
    cJSON_AddItemToObject(container, "empty", null_item);
    cJSON_AddItemToObject(root, "container", container);

    // step 4: Operate
    retrieved_container = cJSON_GetObjectItem(root, "container");
    retrieved_list = cJSON_GetObjectItem(retrieved_container, "items");
    arr_ref = cJSON_CreateArrayReference(retrieved_list);
    arr_item0 = cJSON_GetArrayItem(arr_ref, 0);
    strval = cJSON_GetStringValue(arr_item0);

    // step 5: Validate
    validation = (int)(retrieved_container != (cJSON *)0) + (int)(retrieved_list != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(arr_item0 != (cJSON *)0) + (int)(strval != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(arr_ref);
    cJSON_Delete(root);
    (void)root;
    (void)container;
    (void)list;
    (void)s0;
    (void)s1;
    (void)null_item;
    (void)retrieved_container;
    (void)retrieved_list;
    (void)arr_item0;
    (void)strval;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}