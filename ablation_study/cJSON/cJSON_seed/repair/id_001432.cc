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
//<ID> 1432
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items_ref);
    cJSON_AddNumberToObject(root, "item_count", (double)cJSON_GetArraySize(items_ref));
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *active_node = cJSON_GetObjectItem(meta_ref, "active");

    // step 3: Operate and Validate
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *buf = (char *)cJSON_malloc(len + 16);
    memset(buf, 0, len + 16);
    buf[0] = len > 0 ? unfmt[0] : ' ';
    buf[1] = (char)('0' + (cJSON_GetArraySize(items_ref) % 10));
    buf[2] = items_is_array ? '1' : '0';
    buf[3] = (active_node && cJSON_IsFalse(active_node)) ? 'f' : 'F';

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}