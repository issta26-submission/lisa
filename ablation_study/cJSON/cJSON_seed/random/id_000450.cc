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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *val = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "value", val);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double set_first = cJSON_SetNumberHelper(first, 42.0);
    double set_val = cJSON_SetNumberHelper(val, 3.1415);
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");

    // step 4: Validate
    int validation = 0;
    validation ^= (int)set_first;
    validation ^= (int)set_val;
    validation ^= (int)has_numbers;
    (void)validation;

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}