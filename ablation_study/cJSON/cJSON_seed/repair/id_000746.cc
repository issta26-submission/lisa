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
//<ID> 746
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
    cJSON *num_alpha = cJSON_AddNumberToObject(config, "alpha", 3.14);
    cJSON *num_beta = cJSON_AddNumberToObject(config, "beta", 2.0);

    // step 2: Configure
    double alpha_val = cJSON_GetNumberValue(num_alpha);
    cJSON_bool config_is_object = cJSON_IsObject(config);
    double computed_val = alpha_val + (double)config_is_object;
    cJSON *computed_num = cJSON_AddNumberToObject(root, "computed", computed_val);

    // step 3: Operate and Validate
    cJSON *detached_beta = cJSON_DetachItemFromObject(config, "beta");
    double beta_val = cJSON_GetNumberValue(detached_beta);
    cJSON_AddItemToObject(root, "beta_backup", detached_beta);
    double computed_ref = cJSON_GetNumberValue(computed_num);
    (void)beta_val;
    (void)computed_ref;

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}