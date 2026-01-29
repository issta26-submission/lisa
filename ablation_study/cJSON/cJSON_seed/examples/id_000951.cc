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
//<ID> 951
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
    cJSON *temp = cJSON_CreateNumber(23.5);
    cJSON *payload = cJSON_CreateRaw("{\"sensor\":true}");
    cJSON *name = cJSON_CreateString("device-42");

    // step 2: Configure
    cJSON_AddItemToObject(info, "temperature", temp);
    cJSON_AddItemToObject(info, "payload", payload);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(root, "info", info);
    const char *version = cJSON_Version();
    cJSON *ver_str = cJSON_CreateString(version);
    cJSON_AddItemToObject(root, "cjson_version", ver_str);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_temp = cJSON_GetObjectItem(got_info, "temperature");
    double extracted_temp = cJSON_GetNumberValue(got_temp);
    cJSON_AddItemToObject(root, "extracted_temperature", cJSON_CreateNumber(extracted_temp));
    cJSON *got_payload = cJSON_GetObjectItem(got_info, "payload");
    cJSON_bool is_raw = cJSON_IsRaw(got_payload);
    cJSON_AddItemToObject(root, "payload_is_raw", cJSON_CreateBool(is_raw));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}