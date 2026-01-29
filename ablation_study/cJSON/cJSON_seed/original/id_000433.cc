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
//<ID> 433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"tester\",\"tags\":[\"alpha\",\"beta\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_invalid = cJSON_IsInvalid(name);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *new_arr = cJSON_CreateArray();
    const char *ref_str = "added";
    cJSON *ref_item = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToArray(new_arr, ref_item);
    cJSON *flags_num = cJSON_CreateNumber((double)(root_is_obj + name_invalid));
    cJSON_AddItemToObject(root, "flags", flags_num);
    cJSON_AddItemToObject(root, "new_tags", new_arr);

    // step 3: Operate
    cJSON *tags = cJSON_GetObjectItemCaseSensitive(root, "tags");
    cJSON *first_tag = cJSON_GetArrayItem(tags, 0);
    const char *first_val = cJSON_GetStringValue(first_tag);
    cJSON *ref_first = cJSON_CreateStringReference(first_val);
    cJSON_AddItemToArray(new_arr, ref_first);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}