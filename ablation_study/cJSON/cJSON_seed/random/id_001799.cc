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
//<ID> 1799
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    const char *strval = (const char *)0;
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    s1 = cJSON_CreateString("second");

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    add_ok1 = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    detached_item = cJSON_DetachItemViaPointer(retrieved_arr, item0);
    strval = cJSON_GetStringValue(detached_item);
    add_ok2 = cJSON_AddItemToObject(root, "first", detached_item);

    // step 5: Validate
    validation = (int)(retrieved_arr != (cJSON *)0) + (int)(arr_size == 2) + (int)(detached_item != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)retrieved_arr;
    (void)item0;
    (void)detached_item;
    (void)strval;
    (void)arr_size;
    (void)add_ok1;
    (void)add_ok2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}