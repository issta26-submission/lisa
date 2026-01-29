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
//<ID> 742
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
    cJSON *temp_false = (cJSON *)0;
    cJSON *enabled_item = (cJSON *)0;
    cJSON *added_bool_item = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    temp_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, temp_false);
    added_bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    enabled_item = cJSON_GetObjectItem(root, "enabled");
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (enabled_item != (cJSON *)0) + arr_size;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}