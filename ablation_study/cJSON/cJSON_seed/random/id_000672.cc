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
//<ID> 672
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
    cJSON *name = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr_to_obj = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    char *name_str = (char *)0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    name = cJSON_CreateString("sensor-A");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, num0);
    added1 = cJSON_AddItemToArray(arr, num1);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "name", name);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 0);
    val0 = cJSON_GetNumberValue(retrieved_item);
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    val1 = cJSON_GetNumberValue(retrieved_item);
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);
    sum = val0 + val1;

    // step 5: Validate
    nonnull_count = (name_str != (char *)0) + (int)(sum == sum);

    // step 6: Cleanup
    (void)added0;
    (void)added1;
    (void)added_arr_to_obj;
    (void)nonnull_count;
    (void)val0;
    (void)val1;
    (void)sum;
    (void)name_str;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}