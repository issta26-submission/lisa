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
//<ID> 1159
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
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(child, "inner", str_item);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child, "value", num_item);

    // step 2: Configure
    char *setres = cJSON_SetValuestring(str_item, "updated");
    cJSON_AddStringToObject(root, "from_set", setres);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateFalse());
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());

    // step 3: Operate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
    cJSON_bool num_is_invalid = cJSON_IsInvalid(num_item);
    cJSON_AddBoolToObject(root, "num_is_invalid", num_is_invalid);
    cJSON *detached = cJSON_DetachItemFromObject(child, "inner");
    cJSON_AddItemToObject(root, "inner_detached", detached);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
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