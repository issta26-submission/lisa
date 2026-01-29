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
//<ID> 426
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
    cJSON_AddNumberToObject(config, "alpha", 1.5);
    cJSON_AddNumberToObject(config, "beta", 2.5);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("fuzz-test");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 3: Operate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    double is_obj_value = (double)root_is_obj;
    cJSON_AddNumberToObject(root, "is_object", is_obj_value);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *alpha_item = cJSON_GetObjectItem(cfg, "alpha");
    cJSON *beta_item = cJSON_GetObjectItem(cfg, "beta");
    double a = cJSON_GetNumberValue(alpha_item);
    double b = cJSON_GetNumberValue(beta_item);
    double sum = a + b + is_obj_value;
    cJSON_AddNumberToObject(root, "sum_params", sum);

    // step 4: Validate and Cleanup
    double retrieved_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum_params"));
    (void)retrieved_sum;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}