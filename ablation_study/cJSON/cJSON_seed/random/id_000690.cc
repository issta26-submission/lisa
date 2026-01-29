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
//<ID> 690
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    cJSON *ref_arr = (cJSON *)0;
    cJSON *ret_n1 = (cJSON *)0;
    cJSON *ret_data_ref = (cJSON *)0;
    int idx0_value_int = 0;
    double val_n1 = 0.0;
    double val_idx0 = 0.0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_num1_ref = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool replaced = 0;
    cJSON_bool added_ref_arr = 0;
    cJSON_bool is_array_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    added_arr_to_root = cJSON_AddItemToObject(root, "data", arr);
    added_num1_ref = cJSON_AddItemReferenceToObject(root, "n1", num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    newnum = cJSON_CreateNumber(99.0);
    replaced = cJSON_ReplaceItemViaPointer(arr, num2, newnum);
    ref_arr = cJSON_GetObjectItem(root, "data");
    is_array_flag = cJSON_IsArray(ref_arr);
    added_ref_arr = cJSON_AddItemReferenceToObject(root, "data_ref", ref_arr);
    ret_n1 = cJSON_GetObjectItem(root, "n1");
    val_n1 = cJSON_GetNumberValue(ret_n1);
    ret_data_ref = cJSON_GetObjectItem(root, "data_ref");
    val_idx0 = cJSON_GetNumberValue(cJSON_GetArrayItem(ret_data_ref, 0));
    idx0_value_int = (int)val_idx0;

    // step 5: Validate
    (void)added_arr_to_root;
    (void)added_num1_ref;
    (void)added_num2;
    (void)replaced;
    (void)added_ref_arr;
    (void)is_array_flag;
    (void)val_n1;
    (void)val_idx0;
    (void)idx0_value_int;
    (void)ref_arr;
    (void)ret_n1;
    (void)ret_data_ref;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}