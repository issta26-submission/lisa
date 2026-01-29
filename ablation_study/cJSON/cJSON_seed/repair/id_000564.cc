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
//<ID> 564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToArray(items, num_array);
    cJSON *meta = cJSON_CreateString("dataset");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *meta_ref = cJSON_GetObjectItemCaseSensitive(root, "meta");
    char *meta_str = cJSON_GetStringValue(meta_ref);
    cJSON *meta_copy = cJSON_CreateString(meta_str);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(items_ref, 0);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}