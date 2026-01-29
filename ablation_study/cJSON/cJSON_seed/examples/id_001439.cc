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
//<ID> 1439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(child, "count", 10.0);

    // step 2: Configure & Operate
    cJSON_AddNumberToObject(root, "rootval", 1.5);
    cJSON *num_item = cJSON_GetObjectItem(root, "rootval");
    cJSON *dup = cJSON_Duplicate(num_item, 1);
    cJSON_AddItemToObject(child, "dup", dup);
    char buffer[256];
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 3: Validate
    cJSON_bool is_num_original = cJSON_IsNumber(num_item);
    cJSON_bool is_num_dup = cJSON_IsNumber(dup);
    (void)printed;
    (void)is_num_original;
    (void)is_num_dup;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}