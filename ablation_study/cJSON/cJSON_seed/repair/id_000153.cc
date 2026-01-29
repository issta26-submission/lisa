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
//<ID> 153
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
    cJSON *n0 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    cJSON *offset = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "offset", offset);
    cJSON *bonus = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, bonus);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 2));
    double sum = v0 + v1 + v2;
    double off = cJSON_GetNumberValue(offset);
    double adjusted = sum + off;
    cJSON *adjusted_item = cJSON_CreateNumber(adjusted);
    cJSON_AddItemToObject(root, "adjusted_sum", adjusted_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}