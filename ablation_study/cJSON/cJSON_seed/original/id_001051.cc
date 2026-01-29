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
//<ID> 1051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"flag\":false}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *out = cJSON_CreateObject();
    cJSON *invalid_flag = cJSON_CreateBool(cJSON_IsInvalid(second_item));
    cJSON_AddItemToObject(out, "second_item_invalid", invalid_flag);
    cJSON_AddItemReferenceToObject(out, "picked", second_item);

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    char *prealloc_buf = (char *)cJSON_malloc(512);
    memset(prealloc_buf, 0, 512);
    cJSON_PrintPreallocated(out, prealloc_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}