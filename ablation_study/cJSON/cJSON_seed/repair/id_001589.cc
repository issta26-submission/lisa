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
//<ID> 1589
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", num);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "nothing", null_item);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *got_null = cJSON_GetObjectItem(child, "nothing");
    cJSON_bool is_null = cJSON_IsNull(got_null);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)1);
    buffer[0] = (char)('0' + (int)has_child);
    buffer[1] = (char)('0' + (int)is_null);
    buffer[2] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}