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
//<ID> 1793
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *strval = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok_det = 0;
    int size_before = 0;
    int size_after = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");
    s2 = cJSON_CreateString("three");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToArray(arr, s1);
    add_ok3 = cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    size_before = cJSON_GetArraySize(retrieved_arr);
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    detached = cJSON_DetachItemViaPointer(retrieved_arr, item1);
    add_ok_det = cJSON_AddItemToObject(root, "detached", detached);
    strval = cJSON_GetStringValue(detached);
    size_after = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    validation = (int)(size_before == 3) + (int)(size_after == 2) + (int)(strval != (char *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0) + (int)(add_ok_det != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)s2;
    (void)retrieved_arr;
    (void)item1;
    (void)detached;
    (void)strval;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok_det;
    (void)size_before;
    (void)size_after;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}