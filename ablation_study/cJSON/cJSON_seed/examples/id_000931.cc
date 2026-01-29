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
//<ID> 931
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *device = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("sensor-7");
    cJSON *value = cJSON_CreateNumber(42.0);
    cJSON *raw = cJSON_CreateRaw("{\"calibrated\":false}");
    cJSON *name_dup = cJSON_Duplicate(name, 1);

    // step 2: Configure
    cJSON_AddItemToObject(device, "name", name);
    cJSON_AddItemToObject(device, "reading", value);
    cJSON_AddItemToObject(root, "device", device);
    cJSON_AddItemToObject(root, "meta_raw", raw);
    cJSON_AddItemToObject(root, "alias", name_dup);

    // step 3: Operate & Validate
    cJSON *got_device = cJSON_GetObjectItem(root, "device");
    cJSON *got_name = cJSON_GetObjectItem(got_device, "name");
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_string);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItem(dup_root, "meta_raw");
    char *meta_raw_text = cJSON_PrintUnformatted(dup_meta);
    cJSON_free(meta_raw_text);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    return 66;
    // API sequence test completed successfully
}