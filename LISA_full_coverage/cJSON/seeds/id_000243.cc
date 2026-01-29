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
//<ID> 243
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
    cJSON *leaf = cJSON_CreateString("leaf_value");
    cJSON_AddItemToObject(child, "leaf", leaf);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(child, leaf);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 3: Operate and Validate
    cJSON_bool has_leaf = cJSON_HasObjectItem(child, "leaf");
    cJSON *mirror_str = cJSON_CreateString("leaf_value");
    cJSON_bool equal = cJSON_Compare(detached, mirror_str, 1);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    const char *det_val = cJSON_GetStringValue(detached);
    int summary = (int)has_leaf + (int)equal + (detached ? 1 : 0) + (det_val ? (int)det_val[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(mirror_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}