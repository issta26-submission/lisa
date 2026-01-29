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
//<ID> 613
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
    cJSON *str_ref = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;
    cJSON_bool add_ref_res = 0;
    cJSON_bool was_false = 0;
    const char *orig_str = "hello";
    const char *retrieved_str = (const char *)0;
    int arr_size = 0;
    double count_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_ref = cJSON_CreateStringReference(orig_str);
    num_item = cJSON_CreateNumber(42.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    add_res1 = cJSON_AddItemToArray(arr, str_ref);
    add_res2 = cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));
    add_ref_res = cJSON_AddItemReferenceToObject(root, "greeting_ref", str_ref);

    // step 4: Operate
    flag_item = cJSON_GetObjectItem(root, "flag");
    was_false = cJSON_IsFalse(flag_item);
    retrieved_ref = cJSON_GetObjectItem(root, "greeting_ref");
    retrieved_str = cJSON_GetStringValue(retrieved_ref);
    arr_size = cJSON_GetArraySize(arr);
    count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));

    // step 5: Validate
    (void)was_false;
    (void)add_res1;
    (void)add_res2;
    (void)add_ref_res;
    (void)retrieved_str;
    (void)arr_size;
    (void)count_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}