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
//<ID> 747
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
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(config, "count", count);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *detached_ver = cJSON_DetachItemFromObject(root, "version");
    double ver_val = cJSON_GetNumberValue(detached_ver);
    double count_val = cJSON_GetNumberValue(count);
    cJSON_AddItemToObject(config, "detached_version", detached_ver);
    cJSON_AddNumberToObject(config, "total", ver_val + count_val);
    cJSON_AddNumberToObject(config, "is_object_flag", (double)root_is_obj);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}