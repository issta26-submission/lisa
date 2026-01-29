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
//<ID> 1434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));
    cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(root, "name", "example");
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(items_ref);
    int count = cJSON_GetArraySize(items_ref);

    // step 3: Operate and Validate
    cJSON_AddNumberToObject(meta, "count", (double)count);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *buf = (char *)cJSON_malloc(len + 32);
    memset(buf, 0, len + 32);
    buf[0] = len > 0 ? unfmt[0] : ' ';
    buf[1] = (char)('0' + (count % 10));
    buf[2] = (char)(is_array ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}