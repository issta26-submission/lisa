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
//<ID> 554
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *alpha = cJSON_AddNumberToObject(config, "alpha", 10.5);
    cJSON *beta = cJSON_CreateNumber(20.0);
    cJSON_AddItemToObject(config, "beta", beta);

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON *alpha_copy = cJSON_GetObjectItem(config_copy, "alpha");
    cJSON_SetNumberHelper(alpha_copy, 15.25);
    cJSON_AddNumberToObject(config_copy, "gamma", 7.75);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 3: Operate and Validate
    cJSON *alpha_orig = cJSON_GetObjectItem(config, "alpha");
    double v_orig = cJSON_GetNumberValue(alpha_orig);
    double v_copy = cJSON_GetNumberValue(alpha_copy);
    double v_gamma = cJSON_GetNumberValue(cJSON_GetObjectItem(config_copy, "gamma"));
    double total = v_orig + v_copy + v_gamma;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *full_dup = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(full_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(full_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}