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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *raw = cJSON_CreateRaw("{\"temp\":27.5}");
    cJSON *id = cJSON_CreateNumber(100);

    // step 2: Configure
    cJSON *dup_name = cJSON_Duplicate(name, 1);
    cJSON_AddItemToObject(info, "name", dup_name);
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(info, "id", id);
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name_copy = cJSON_Duplicate(name, 0);
    cJSON_AddItemToObject(root, "original_name", name_copy);
    cJSON_Delete(name);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_name = cJSON_GetObjectItem(got_info, "name");
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    cJSON_AddItemToObject(root, "name_is_string", cJSON_CreateBool(name_is_string));
    cJSON *info_copy = cJSON_Duplicate(info, 1);
    cJSON_AddItemToObject(root, "info_copy", info_copy);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    if (out) {
        cJSON_free(out);
    }
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}