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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON *base_array = cJSON_CreateArray();
    cJSON_AddItemToArray(base_array, bool_item);
    cJSON_AddItemToObject(root, "base", base_array);
    cJSON *array_ref = cJSON_CreateArrayReference(base_array);
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "alias", array_ref);

    // step 2: Configure
    const int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 3: Operate and Validate
    cJSON *alias = cJSON_GetObjectItemCaseSensitive(root, "alias");
    int alias_size = cJSON_GetArraySize(alias);
    cJSON_bool alias_is_array = cJSON_IsArray(alias);
    int summary = (int)alias_is_array + alias_size + (int)ref_added + (int)printed_ok + root->type + (int)buf[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}