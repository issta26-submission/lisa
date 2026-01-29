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
//<ID> 1817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *item_from_list = (cJSON *)0;
    cJSON *item_from_ref = (cJSON *)0;
    const char *strval_list = (const char *)0;
    const char *strval_ref = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    list = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(list, s0);
    cJSON_AddItemToArray(list, s1);
    add_ok1 = cJSON_AddItemToObject(root, "list", list);
    add_ok2 = cJSON_AddItemToObject(root, "nothing", null_item);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)s0);
    add_ok3 = cJSON_AddItemToObject(root, "ref", arr_ref);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    item_from_list = cJSON_GetArrayItem(retrieved_list, 1);
    strval_list = cJSON_GetStringValue(item_from_list);
    retrieved_ref = cJSON_GetObjectItem(root, "ref");
    item_from_ref = cJSON_GetArrayItem(retrieved_ref, 0);
    strval_ref = cJSON_GetStringValue(item_from_ref);

    // step 5: Validate
    validation = (int)(retrieved_list != (cJSON *)0) + (int)(item_from_list != (cJSON *)0) + (int)(strval_list != (const char *)0) + (int)(retrieved_ref != (cJSON *)0) + (int)(item_from_ref != (cJSON *)0) + (int)(strval_ref != (const char *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)list;
    (void)s0;
    (void)s1;
    (void)null_item;
    (void)arr_ref;
    (void)retrieved_list;
    (void)retrieved_ref;
    (void)item_from_list;
    (void)item_from_ref;
    (void)strval_list;
    (void)strval_ref;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}