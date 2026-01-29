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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *raw_child = (cJSON *)0;
    cJSON *raw_root = (cJSON *)0;
    cJSON *num_flag = (cJSON *)0;
    cJSON_bool child_is_array = (cJSON_bool)0;
    cJSON_bool root_is_array = (cJSON_bool)0;
    int flags_sum = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    raw_child = cJSON_AddRawToObject(child, "payload", "{\"a\":1,\"b\":2}");
    raw_root = cJSON_AddRawToObject(root, "metadata", "{\"ver\":1}");
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    child_is_array = cJSON_IsArray(child);
    root_is_array = cJSON_IsArray(root);

    // step 5: Validate
    flags_sum = (int)child_is_array + (int)root_is_array;
    num_flag = cJSON_CreateNumber((double)flags_sum);
    cJSON_AddItemToObject(root, "flags_sum", num_flag);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}