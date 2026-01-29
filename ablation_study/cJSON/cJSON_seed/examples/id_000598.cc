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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name_item = cJSON_CreateString("fuzzer");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(config, "count", count_item);

    // step 2: Configure
    cJSON_bool has_name_original = cJSON_HasObjectItem(config, "name");
    cJSON_bool name_invalid_original = cJSON_IsInvalid(name_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed_config, "name");
    char *parsed_name = cJSON_GetStringValue(parsed_name_item);
    cJSON_bool has_name_parsed = cJSON_HasObjectItem(parsed_config, "name");
    cJSON_bool parsed_name_invalid = cJSON_IsInvalid(parsed_name_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig_has=%d;orig_invalid=%d;parsed_has=%d;parsed_name=%s;parsed_invalid=%d",
            version ? version : "null",
            (int)has_name_original,
            (int)name_invalid_original,
            (int)has_name_parsed,
            parsed_name ? parsed_name : "null",
            (int)parsed_name_invalid);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}