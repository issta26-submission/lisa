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
//<ID> 1819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *child_null = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    const char *strval = (const char *)0;
    const char *null_str = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_str = cJSON_CreateString("hello");
    child_null = cJSON_CreateNull();

    // step 3: Configure
    arr_ref = cJSON_CreateArrayReference(child_str);
    add_ok1 = cJSON_AddItemToObject(root, "refarray", arr_ref);
    add_ok2 = cJSON_AddItemToObject(root, "nullval", child_null);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "refarray");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    arr_item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    strval = cJSON_GetStringValue(arr_item0);
    retrieved_null = cJSON_GetObjectItem(root, "nullval");
    null_str = cJSON_GetStringValue(retrieved_null);

    // step 5: Validate
    validation = (int)(retrieved_arr != (cJSON *)0) + (int)(arr_size == 1) + (int)(arr_item0 != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(retrieved_null != (cJSON *)0) + (int)(null_str == (const char *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)child_str;
    (void)child_null;
    (void)arr_ref;
    (void)retrieved_arr;
    (void)arr_item0;
    (void)retrieved_null;
    (void)strval;
    (void)null_str;
    (void)add_ok1;
    (void)add_ok2;
    (void)arr_size;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}