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
//<ID> 415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\": [1, 2, 3]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *orig_values = cJSON_GetObjectItem(root, "values");

    // step 2: Configure
    cJSON *new_values = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(new_values, n0);
    cJSON_AddItemToArray(new_values, n1);
    double updated_n1 = cJSON_SetNumberHelper(n1, 25.5);
    (void)updated_n1;
    cJSON_AddItemToObject(root, "new_values", new_values);

    // step 3: Operate
    double orig_v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_values, 0));
    double orig_v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_values, 1));
    double orig_v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_values, 2));
    double sum_orig = orig_v0 + orig_v1 + orig_v2;
    cJSON *new_vals_ref = cJSON_GetObjectItem(root, "new_values");
    double new_v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(new_vals_ref, 0));
    double new_v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(new_vals_ref, 1));
    double sum_new = new_v0 + new_v1;
    cJSON_AddNumberToObject(root, "sum_original", sum_orig);
    cJSON_AddNumberToObject(root, "sum_new", sum_new);

    // step 4: Validate and Cleanup
    double diff = sum_new - sum_orig;
    cJSON_AddNumberToObject(root, "difference", diff);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}