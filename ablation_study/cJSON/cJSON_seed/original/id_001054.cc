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
//<ID> 1054
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"items\":[10,20,30],\"meta\":{\"ok\":false}}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(second_value + 5.0));

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    const int buf_len = 512;
    char *prealloc_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc_buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, prealloc_buf, buf_len, 1);
    cJSON_bool second_is_invalid = cJSON_IsInvalid(second_item);
    (void)second_is_invalid;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}