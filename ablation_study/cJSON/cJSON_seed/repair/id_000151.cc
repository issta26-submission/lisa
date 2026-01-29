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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(numbers, n0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *active_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active_flag);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *it1 = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double sum = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}