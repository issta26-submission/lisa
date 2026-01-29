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
//<ID> 121
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

    // step 2: Configure
    cJSON *pi_manual = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(metrics, "pi_manual", pi_manual);
    cJSON *pi_auto = cJSON_AddNumberToObject(metrics, "pi_auto", 3.141592653589793);
    cJSON_AddNumberToObject(root, "sample_count", 42.0);

    // step 3: Operate & Validate
    cJSON *metrics_lookup = cJSON_GetObjectItem(root, "metrics");
    cJSON *pi_lookup = cJSON_GetObjectItem(metrics_lookup, "pi_auto");
    double pi_value = cJSON_GetNumberValue(pi_lookup);
    cJSON_AddNumberToObject(root, "pi_half", pi_value * 0.5);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}