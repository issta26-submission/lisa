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
//<ID> 1022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"orig\",\"meta\":99}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    int nums[] = {7, 8, 9};
    cJSON *num_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(dup_root, "name_copy", name_copy);

    // step 3: Operate
    cJSON_AddItemToObject(root, "duplicate", dup_root);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}