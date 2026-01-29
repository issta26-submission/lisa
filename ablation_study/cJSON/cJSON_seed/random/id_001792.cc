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
//<ID> 1792
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
    cJSON *s2 = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *strval = (const char *)0;
    cJSON_bool add_ok_arr_to_root = 0;
    cJSON_bool add_ok0 = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int size_before = 0;
    int size_after = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    s1 = cJSON_CreateString("second");
    s2 = cJSON_CreateString("third");
    second = cJSON_CreateObject();

    // step 3: Configure
    add_ok0 = cJSON_AddItemToArray(arr, s0);
    add_ok1 = cJSON_AddItemToArray(arr, s1);
    add_ok2 = cJSON_AddItemToArray(arr, s2);
    add_ok_arr_to_root = cJSON_AddItemToObject(root, "array", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    item1 = cJSON_GetArrayItem(arr, 1);
    detached = cJSON_DetachItemViaPointer(arr, item1);
    strval = cJSON_GetStringValue(detached);
    cJSON_bool add_moved_ok = cJSON_AddItemToObject(second, "moved", detached);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation = (int)(size_before == 3) + (int)(size_after == 2) + (int)(strval != (const char *)0) + (int)(add_ok0 != 0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok_arr_to_root != 0) + (int)(add_moved_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(second);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)s2;
    (void)second;
    (void)item1;
    (void)detached;
    (void)strval;
    (void)add_ok_arr_to_root;
    (void)add_ok0;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_moved_ok;
    (void)size_before;
    (void)size_after;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}