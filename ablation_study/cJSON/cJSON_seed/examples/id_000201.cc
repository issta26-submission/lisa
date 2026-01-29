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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "item", orig);

    // step 2: Configure
    cJSON *ref = cJSON_CreateStringReference("referenced_value");
    cJSON_bool replaced1 = cJSON_ReplaceItemViaPointer(root, orig, ref);
    cJSON *replaced1_num = cJSON_CreateNumber((double)replaced1);
    cJSON_AddItemToObject(root, "replaced1", replaced1_num);

    // step 3: Operate & Validate
    cJSON *cur = cJSON_GetObjectItem(root, "item");
    cJSON *nullobj = cJSON_CreateNull();
    cJSON_bool replaced2 = cJSON_ReplaceItemViaPointer(root, cur, nullobj);
    cJSON *replaced2_num = cJSON_CreateNumber((double)replaced2);
    cJSON_AddItemToObject(root, "replaced2", replaced2_num);
    cJSON *after = cJSON_GetObjectItem(root, "item");
    cJSON_bool is_null_flag = cJSON_IsNull(after);
    cJSON *is_null_num = cJSON_CreateNumber((double)is_null_flag);
    cJSON_AddItemToObject(root, "is_null", is_null_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}