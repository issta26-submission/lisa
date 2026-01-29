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
//<ID> 422
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
    cJSON *label = cJSON_CreateString("example_config");
    cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON_AddItemToObject(config, "label", label);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "count", 5.0);
    cJSON_AddNumberToObject(stats, "sum", 42.5);
    cJSON_AddItemToObject(root, "stats", stats);

    // step 3: Operate
    cJSON_bool is_root_obj = cJSON_IsObject(root);
    cJSON_bool is_config_obj = cJSON_IsObject(cJSON_GetObjectItem(root, "config"));
    double sum_val = cJSON_GetNumberValue(cJSON_GetObjectItem(stats, "sum"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(stats, "count"));
    double average = sum_val / count_val;
    cJSON_AddNumberToObject(root, "average", average);
    double retrieved_avg = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "average"));

    // step 4: Validate and Cleanup
    (void)is_root_obj;
    (void)is_config_obj;
    (void)retrieved_avg;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}