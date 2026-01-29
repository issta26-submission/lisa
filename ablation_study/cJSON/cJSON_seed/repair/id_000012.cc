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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *falseItem = cJSON_CreateFalse();
    cJSON *parsed = cJSON_Parse("{\"alpha\":123.0,\"beta\":[true,false]}");

    // step 2: Configure
    cJSON *arr = cJSON_AddArrayToObject(root, "combined");
    cJSON_AddItemToArray(arr, parsed);
    cJSON_AddItemToArray(arr, falseItem);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *alpha = cJSON_GetObjectItem(first, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool first_is_obj = cJSON_IsObject(first);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    int validation_sum = (int)first_is_obj + (int)second_is_bool + (int)alpha_val;
    cJSON_SetNumberHelper(alpha, (double)validation_sum);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}