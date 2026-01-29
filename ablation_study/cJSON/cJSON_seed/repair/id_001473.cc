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
//<ID> 1473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString("fuzz-test");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 123.456);
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "ok", true_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(list, cJSON_CreateTrue());

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    int orig_count = cJSON_GetArraySize(list);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "original_count", (double)orig_count);
    cJSON_AddItemToObject(dup, "summary", summary);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    const char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_list = cJSON_GetObjectItem(dup, "list");
    cJSON_bool list_is_array = cJSON_IsArray(got_list);
    char *out = cJSON_PrintUnformatted(dup);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_val ? name_val[0] : ' ';
    scratch[1] = (char)('0' + (orig_count % 10));
    scratch[2] = list_is_array ? 'A' : 'N';
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}