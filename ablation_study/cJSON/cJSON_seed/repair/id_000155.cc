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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *num0 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, num0);
    cJSON *num1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(values, num1);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(values, 0);
    cJSON *it1 = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double total = v0 + v1;
    cJSON *total_num = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", total_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}