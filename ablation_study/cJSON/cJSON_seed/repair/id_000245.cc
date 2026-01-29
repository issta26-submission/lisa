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
//<ID> 245
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.5));
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(values));
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "has_values", (double)has_values);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}