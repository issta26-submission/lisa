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
//<ID> 550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *initial_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "initial", initial_num);
    cJSON *added_num = cJSON_AddNumberToObject(metrics, "added", 3.5);

    // step 2: Configure
    cJSON *metrics_dup = cJSON_Duplicate(metrics, 1);
    cJSON *init_dup = cJSON_GetObjectItem(metrics_dup, "initial");
    double init_val = cJSON_GetNumberValue(init_dup);
    double doubled = cJSON_SetNumberHelper(init_dup, init_val * 2.0);
    cJSON_AddNumberToObject(metrics_dup, "initial_doubled", doubled);

    // step 3: Operate and Validate
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *metrics_in_dup = cJSON_GetObjectItem(root_dup, "metrics");
    cJSON *initial_in_dup = cJSON_GetObjectItem(metrics_in_dup, "initial");
    cJSON *added_in_dup = cJSON_GetObjectItem(metrics_in_dup, "added");
    double sum = cJSON_GetNumberValue(initial_in_dup) + cJSON_GetNumberValue(added_in_dup);
    cJSON_AddNumberToObject(root_dup, "sum", sum);
    char *out = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(metrics_dup);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}