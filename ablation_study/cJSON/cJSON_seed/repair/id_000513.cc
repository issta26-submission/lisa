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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"name\":\"device\",\"meta\":{\"version\":1},\"flags\":{\"enabled\":false}}";
    size_t json_len = strlen(initial_json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    double version = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "version"));
    cJSON *config_obj = cJSON_CreateObject();
    cJSON *status_str = cJSON_CreateString("configured");
    cJSON_AddItemToObject(config_obj, "status", status_str);
    cJSON_AddItemToObject(root, "config", config_obj);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate
    cJSON_bool flags_present = cJSON_HasObjectItem(root, "flags");
    cJSON_AddBoolToObject(root, "flags_present", flags_present);
    cJSON *flags = cJSON_GetObjectItem(root, "flags");
    cJSON *enabled_item = cJSON_GetObjectItem(flags, "enabled");
    cJSON_bool enabled_bool = cJSON_IsTrue(enabled_item);
    double multiplier = 1.0 + (double)enabled_bool;
    double scaled_version = version * multiplier;
    cJSON_AddItemToObject(root, "scaled_version", cJSON_CreateNumber(scaled_version));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON_bool config_exists = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(root, "config_exists", config_exists);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}