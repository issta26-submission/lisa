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
//<ID> 1537
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

    // step 2: Configure
    cJSON *name_item = cJSON_AddStringToObject(config, "name", "fuzz_module");
    cJSON *enabled_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled_flag);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *retries = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "retries", retries);

    // step 3: Operate and Validate
    double tval = cJSON_GetNumberValue(threshold);
    double rval = cJSON_GetNumberValue(retries);
    double sum = tval + rval;
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}