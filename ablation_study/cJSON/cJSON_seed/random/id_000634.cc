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
//<ID> 634
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
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr_to_root = 0;
    int size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    size = cJSON_GetArraySize(arr);
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(retrieved0);
    val1 = cJSON_GetNumberValue(retrieved1);
    sum = val0 + val1;

    // step 5: Validate
    (void)added1;
    (void)added2;
    (void)added_arr_to_root;
    (void)size;
    (void)val0;
    (void)val1;
    (void)sum;
    (void)retrieved0;
    (void)retrieved1;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}