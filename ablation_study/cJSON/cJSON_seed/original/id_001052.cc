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
//<ID> 1052
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"name\":\"test\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first);
    cJSON_AddItemToObject(root, "first_is_invalid", cJSON_CreateBool(first_is_invalid));

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    const int buf_len = 256;
    char *prealloc_buf = (char *)cJSON_malloc(buf_len);
    memset(prealloc_buf, 0, buf_len);
    cJSON_PrintPreallocated(root, prealloc_buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(prealloc_buf);
    cJSON_free(pretty);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}