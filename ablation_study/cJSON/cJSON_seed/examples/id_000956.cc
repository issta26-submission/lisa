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
//<ID> 956
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
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON *raw = cJSON_CreateRaw("{\"inner\":7}");
    cJSON *name = cJSON_CreateString("sensor-1");

    // step 2: Configure
    cJSON_AddItemToObject(info, "value", num);
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(root, "info", info);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_value = cJSON_GetObjectItem(got_info, "value");
    cJSON *got_raw = cJSON_GetObjectItem(got_info, "raw");
    double extracted_value = cJSON_GetNumberValue(got_value);
    cJSON_bool raw_is_raw = cJSON_IsRaw(got_raw);
    cJSON_AddItemToObject(root, "extracted_value", cJSON_CreateNumber(extracted_value));
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(raw_is_raw));
    const char *version_str = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(version_str));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}