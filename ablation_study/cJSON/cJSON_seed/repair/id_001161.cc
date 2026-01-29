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
//<ID> 1161
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
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(meta, "count", count);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "initial");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_ref);
    cJSON *count_ref = cJSON_GetObjectItem(meta_ref, "count");
    double set_ret = cJSON_SetNumberHelper(count_ref, 5.0);
    double computed_val = cJSON_GetNumberValue(count_ref) + 1.0;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    cJSON_bool optional_is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    cJSON *detached_note = cJSON_DetachItemFromObject(root, "note");
    cJSON_AddItemToObject(root, "note", detached_note);
    (void)meta_is_obj;
    (void)set_ret;
    (void)optional_is_null;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}