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
//<ID> 1794
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
    cJSON *item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *strval = (const char *)0;
    int size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    s1 = cJSON_CreateString("second");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToArray(arr, s1);
    add_ok3 = cJSON_AddItemToObject(root, "myarray", arr);
    cJSON_AddStringToObject(root, "note", "sample");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "myarray");
    size = cJSON_GetArraySize(retrieved_arr);
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    detached = cJSON_DetachItemViaPointer(retrieved_arr, item1);
    strval = cJSON_GetStringValue(detached);
    add_ok4 = cJSON_AddItemToObject(root, "detached", detached);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(size == 2) + (int)(strval != (const char *)0) + (int)(add_ok3 != 0) + (int)(add_ok4 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)arr;
    (void)s0;
    (void)s1;
    (void)item1;
    (void)detached;
    (void)strval;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)size;
    (void)retrieved_arr;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}