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
//<ID> 1518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddStringToObject(config, "name", "device");
    cJSON_AddNumberToObject(config, "version", 1.0);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", optional);
    const int nums[4] = {1, 2, 3, 4};
    cJSON *values = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(config, "values", values);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    const char parsed_text[] = "{\"temp\":23,\"status\":null}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(parsed_text, (size_t)(sizeof(parsed_text) - 1), &parse_end, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *duplicate = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON *cfg_from_root = cJSON_GetObjectItem(root, "config");
    cJSON *opt_from_cfg = cJSON_GetObjectItemCaseSensitive(cfg_from_root, "optional");
    cJSON_bool opt_is_null = cJSON_IsNull(opt_from_cfg);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "parsed");
    char *printed_dup = cJSON_PrintUnformatted(duplicate);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    (void)has_cfg;
    (void)opt_is_null;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed_dup);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}