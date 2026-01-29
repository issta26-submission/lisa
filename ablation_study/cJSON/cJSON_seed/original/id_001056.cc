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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"meta\":{\"count\":2}}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *second_name = cJSON_GetObjectItem(second_item, "name");
    const char *second_name_str = cJSON_GetStringValue(second_name);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddStringToObject(out, "selected_name", second_name_str);
    cJSON_AddBoolToObject(out, "second_is_invalid", cJSON_IsInvalid(second_item));

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    int buf_len = 512;
    char *prealloc = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(out, prealloc, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(prealloc);
    cJSON_free(pretty);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}