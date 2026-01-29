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
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"init\": true}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *originalItem = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "orig", originalItem);
    cJSON *refTarget = cJSON_CreateNumber(99);
    cJSON_AddItemToObject(root, "ref_target", refTarget);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(array, refTarget);
    cJSON_AddBoolToObject(root, "ref_added", ref_added);

    // step 3: Operate
    cJSON_bool is_arr = cJSON_IsArray(array);
    cJSON_AddBoolToObject(root, "arr_is_array", is_arr);
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemViaPointer(root, originalItem, replacement);
    cJSON *null_item = cJSON_AddNullToObject(root, "null_entry");
    (void)null_item;

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}