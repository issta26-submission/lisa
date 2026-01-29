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
//<ID> 1810
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
    cJSON *arr = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *nul = (cJSON *)0;
    cJSON *retrieved_container = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *arrref_obj = (cJSON *)0;
    cJSON *item0_ref = (cJSON *)0;
    const char *strval1 = (const char *)0;
    const char *strval0 = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    container = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("hello");
    s1 = cJSON_CreateString("world");
    nul = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(container, "items", arr);
    cJSON_AddItemToObject(container, "nothing", nul);
    add_ok1 = cJSON_AddItemToObject(root, "container", container);
    arr_ref = cJSON_CreateArrayReference(arr);
    add_ok2 = cJSON_AddItemToObject(root, "items_ref", arr_ref);
    add_ok3 = add_ok1 && add_ok2;

    // step 4: Operate
    retrieved_container = cJSON_GetObjectItem(root, "container");
    retrieved_arr = cJSON_GetObjectItem(retrieved_container, "items");
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    strval1 = cJSON_GetStringValue(item1);
    arrref_obj = cJSON_GetObjectItem(root, "items_ref");
    item0_ref = cJSON_GetArrayItem(arrref_obj, 0);
    strval0 = cJSON_GetStringValue(item0_ref);

    // step 5: Validate
    validation = (int)(retrieved_container != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(strval1 != (const char *)0) + (int)(arrref_obj != (cJSON *)0) + (int)(item0_ref != (cJSON *)0) + (int)(strval0 != (const char *)0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)container;
    (void)arr;
    (void)arr_ref;
    (void)s0;
    (void)s1;
    (void)nul;
    (void)retrieved_container;
    (void)retrieved_arr;
    (void)item1;
    (void)arrref_obj;
    (void)item0_ref;
    (void)strval1;
    (void)strval0;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}