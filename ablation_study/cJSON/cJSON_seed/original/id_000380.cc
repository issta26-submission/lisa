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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "alpha", 3.5);
    cJSON_AddNumberToObject(root, "beta", 2.0);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddBoolToObject(root, "enabled", 0);

    // step 2: Configure
    cJSON *alpha = cJSON_GetObjectItemCaseSensitive(root, "alpha");
    cJSON *beta = cJSON_GetObjectItemCaseSensitive(root, "beta");
    double a = cJSON_GetNumberValue(alpha);
    double b = cJSON_GetNumberValue(beta);

    // step 3: Operate and Validate
    double sum = a + b;
    double ratio = a / b;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "ratio", ratio);
    cJSON_AddBoolToObject(root, "sum_gt_5", (sum > 5));
    cJSON *sum_item = cJSON_GetObjectItemCaseSensitive(root, "sum");
    cJSON *ratio_item = cJSON_GetObjectItemCaseSensitive(root, "ratio");
    double verified_sum = cJSON_GetNumberValue(sum_item);
    double verified_ratio = cJSON_GetNumberValue(ratio_item);
    cJSON_AddNumberToObject(root, "verified_sum", verified_sum);
    cJSON_AddNumberToObject(root, "verified_ratio", verified_ratio);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}