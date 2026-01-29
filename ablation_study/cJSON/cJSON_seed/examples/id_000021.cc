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
//<ID> 21
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
    cJSON *threshold = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", optional);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(config, buffer, 256, 1);

    // step 3: Operate & Validate
    double val = cJSON_GetNumberValue(threshold);
    double val2 = val + (double)printed;
    cJSON *threshold_plus = cJSON_CreateNumber(val2);
    cJSON_AddItemToObject(config, "threshold_plus", threshold_plus);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON *fetched = cJSON_GetObjectItem(config, "threshold");
    double fetched_val = cJSON_GetNumberValue(fetched);
    double final_check = fetched_val + (double)equal;
    cJSON *check_num = cJSON_CreateNumber(final_check);
    cJSON_AddItemToObject(root, "final_check", check_num);

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}