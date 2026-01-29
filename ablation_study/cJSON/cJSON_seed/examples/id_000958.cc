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
//<ID> 958
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
    cJSON *id = cJSON_CreateNumber(123.456);
    cJSON *raw = cJSON_CreateRaw("{\"sensor\":true,\"val\":27.5}");
    cJSON *name = cJSON_CreateString("sensor-abc");

    // step 2: Configure
    cJSON_AddItemToObject(info, "id", id);
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddStringToObject(root, "version", cJSON_Version());
    cJSON *raw_flag = cJSON_CreateBool(cJSON_IsRaw(raw));
    cJSON_AddItemToObject(root, "raw_is_raw", raw_flag);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_id = cJSON_GetObjectItem(got_info, "id");
    double extracted_id = cJSON_GetNumberValue(got_id);
    cJSON_AddItemToObject(root, "extracted_id", cJSON_CreateNumber(extracted_id));
    cJSON *got_name = cJSON_GetObjectItem(got_info, "name");
    cJSON *name_copy = cJSON_Duplicate(got_name, 0);
    cJSON_AddItemToObject(root, "original_name_copy", name_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}