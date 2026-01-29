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
//<ID> 985
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

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("old_name");
    cJSON_AddItemToObject(meta, "name", name_item);
    char *new_name_ptr = cJSON_SetValuestring(name_item, "updated_name");
    cJSON *flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_item);
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_item);
    cJSON_AddNumberToObject(root, "flag_was_false", (double)flag_was_false);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_obj);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}