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
//<ID> 1317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"arr\":[1,2,3],\"raw\":\"<raw>content</raw>\"}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *added_arr = NULL;
    cJSON *created_raw = NULL;
    cJSON *raw_item = NULL;
    cJSON *name_item = NULL;
    cJSON *dup = NULL;
    const char *name_value = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool same = 0;
    cJSON_bool add_ret = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    arr = cJSON_GetObjectItem(root, "arr");
    added_arr = cJSON_AddArrayToObject(root, "added_arr");
    created_raw = cJSON_CreateRaw("<<raw-data>>");
    cJSON_AddItemToObject(root, "created_raw", created_raw);

    // step 3: Operate / Validate
    raw_item = cJSON_GetObjectItem(root, "created_raw");
    is_raw = cJSON_IsRaw(raw_item);
    name_item = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(name_item);
    add_ret = cJSON_AddItemToArray(added_arr, cJSON_CreateString(name_value));
    dup = cJSON_Duplicate(root, 1);
    same = cJSON_Compare(root, dup, 1);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    (void)parse_end;
    (void)arr;
    (void)is_raw;
    (void)add_ret;
    (void)same;
    (void)name_value;
    // API sequence test completed successfully
    return 66;
}