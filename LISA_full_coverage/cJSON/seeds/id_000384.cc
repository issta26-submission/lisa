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
//<ID> 384
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
    cJSON_AddNumberToObject(config, "threshold", 42.5);
    cJSON_AddBoolToObject(config, "enabled", 1);

    // step 2: Configure
    cJSON *thr_item = cJSON_GetObjectItemCaseSensitive(config, "threshold");
    double threshold = cJSON_GetNumberValue(thr_item);
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(config, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);

    // step 3: Operate and Validate
    double factor = 0.5 + 0.5 * (double)enabled_flag; // yields 1.0 if true, 0.5 if false
    double adjusted = threshold * factor;
    cJSON_AddNumberToObject(root, "adjusted_threshold", adjusted);
    cJSON *adj_item = cJSON_GetObjectItemCaseSensitive(root, "adjusted_threshold");
    double read_back = cJSON_GetNumberValue(adj_item);
    (void)read_back; // value used to demonstrate flow; no branching required

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}