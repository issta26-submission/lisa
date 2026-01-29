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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "empty", null_item);

    // step 2: Configure
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObjectCS(root, "meta_copy", dup_meta);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddStringToObject(extra, "tag", "x");
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON *empty_item = cJSON_GetObjectItemCaseSensitive(root, "empty");
    cJSON_bool empty_is_null = cJSON_IsNull(empty_item);
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);
    int arr_size = cJSON_GetArraySize(arr);
    int summary = (int)empty_is_null + (int)meta_is_obj + arr_size + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}