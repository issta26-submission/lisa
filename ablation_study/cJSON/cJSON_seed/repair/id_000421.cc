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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(config, "x", 1.5);
    cJSON_AddNumberToObject(config, "y", 2.5);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    cJSON *x_item = cJSON_GetObjectItem(config, "x");
    cJSON *y_item = cJSON_GetObjectItem(config, "y");
    double x_val = cJSON_GetNumberValue(x_item);
    double y_val = cJSON_GetNumberValue(y_item);
    double sum_val = x_val + y_val;
    cJSON_AddNumberToObject(root, "sum", sum_val);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "root_is_object", (double)root_is_obj);

    // step 4: Validate and Cleanup
    cJSON *sum_item = cJSON_GetObjectItem(root, "sum");
    double retrieved_sum = cJSON_GetNumberValue(sum_item);
    double expected_sum = 1.5 + 2.5;
    cJSON_AddNumberToObject(root, "sum_difference", retrieved_sum - expected_sum);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}