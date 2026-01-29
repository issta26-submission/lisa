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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "[10,20]";
    cJSON *root = cJSON_Parse(json);
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(6.28);
    cJSON_AddItemToArray(new_arr, n1);
    cJSON_AddItemToArray(new_arr, n2);

    // step 2: Configure
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 1));
    double sum = v0 + v1;
    cJSON_SetNumberHelper(n2, sum);

    // step 3: Operate
    cJSON_AddItemToArray(root, new_arr);
    cJSON *n3 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(n3, 42.0);
    cJSON_AddItemToArray(root, n3);

    // step 4: Validate and Cleanup
    cJSON *nested = cJSON_GetArrayItem(root, 2);
    cJSON *nested_second = cJSON_GetArrayItem(nested, 1);
    double extracted = cJSON_GetNumberValue(nested_second);
    (void)extracted;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}