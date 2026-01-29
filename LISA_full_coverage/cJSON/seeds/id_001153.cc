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
//<ID> 1153
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
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child, "value", num_item);

    // step 2: Configure
    char *set_result = cJSON_SetValuestring(name_item, "updated");
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool name_is_invalid = cJSON_IsInvalid(name_item);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
    cJSON_AddBoolToObject(root, "name_is_invalid", name_is_invalid);
    (void)set_result;

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(child, "value");
    const char *updated_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "child_name", updated_name);
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}