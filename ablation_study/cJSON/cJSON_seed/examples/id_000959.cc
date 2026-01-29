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
//<ID> 959
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
    cJSON *name = cJSON_CreateString("deviceX");
    cJSON *temp = cJSON_CreateNumber(23.5);
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON *raw = cJSON_CreateRaw("{\"nested\":true}");

    // step 2: Configure
    cJSON_AddItemToObject(info, "name", cJSON_Duplicate(name, 1));
    cJSON_AddItemToObject(info, "temp", temp);
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(info, "id", id);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "original_name", name);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_temp = cJSON_GetObjectItem(got_info, "temp");
    double extracted_temp = cJSON_GetNumberValue(got_temp);
    cJSON_AddItemToObject(root, "extracted_temp", cJSON_CreateNumber(extracted_temp));
    cJSON *got_raw = cJSON_GetObjectItem(got_info, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(raw_flag));
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "cjson_version", cJSON_CreateString(ver));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}