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
//<ID> 636
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
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr_to_root = 0;
    int arr_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double aggregate = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.71);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(retrieved);
    retrieved = cJSON_GetArrayItem(arr, 1);
    val1 = cJSON_GetNumberValue(retrieved);
    aggregate = val0 + val1 + (double)arr_size;

    // step 5: Validate
    (void)added_num1;
    (void)added_num2;
    (void)added_arr_to_root;
    (void)arr_size;
    (void)val0;
    (void)val1;
    (void)aggregate;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}