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
//<ID> 240
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
    cJSON *n1 = cJSON_CreateNumber(1.1);
    cJSON *n2 = cJSON_CreateNumber(2.2);
    cJSON *n3 = cJSON_CreateNumber(3.3);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    int size = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "count", (double)size);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "second_value", second_val);

    // step 3: Operate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "values_present", (double)has_values);
    cJSON *second_copy = cJSON_Duplicate(second, 0);
    cJSON_AddItemToObject(root, "second_copy", second_copy);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}