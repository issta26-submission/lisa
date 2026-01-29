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
//<ID> 1310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"items\":[{\"raw\":true}], \"name\":\"root\"}";
    const char *parse_end = NULL;
    size_t json_len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *added_arr = NULL;
    cJSON *raw_item = NULL;
    cJSON *first_in_added = NULL;
    cJSON *name_item = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool compare_result = 0;
    const char *name_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    items = cJSON_GetObjectItem(root, "items");
    added_arr = cJSON_AddArrayToObject(root, "added_arr");
    raw_item = cJSON_CreateRaw("unparsed_raw_value");
    cJSON_AddItemToArray(added_arr, raw_item);
    first_in_added = cJSON_GetArrayItem(added_arr, 0);
    name_item = cJSON_GetObjectItem(root, "name");

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(first_in_added);
    compare_result = cJSON_Compare(items, added_arr, 0);
    name_str = cJSON_GetStringValue(name_item);
    (void)is_raw;
    (void)compare_result;
    (void)name_str;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}