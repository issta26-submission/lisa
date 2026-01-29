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
//<ID> 989
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
    cJSON *enabled = cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddNumberToObject(config, "threshold", 10.0);
    cJSON_AddStringToObject(config, "mode", "automatic");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.2.3");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate & Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(got_enabled);
    cJSON *override_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "override", override_flag);
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_threshold = cJSON_GetObjectItem(got_config, "threshold");
    double current_threshold = cJSON_GetNumberValue(got_threshold);
    double updated_threshold = cJSON_SetNumberHelper(got_threshold, current_threshold + (double)was_false);
    (void)updated_threshold;
    cJSON *got_mode = cJSON_GetObjectItem(got_config, "mode");
    char *mode_str = cJSON_GetStringValue(got_mode);
    cJSON *mode_copy = cJSON_CreateString(mode_str);
    cJSON_AddItemToObject(root, "mode_copy", mode_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}