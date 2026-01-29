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
//<ID> 1015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char input_json[] = "{\"key\":null,\"name\":\"example\",\"count\":3}";
    size_t input_len = sizeof(input_json) - 1;
    cJSON *root = cJSON_ParseWithLength(input_json, input_len);
    cJSON *key_item = cJSON_GetObjectItem(root, "key");

    // step 2: Configure
    cJSON_bool key_was_null = cJSON_IsNull(key_item);
    cJSON_AddBoolToObject(root, "key_was_null", key_was_null);
    cJSON *note = cJSON_CreateString("generated");
    cJSON_AddItemToObject(root, "note", note);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddBoolToObject(extra, "active", 1);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 512;
    char *print_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(print_buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, print_buf, buf_len, 0);

    // step 4: Validate & Cleanup
    cJSON *note_item = cJSON_GetObjectItem(root, "note");
    cJSON_bool note_is_null = cJSON_IsNull(note_item);
    cJSON_AddBoolToObject(root, "note_is_null", note_is_null);
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}