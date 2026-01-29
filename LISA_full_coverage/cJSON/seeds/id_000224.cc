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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *ref_item = cJSON_CreateString("reference");
    cJSON_AddItemReferenceToArray(arr, ref_item);
    cJSON *bool_item = cJSON_AddBoolToObject(root, "enabled", 0);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved", detached);

    // step 3: Operate and Validate
    cJSON_bool is_false = cJSON_IsFalse(bool_item);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    const char *det_str = cJSON_GetStringValue(detached);
    int array_size = cJSON_GetArraySize(arr);
    int summary = array_size + (int)is_false + (det_str ? (int)det_str[0] : 0) + root->type;
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}