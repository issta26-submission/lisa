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
//<ID> 1435
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
    cJSON_AddFalseToObject(meta, "active");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    cJSON *found_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool found_is_array = cJSON_IsArray(found_items);

    // step 3: Operate and Validate
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len_unfmt = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len_unfmt + 16);
    memset(scratch, 0, len_unfmt + 16);
    scratch[0] = len_unfmt > 0 ? unfmt[0] : ' ';
    scratch[1] = (char)('0' + (cJSON_GetArraySize(items) % 10));
    cJSON *active_item = cJSON_GetObjectItem(meta, "active");
    cJSON_bool active_is_false = cJSON_IsFalse(active_item);
    char *fmt = cJSON_Print(root);
    size_t len_fmt = fmt ? (size_t)strlen(fmt) : (size_t)0;
    char *scratch2 = (char *)cJSON_malloc(len_fmt + 8);
    memset(scratch2, 0, len_fmt + 8);
    scratch2[0] = len_fmt > 0 ? fmt[0] : ' ';
    scratch2[1] = active_is_false ? '0' : '1';
    scratch2[2] = found_is_array ? 'A' : 'N';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(scratch2);
    cJSON_free(unfmt);
    cJSON_free(fmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}