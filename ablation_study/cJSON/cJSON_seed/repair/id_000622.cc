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
//<ID> 622
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
    cJSON_AddNumberToObject(metrics, "a", 5.5);
    cJSON_AddNumberToObject(metrics, "b", 2.5);
    cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON_AddBoolToObject(root, "flag", 1);
    cJSON *flag_ptr = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_ptr);
    cJSON_AddNumberToObject(root, "flag_is_bool", (double)flag_is_bool);

    // step 3: Operate and Validate
    cJSON *a_ptr = cJSON_GetObjectItem(metrics, "a");
    cJSON *b_ptr = cJSON_GetObjectItem(metrics, "b");
    double a_val = cJSON_GetNumberValue(a_ptr);
    double b_val = cJSON_GetNumberValue(b_ptr);
    double sum = a_val + b_val;
    double average = sum / 2.0;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "average", average);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}