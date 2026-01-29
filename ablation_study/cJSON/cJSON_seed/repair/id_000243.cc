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
//<ID> 243
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
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(values, num0);
    cJSON_AddItemToArray(values, num1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "total_count", (double)cJSON_GetArraySize(values));
    double second_element = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    cJSON_AddNumberToObject(root, "second_element", second_element);

    // step 3: Operate and Validate
    cJSON_bool has_vals = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "has_values_flag", (double)has_vals);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}