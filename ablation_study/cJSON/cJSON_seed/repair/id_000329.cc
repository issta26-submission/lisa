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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(nested, "value", num_item);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    cJSON *pi_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *got_nested = cJSON_GetObjectItem(root, "nested");
    cJSON *got_value = cJSON_GetObjectItem(got_nested, "value");
    double value = cJSON_GetNumberValue(got_value);
    cJSON_bool value_gt_40 = (value > 40.0) ? 1 : 0;
    cJSON_AddBoolToObject(root, "value_gt_40", value_gt_40);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}