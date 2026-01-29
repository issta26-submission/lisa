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
//<ID> 144
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
    cJSON *name = cJSON_CreateString("synth");
    cJSON_AddItemToObjectCS(meta, "name", name);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 2: Configure
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObjectCS(root, "meta_copy", dup_meta);
    cJSON_AddItemToObjectCS(root, "maybe_null", cJSON_CreateNull());

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *got_null = cJSON_GetObjectItemCaseSensitive(root, "maybe_null");
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_bool is_null = cJSON_IsNull(got_null);
    cJSON_bool meta_is_null = cJSON_IsNull(got_meta);
    int size_items = cJSON_GetArraySize(arr);
    int summary = (int)is_null + (int)meta_is_null + size_items + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}