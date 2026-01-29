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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *old_arr = cJSON_AddArrayToObject(root, "numbers");
    double initial_vals[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_array = cJSON_CreateDoubleArray(initial_vals, 3);

    // step 2: Configure
    cJSON *parsed = cJSON_Parse("{\"meta\":\"demo-string\"}");
    cJSON *meta_item = cJSON_GetObjectItem(parsed, "meta");
    const char *meta_str = cJSON_GetStringValue(meta_item);
    cJSON_AddStringToObject(root, "meta", meta_str);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, old_arr, dbl_array);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *n0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *n1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *n2 = cJSON_GetArrayItem(numbers, 2);
    double sum = cJSON_GetNumberValue(n0) + cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2);
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}