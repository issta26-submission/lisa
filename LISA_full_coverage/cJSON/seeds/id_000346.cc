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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[1,2], \"meta\":{\"tag\":\"ok\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *nums = cJSON_GetObjectItemCaseSensitive(root, "nums");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *tag = cJSON_GetObjectItemCaseSensitive(meta, "tag");
    const char *tag_str = cJSON_GetStringValue(tag);

    // step 2: Configure
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemReferenceToArray(nums, n3);

    // step 3: Operate
    cJSON *stats = cJSON_CreateObject();
    int array_size = cJSON_GetArraySize(nums);
    cJSON *size_node = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(stats, "size", size_node);
    cJSON_AddStringToObject(stats, "tag_copy", tag_str);
    cJSON_AddItemToObject(root, "stats", stats);
    char *printed = cJSON_Print(root);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}