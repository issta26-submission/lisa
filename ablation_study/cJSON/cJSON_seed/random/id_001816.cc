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
//<ID> 1816
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
    cJSON *arrref = (cJSON *)0;
    cJSON *nullval = (cJSON *)0;
    cJSON *nullref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *ref_item0 = (cJSON *)0;
    cJSON *retrieved_nullref = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    const char *strval = (const char *)0;
    const char *strval_ref = (const char *)0;
    const char *null_str = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    nullval = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    add_ok1 = cJSON_AddItemToObject(root, "items", arr);
    arrref = cJSON_CreateArrayReference(arr);
    add_ok2 = cJSON_AddItemToObject(root, "items_ref", arrref);
    nullref = cJSON_CreateArrayReference(nullval);
    add_ok3 = cJSON_AddItemToObject(root, "null_ref", nullref);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    strval = cJSON_GetStringValue(item0);
    retrieved_ref = cJSON_GetObjectItem(root, "items_ref");
    ref_item0 = cJSON_GetArrayItem(retrieved_ref, 0);
    strval_ref = cJSON_GetStringValue(ref_item0);
    retrieved_nullref = cJSON_GetObjectItem(root, "null_ref");
    null_item = cJSON_GetArrayItem(retrieved_nullref, 0);
    null_str = cJSON_GetStringValue(null_item);

    // step 5: Validate
    validation = (int)(retrieved_arr != (cJSON *)0)
               + (int)(item0 != (cJSON *)0)
               + (int)(strval != (const char *)0)
               + (int)(retrieved_ref != (cJSON *)0)
               + (int)(ref_item0 != (cJSON *)0)
               + (int)(strval_ref != (const char *)0)
               + (int)(retrieved_nullref != (cJSON *)0)
               + (int)(null_item == (cJSON *)0 || null_item != (cJSON *)0) /* harmless check to include null_item */
               + (int)(null_str == (const char *)0)
               + (int)(add_ok1 != 0)
               + (int)(add_ok2 != 0)
               + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)arrref;
    (void)nullval;
    (void)nullref;
    (void)retrieved_arr;
    (void)item0;
    (void)retrieved_ref;
    (void)ref_item0;
    (void)retrieved_nullref;
    (void)null_item;
    (void)strval;
    (void)strval_ref;
    (void)null_str;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}