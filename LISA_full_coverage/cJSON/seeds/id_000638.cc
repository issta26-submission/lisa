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
//<ID> 638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    const char *raw_settings = "{\"enabled\":true, \"threshold\":3.14}";
    cJSON *raw_item = cJSON_AddRawToObject(config, "settings_raw", raw_settings);
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_bool is_config_obj = cJSON_IsObject(config);
    cJSON_bool is_dup_obj = cJSON_IsObject(dup_config);

    // step 3: Operate
    cJSON_bool equal = cJSON_Compare(config, dup_config, 1);
    char *pretty = cJSON_Print(root);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Validate & Cleanup
    (void)is_config_obj;
    (void)is_dup_obj;
    (void)equal;
    cJSON_free(pretty);
    cJSON_Delete(dup_config);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}