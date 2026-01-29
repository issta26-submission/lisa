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
//<ID> 630
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
    cJSON *num3 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int arr_size = 0;
    double v1 = 0.0;
    double v2 = 0.0;
    double total = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(20.25);
    num3 = cJSON_CreateNumber(-3.75);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(num1);
    v2 = cJSON_GetNumberValue(retrieved);
    total = v1 + v2;

    // step 5: Validate
    (void)added1;
    (void)added2;
    (void)arr_size;
    (void)total;
    (void)root;
    (void)num1;
    (void)num2;
    (void)num3;
    (void)retrieved;
    (void)v1;
    (void)v2;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}