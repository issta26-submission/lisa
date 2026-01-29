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
//<ID> 142
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
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(meta, "name", cJSON_CreateString("synth"));
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));

    // step 2: Configure
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "maybe_null", maybe_null);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(extra, "tag", cJSON_CreateString("x"));
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *retrieved_null = cJSON_GetObjectItemCaseSensitive(root, "maybe_null");
    cJSON_bool is_null = cJSON_IsNull(retrieved_null);
    cJSON *retrieved_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *retrieved_name = cJSON_GetObjectItemCaseSensitive(retrieved_meta, "name");
    int items_count = cJSON_GetArraySize(arr);
    (void)is_null;
    (void)retrieved_name;
    (void)items_count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}