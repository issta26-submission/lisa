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
//<ID> 1029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"tester\",\"nums\":[10,20,30],\"meta\":{\"id\":7}}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *nums_item = cJSON_GetObjectItem(root, "nums");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *id_item = cJSON_GetObjectItem(meta_item, "id");
    char *name_str = cJSON_GetStringValue(name_item);
    int extra_nums[4] = {4, 5, 6, 7};
    cJSON *new_ints = cJSON_CreateIntArray(extra_nums, 4);
    cJSON *dup_nums = cJSON_Duplicate(nums_item, 1);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObject(out, "owner", cJSON_CreateString(name_str));
    cJSON_AddItemToObject(out, "copied_nums", dup_nums);
    cJSON_AddItemToObject(out, "new_ints", new_ints);
    cJSON_AddItemToObject(out, "meta_id", cJSON_CreateNumber(cJSON_GetNumberValue(id_item)));

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(out, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}