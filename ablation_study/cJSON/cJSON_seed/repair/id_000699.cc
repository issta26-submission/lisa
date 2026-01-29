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
//<ID> 699
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
    cJSON *bools = cJSON_AddArrayToObject(config, "bools");
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(bools, f1);
    cJSON *f2 = cJSON_CreateFalse();
    cJSON_AddItemToArray(bools, f2);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(bools);
    cJSON_AddNumberToObject(root, "bool_count", count);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}