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
//<ID> 1765
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
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_data = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    cJSON_bool is_array = 0;
    double extracted_number = 0.0;
    cJSON_bool ok_add_arr1 = 0;
    cJSON_bool ok_add_arr2 = 0;
    cJSON_bool ok_add_obj = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");
    num_item = cJSON_CreateNumber(3.14);

    // step 3: Configure
    ok_add_arr1 = cJSON_AddItemToArray(arr, s0);
    ok_add_arr2 = cJSON_AddItemToArray(arr, s1);
    ok_add_obj = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 4: Operate
    retrieved_data = cJSON_GetObjectItem(root, "data");
    is_array = cJSON_IsArray(retrieved_data);
    retrieved_value = cJSON_GetObjectItem(root, "value");
    extracted_number = cJSON_GetNumberValue(retrieved_value);
    cJSON_DeleteItemFromObject(root, "value");

    // step 5: Validate
    validation = (int)(is_array != 0) + (int)(extracted_number == 3.14) + (int)(ok_add_arr1 != 0) + (int)(ok_add_arr2 != 0) + (int)(ok_add_obj != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)num_item;
    (void)retrieved_data;
    (void)retrieved_value;
    (void)is_array;
    (void)extracted_number;
    (void)ok_add_arr1;
    (void)ok_add_arr2;
    (void)ok_add_obj;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}