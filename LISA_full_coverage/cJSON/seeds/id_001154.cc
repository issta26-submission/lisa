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
//<ID> 1154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "key", item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child, "pi", num);

    // step 2: Configure
    char *newstr = cJSON_SetValuestring(item, "changed");
    cJSON_bool child_is_obj = cJSON_IsObject(child);
    cJSON_bool root_is_invalid = cJSON_IsInvalid(root);
    cJSON *detached = cJSON_DetachItemFromObject(root, "key");
    cJSON_AddBoolToObject(root, "child_is_object", child_is_obj);
    cJSON_AddBoolToObject(root, "root_is_invalid", root_is_invalid);

    // step 3: Operate
    cJSON_AddItemToObject(child, "reinserted", detached);
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}