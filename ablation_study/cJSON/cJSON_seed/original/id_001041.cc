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
//<ID> 1041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int base_nums[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("tester");
    cJSON *nums = cJSON_CreateIntArray(base_nums, 3);
    cJSON_AddItemToObjectCS(root, "name", name);
    cJSON_AddItemToObjectCS(root, "nums", nums);

    // step 2: Configure
    cJSON_bool nums_is_array = cJSON_IsArray(nums);
    cJSON *flag = cJSON_CreateBool(nums_is_array);
    cJSON_AddItemToObjectCS(root, "nums_is_array", flag);
    cJSON_SetValuestring(name, "tester_modified");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "id", 42);

    // step 3: Operate
    char *compact = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(512);
    memset(buf, 0, 512);
    cJSON_PrintPreallocated(root, buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}