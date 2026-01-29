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
//<ID> 952
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
    cJSON *raw = cJSON_CreateRaw("{\"temp\":27.5}");
    cJSON *id = cJSON_CreateNumber(100.0);
    cJSON *name = cJSON_CreateString("device-01");

    // step 2: Configure
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(info, "id", id);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(root, "info", info);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_raw = cJSON_GetObjectItem(got_info, "raw");
    cJSON *got_id = cJSON_GetObjectItem(got_info, "id");
    double id_value = cJSON_GetNumberValue(got_id);
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    const char *version = cJSON_Version();
    cJSON_AddItemToObject(root, "id_value", cJSON_CreateNumber(id_value));
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(raw_flag));
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(version));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}