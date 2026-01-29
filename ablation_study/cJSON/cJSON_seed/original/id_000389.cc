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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    const cJSON *pi_lookup = cJSON_GetObjectItemCaseSensitive(root, "pi");
    const cJSON *enabled_lookup = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    double pi_value = cJSON_GetNumberValue(pi_lookup);
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_lookup);
    cJSON *pi_times_two = cJSON_AddNumberToObject(root, "pi_times_two", pi_value * 2.0);
    cJSON *enabled_as_number = cJSON_AddNumberToObject(root, "enabled_as_number", (double)enabled_flag);

    // step 3: Operate
    const cJSON *double_lookup = cJSON_GetObjectItemCaseSensitive(root, "pi_times_two");
    double double_value = cJSON_GetNumberValue(double_lookup);
    double combined = pi_value + double_value;
    cJSON *combined_item = cJSON_AddNumberToObject(root, "combined_pi", combined);

    // step 4: Validate and Cleanup
    const cJSON *combined_lookup = cJSON_GetObjectItemCaseSensitive(root, "combined_pi");
    double combined_verify = cJSON_GetNumberValue(combined_lookup);
    cJSON *verified = cJSON_AddNumberToObject(root, "verified_combined_pi", combined_verify);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}