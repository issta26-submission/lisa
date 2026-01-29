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
//<ID> 1217
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
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n_base = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(config, "pi", n_base);
    cJSON *n_extra = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n_extra);

    // step 2: Configure
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_dup);
    cJSON *detached_pi = cJSON_DetachItemFromObject(config, "pi");
    cJSON_AddItemToArray(values, detached_pi);

    // step 3: Operate and Validate
    cJSON *dup_pi = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "config_copy"), "pi");
    double dup_pi_val = cJSON_GetNumberValue(dup_pi);
    cJSON *first_in_values = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_in_values);
    double combined = dup_pi_val + first_val;
    cJSON *combined_num = cJSON_CreateNumber(combined);
    cJSON_AddItemToArray(values, combined_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = out ? out[1] : 0;
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}