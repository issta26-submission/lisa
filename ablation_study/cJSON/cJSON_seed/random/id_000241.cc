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
//<ID> 241
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *dup_second = (cJSON *)0;
    cJSON *got_array = (cJSON *)0;
    cJSON *got_second = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON *check_sum = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double total = 0.0;
    double check_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.718);
    num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Configure
    got_array = cJSON_GetObjectItem(root, "values");
    got_second = cJSON_GetArrayItem(got_array, 1);
    dup_second = cJSON_Duplicate(got_second, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "second", dup_second);

    // step 4: Operate
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(got_array, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(got_array, 1));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(got_array, 2));
    total = v0 + v1 + v2;
    sum_item = cJSON_AddNumberToObject(root, "sum", total);

    // step 5: Validate
    check_sum = cJSON_GetObjectItem(root, "sum");
    check_value = cJSON_GetNumberValue(check_sum);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)check_value;
    return 66;
}