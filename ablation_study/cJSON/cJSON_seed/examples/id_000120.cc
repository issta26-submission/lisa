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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    cJSON *num_pi = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(stats, "pi_val", num_pi);
    cJSON *threshold_item = cJSON_AddNumberToObject(root, "threshold", 10.0);

    // step 3: Operate & Validate
    cJSON *fetched_pi = cJSON_GetObjectItem(stats, "pi_val");
    cJSON *fetched_threshold = cJSON_GetObjectItem(root, "threshold");
    double pi_value = cJSON_GetNumberValue(fetched_pi);
    double threshold_value = cJSON_GetNumberValue(fetched_threshold);
    double avg_value = (pi_value + threshold_value) / 2.0;
    cJSON_AddNumberToObject(root, "average", avg_value);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}