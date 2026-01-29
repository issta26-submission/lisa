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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"flag\":null}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *name_item = cJSON_GetObjectItem(second_item, "name");
    cJSON_bool name_is_invalid = cJSON_IsInvalid(name_item);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddBoolToObject(out, "name_invalid", name_is_invalid);
    cJSON_AddStringToObject(out, "copied_name", cJSON_GetStringValue(name_item));

    // step 3: Operate
    char *pretty_out = cJSON_Print(out);
    int buf_len = 512;
    char *prebuf = (char *)cJSON_malloc((size_t)buf_len);
    memset(prebuf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, prebuf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty_out);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}