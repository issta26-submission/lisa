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
//<ID> 140
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
    cJSON_AddItemToObjectCS(meta, "name", cJSON_CreateString("synth"));
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToObjectCS(root, "optional", cJSON_CreateNull());

    // step 2: Configure
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToArray(arr, dup_meta);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddStringToObject(extra, "tag", "x");
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *opt = cJSON_GetObjectItemCaseSensitive(root, "optional");
    cJSON_bool opt_is_null = cJSON_IsNull(opt);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int first_char = printed ? (int)printed[0] : 0;
    int arr_size = cJSON_GetArraySize(arr);
    int summary = first_char + (int)opt_is_null + arr_size + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}