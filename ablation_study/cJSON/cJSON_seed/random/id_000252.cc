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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *num_obj = (cJSON *)0;
    cJSON *str_obj = (cJSON *)0;
    cJSON *tmp_item = (cJSON *)0;
    cJSON *sum_obj = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int nums[3] = {1, 2, 3};
    int arr_size = 0;
    double total = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 3);

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "int_array", int_arr);
    num_obj = cJSON_CreateNumber(42.0);
    add_ok = cJSON_AddItemToObject(root, "answer", num_obj);
    str_obj = cJSON_AddStringToObject(root, "note", "example note");

    // step 4: Operate
    arr_size = cJSON_GetArraySize(int_arr);
    tmp_item = cJSON_GetArrayItem(int_arr, 1);
    total = cJSON_GetNumberValue(tmp_item);
    total = total + cJSON_GetNumberValue(num_obj);

    // step 5: Validate
    sum_obj = cJSON_CreateNumber(total + (double)arr_size);
    add_ok = cJSON_AddItemToObject(root, "total", sum_obj);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}