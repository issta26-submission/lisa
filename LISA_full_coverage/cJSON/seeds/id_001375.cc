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
//<ID> 1375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[3] = {1, 2, 3};
    cJSON *ints = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *dup_ints = cJSON_Duplicate(ints, 1);
    cJSON_AddItemToObject(root, "ints_copy", dup_ints);

    // step 3: Operate
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *hasnum = cJSON_CreateNumber((double)has_name);
    cJSON_AddItemToObject(root, "has_name", hasnum);
    char *printed1 = cJSON_PrintBuffered(root, 128, 1);
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "name", new_name);
    char *printed2 = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}