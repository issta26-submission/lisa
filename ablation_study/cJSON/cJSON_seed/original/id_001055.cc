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
//<ID> 1055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[10,20,30],\"meta\":{\"active\":true}}";
    size_t json_len = sizeof(json_text) - 1;
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool invalid_flag = cJSON_IsInvalid(second_item);
    cJSON_bool format_flag = (cJSON_bool)(1 - invalid_flag);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *prealloc_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc_buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, prealloc_buf, buf_len, format_flag);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}