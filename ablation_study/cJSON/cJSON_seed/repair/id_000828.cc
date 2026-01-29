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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *seed_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(meta, "seed", seed_num);

    // step 2: Configure
    const char *json_text = "{\"name\":\"example\",\"value\":100}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    double is_str_val = (double)name_is_str;
    cJSON_AddNumberToObject(meta, "name_is_string", is_str_val);
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double parsed_value = cJSON_GetNumberValue(value_item);
    cJSON *incremented = cJSON_CreateNumber(parsed_value + 1.0);
    cJSON_AddItemToObject(root, "incremented", incremented);

    // step 3: Operate and Validate
    cJSON *detached_name = cJSON_DetachItemFromObject(parsed, "name");
    cJSON_AddItemToObject(root, "imported_name", detached_name);
    cJSON *detached_value = cJSON_DetachItemViaPointer(parsed, value_item);
    cJSON_AddItemToObject(root, "imported_value", detached_value);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}