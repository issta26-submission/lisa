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
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);

    // step 2: Configure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(values));

    // step 3: Operate and Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "has_values", (double)has_values);
    cJSON *middle_item = cJSON_GetArrayItem(values, 1);
    double middle_value = cJSON_GetNumberValue(middle_item);
    cJSON_AddNumberToObject(root, "middle", middle_value);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "inner", 42.0);
    cJSON_AddItemToObject(root, "child", child);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}