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
//<ID> 321
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

    // step 2: Configure
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *pi = cJSON_CreateNumber(3.1415926535);
    cJSON_AddItemToObject(metrics, "pi", pi);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate and Validate
    cJSON *metrics_ref = cJSON_GetObjectItem(root, "metrics");
    cJSON *pi_ref = cJSON_GetObjectItem(metrics_ref, "pi");
    double pi_val = cJSON_GetNumberValue(pi_ref);
    cJSON *pi_next = cJSON_CreateNumber(pi_val + 1.0);
    cJSON_AddItemToObject(metrics_ref, "pi_next", pi_next);
    double active_num = cJSON_GetNumberValue(active_item);
    (void)active_num;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}