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
//<ID> 244
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.5);
    cJSON *n2 = cJSON_CreateNumber(-3.25);
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "constant", 1.0);
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "second_copy", second_val);
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "has_values_flag", (double)has_values);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}