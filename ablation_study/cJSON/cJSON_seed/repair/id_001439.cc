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
//<ID> 1439
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
    cJSON_AddStringToObject(root, "note", "api-sequence-test");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *active_item = cJSON_GetObjectItem(meta_item, "active");
    cJSON *items_item = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items_item);
    int count = cJSON_GetArraySize(items_item);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = len > 0 ? unfmt[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)(items_is_array ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}