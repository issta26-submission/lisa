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
//<ID> 1812
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
    cJSON *arr_ref = (cJSON *)0;
    cJSON *nil_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    char *strval = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("hello");
    nil_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToObject(root, "original", arr);
    arr_ref = cJSON_CreateArrayReference(arr);
    add_ok3 = cJSON_AddItemToObject(root, "alias", arr_ref);
    cJSON_AddItemToObject(root, "nothing", nil_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "alias");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    strval = cJSON_GetStringValue(item0);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(s0 != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(item0 != (cJSON *)0) + (int)(strval != (char *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)arr_ref;
    (void)nil_item;
    (void)retrieved_arr;
    (void)item0;
    (void)strval;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}