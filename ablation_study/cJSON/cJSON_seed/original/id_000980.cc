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
//<ID> 980
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\": {\"flag\": false}, \"data\": {\"name\": \"original\"}}";
    size_t initial_len = sizeof(initial_json) - 1;
    char *mutable_json = (char *)cJSON_malloc(initial_len + 1);
    memcpy(mutable_json, initial_json, initial_len + 1);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *flag = cJSON_GetObjectItem(meta, "flag");
    cJSON_bool flag_was_false = cJSON_IsFalse(flag);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *name = cJSON_GetObjectItem(data, "name");
    char *previous_name = cJSON_GetStringValue(name);
    char *set_result = cJSON_SetValuestring(name, "updated");
    cJSON *inner = cJSON_AddObjectToObject(data, "inner");
    cJSON_bool inner_is_obj = cJSON_IsObject(inner);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *print_buf = (char *)cJSON_malloc(buf_len);
    memset(print_buf, 0, buf_len);
    cJSON_PrintPreallocated(root, print_buf, buf_len, 1);

    // step 4: Validate & Cleanup
    (void)previous_name;
    (void)flag_was_false;
    (void)set_result;
    (void)inner_is_obj;
    cJSON_DeleteItemFromObjectCaseSensitive(data, "name");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_free(mutable_json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}