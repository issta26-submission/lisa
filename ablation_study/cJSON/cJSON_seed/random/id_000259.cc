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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON_bool add_ok_ints = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    int numbers[3];
    int array_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    numbers[0] = 5;
    numbers[1] = 10;
    numbers[2] = 15;
    root = cJSON_CreateObject();
    ints_arr = cJSON_CreateIntArray(numbers, 3);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_ok_ints = cJSON_AddItemToObject(root, "ints", ints_arr);
    str_item = cJSON_AddStringToObject(root, "description", "sample integer array");
    add_ok_num = cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "ints");
    array_size = cJSON_GetArraySize(retrieved_arr);
    arr_item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    val0 = cJSON_GetNumberValue(arr_item0);
    arr_item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    val1 = cJSON_GetNumberValue(arr_item1);
    sum = val0 + val1 + cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "partial_sum", sum_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}