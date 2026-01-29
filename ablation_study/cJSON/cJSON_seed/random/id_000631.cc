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
//<ID> 631
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    int arr_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double val2 = 0.0;
    double sum = 0.0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool attached = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.5);
    num2 = cJSON_CreateNumber(-2.25);
    num3 = cJSON_CreateNumber(10.0);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    added3 = cJSON_AddItemToArray(arr, num3);
    attached = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    val0 = cJSON_GetNumberValue(item0);
    val1 = cJSON_GetNumberValue(item1);
    val2 = cJSON_GetNumberValue(item2);
    sum = val0 + val1 + val2;

    // step 5: Validate
    (void)added1;
    (void)added2;
    (void)added3;
    (void)attached;
    (void)arr_size;
    (void)val0;
    (void)val1;
    (void)val2;
    (void)sum;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}