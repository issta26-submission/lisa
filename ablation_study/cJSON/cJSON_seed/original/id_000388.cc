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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "base", 12.0);
    cJSON_AddNumberToObject(root, "factor", 0.5);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *base_item = cJSON_GetObjectItemCaseSensitive(root, "base");
    double base_val = cJSON_GetNumberValue(base_item);
    cJSON *factor_item = cJSON_GetObjectItemCaseSensitive(root, "factor");
    double factor_val = cJSON_GetNumberValue(factor_item);

    // step 3: Operate
    double product = base_val * factor_val;
    cJSON_AddNumberToObject(root, "product", product);
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    cJSON_AddNumberToObject(root, "enabled_numeric", (double)enabled_flag);

    // step 4: Validate and Cleanup
    cJSON *product_item = cJSON_GetObjectItemCaseSensitive(root, "product");
    double product_checked = cJSON_GetNumberValue(product_item);
    cJSON_AddNumberToObject(root, "product_checked_diff", product_checked - product);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}