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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");
    cJSON *manual_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, manual_false);

    // step 2: Configure
    cJSON *title = cJSON_CreateString("demo_list");
    cJSON_AddItemToObject(root, "title", title);
    int sample_nums[3] = {1, 2, 3};
    cJSON *numbers = cJSON_CreateIntArray(sample_nums, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_obj = cJSON_IsObject(got_items);
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(got_enabled);
    int item_count = cJSON_GetArraySize(got_items);
    char *flat = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (item_count & 0xF));
    ((char *)scratch)[1] = (char)('0' + (enabled_is_true ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (items_is_obj & 1));
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}