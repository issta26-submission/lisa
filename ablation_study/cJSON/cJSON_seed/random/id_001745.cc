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
//<ID> 1745
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    char *retrieved_str_value = (char *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool is_num = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str0 = cJSON_CreateString("first");
    str1 = cJSON_CreateString("second");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num_item);
    add_ok = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    retrieved_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    is_num = cJSON_IsNumber(retrieved_count);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_str_value = cJSON_GetStringValue(retrieved_elem);

    // step 5: Validate
    validation = (int)(is_num != 0) + (int)(arr_size == 3) + (int)(retrieved_str_value != (char *)0) + (int)(add_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num_item;
    (void)str0;
    (void)str1;
    (void)retrieved_arr;
    (void)retrieved_elem;
    (void)retrieved_count;
    (void)retrieved_str_value;
    (void)arr_size;
    (void)is_num;
    (void)add_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}