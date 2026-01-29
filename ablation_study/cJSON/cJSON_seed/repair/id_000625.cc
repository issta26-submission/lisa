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
//<ID> 625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *item_a = cJSON_AddNumberToObject(root, "a", 5.5);
    cJSON *item_b = cJSON_AddNumberToObject(root, "b", 4.5);
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");
    cJSON *active = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate and Validate
    double a_val = cJSON_GetNumberValue(item_a);
    double b_val = cJSON_GetNumberValue(item_b);
    double sum = a_val + b_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_bool active_is_bool = cJSON_IsBool(active);
    cJSON_AddBoolToObject(root, "active_is_bool", active_is_bool);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}