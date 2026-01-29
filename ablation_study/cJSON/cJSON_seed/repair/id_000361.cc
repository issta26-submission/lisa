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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *n3 = cJSON_CreateNumber(5.25);

    // step 2: Configure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddNullToObject(config, "optional");

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(config, "values");
    cJSON_bool is_array = cJSON_IsArray(got_values);
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    cJSON *second = cJSON_GetArrayItem(got_values, 1);
    double sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(sum / 3.0));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}