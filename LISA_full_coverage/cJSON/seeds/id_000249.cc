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
//<ID> 249
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
    cJSON_AddNumberToObject(child, "value", 3.14159);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_AddBoolToObject(root, "has_child", has_child);
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON *dup = cJSON_Duplicate(detached, 1);

    // step 3: Operate and Validate
    cJSON_bool equal = cJSON_Compare(detached, dup, 1);
    cJSON_AddBoolToObject(root, "detached_equals_dup", equal);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)has_child + (int)equal + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}