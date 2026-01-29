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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON *name = cJSON_CreateString("sensor-alpha");
    cJSON *name_dup = cJSON_Duplicate(name, 1);
    cJSON *threshold = cJSON_CreateNumber(3.14);

    // step 2: Configure
    cJSON_AddItemToObject(root, "metadata_raw", raw);
    cJSON_AddItemToObject(root, "name", name_dup);
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON_bool is_name_string = cJSON_IsString(got_name);
    cJSON_AddBoolToObject(root, "name_is_string", is_name_string);
    cJSON *raw_copy = cJSON_Duplicate(raw, 0);
    cJSON_AddItemToObject(config, "raw_copy", raw_copy);

    // step 4: Cleanup
    cJSON_Delete(name);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}