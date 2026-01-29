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
//<ID> 640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_ok = cJSON_CreateBool(1);
    cJSON_AddItemToObject(meta, "ok", meta_ok);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON *b_extra = cJSON_CreateBool(1);
    cJSON_AddItemToArray(flags, b_true);
    cJSON_AddItemToArray(flags, b_false);
    cJSON_AddItemToArray(flags, b_extra);

    // step 3: Operate
    int size_before = cJSON_GetArraySize(flags);
    cJSON *first_item = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_item);
    cJSON_DeleteItemFromArray(flags, 0);
    int size_after = cJSON_GetArraySize(flags);

    // step 4: Validate & Cleanup
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    cJSON_AddBoolToObject(root, "first_was_bool", first_is_bool);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}