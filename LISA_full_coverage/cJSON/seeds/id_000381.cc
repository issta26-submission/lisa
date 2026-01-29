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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double base_value = 3.14159;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", base_value);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *pi_item = cJSON_GetObjectItemCaseSensitive(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);

    // step 3: Operate and Validate
    double computed = pi_val * (1.0 + (double)enabled_flag);
    cJSON_AddNumberToObject(root, "computed", computed);
    cJSON *computed_item = cJSON_GetObjectItemCaseSensitive(root, "computed");
    double validated = cJSON_GetNumberValue(computed_item);

    // step 4: Cleanup
    (void)validated;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}