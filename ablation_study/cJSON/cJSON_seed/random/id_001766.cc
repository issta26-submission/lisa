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
//<ID> 1766
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
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    int arr_size = 0;
    double num_value = 0.0;
    cJSON_bool is_arr = 0;
    cJSON_bool add_ok_arr0 = 0;
    cJSON_bool add_ok_arr1 = 0;
    cJSON_bool add_ok_obj_arr = 0;
    cJSON_bool add_ok_obj_num = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");
    num = cJSON_CreateNumber(3.140000);

    // step 3: Configure
    add_ok_arr0 = cJSON_AddItemToArray(arr, s0);
    add_ok_arr1 = cJSON_AddItemToArray(arr, s1);
    add_ok_obj_arr = cJSON_AddItemToObject(root, "letters", arr);
    add_ok_obj_num = cJSON_AddItemToObject(root, "pi", num);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    is_arr = cJSON_IsArray(retrieved_arr);
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    num_value = cJSON_GetNumberValue(retrieved_num);
    cJSON_DeleteItemFromObject(root, "letters");

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(is_arr != 0) + (int)(num_value == 3.140000) + (int)(add_ok_arr0 != 0) + (int)(add_ok_arr1 != 0) + (int)(add_ok_obj_arr != 0) + (int)(add_ok_obj_num != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)num;
    (void)retrieved_arr;
    (void)retrieved_num;
    (void)arr_size;
    (void)num_value;
    (void)is_arr;
    (void)add_ok_arr0;
    (void)add_ok_arr1;
    (void)add_ok_obj_arr;
    (void)add_ok_obj_num;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}