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
//<ID> 1369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *raw_item = NULL;
    cJSON *new_item = NULL;
    char *printed = NULL;
    int *nums = NULL;
    cJSON_bool was_raw = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 3);
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);
    new_item = cJSON_CreateString("replaced_value");

    // step 3: Operate / Validate
    was_raw = cJSON_IsRaw(raw_item);
    replaced = cJSON_ReplaceItemInObject(root, "rawdata", new_item);
    printed = cJSON_PrintUnformatted(root);
    (void)was_raw;
    (void)replaced;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(nums);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}