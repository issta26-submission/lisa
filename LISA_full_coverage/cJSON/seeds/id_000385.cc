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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON_AddBoolToObject(root, "valid", 1);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "count", 42.0);
    cJSON_AddBoolToObject(meta, "enabled", 0);

    // step 3: Operate
    cJSON *pi_item = cJSON_GetObjectItemCaseSensitive(root, "pi");
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(meta, "count");
    double pi_val = cJSON_GetNumberValue(pi_item);
    double count_val = cJSON_GetNumberValue(count_item);
    double total = pi_val + count_val;
    cJSON_AddNumberToObject(root, "total", total);

    // step 4: Validate and Cleanup
    cJSON *total_item = cJSON_GetObjectItemCaseSensitive(root, "total");
    double total_check = cJSON_GetNumberValue(total_item);
    cJSON_AddNumberToObject(root, "checked_total", total_check);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}