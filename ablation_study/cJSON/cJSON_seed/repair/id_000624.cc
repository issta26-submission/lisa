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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *valA = cJSON_AddNumberToObject(metrics, "a", 4.5);
    cJSON *valB = cJSON_AddNumberToObject(metrics, "b", 1.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *missing = cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    double a_val = cJSON_GetNumberValue(valA);
    double b_val = cJSON_GetNumberValue(valB);
    double sum = a_val + b_val;
    cJSON_AddNumberToObject(metrics, "sum", sum);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);

    // step 3: Operate and Validate
    double ratio = a_val / b_val;
    cJSON_AddNumberToObject(root, "ratio", ratio);
    cJSON_AddNumberToObject(root, "flag_is_bool", (double)flag_is_bool);
    cJSON_AddNumberToObject(root, "computed_average", sum / 2.0);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}