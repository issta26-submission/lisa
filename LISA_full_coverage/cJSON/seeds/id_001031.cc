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
//<ID> 1031
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"alpha\",\"value\":3.14,\"meta\":{\"id\":7}}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *set_ret = cJSON_SetValuestring(name_item, "beta");
    char *name_after = cJSON_GetStringValue(name_item);
    cJSON *raw_item = cJSON_CreateRaw("raw_payload: {\"x\":1}");
    cJSON_AddItemToObject(root, "raw_blob", raw_item);
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObject(out, "name_modified", cJSON_CreateString(name_after));
    cJSON_AddItemToObject(out, "raw_was_raw", cJSON_CreateBool(raw_flag));

    // step 3: Operate
    char *root_pretty = cJSON_Print(root);
    char *out_unformatted = cJSON_PrintUnformatted(out);
    int buf_len = 256;
    char *print_buf = (char *)cJSON_malloc(buf_len);
    memset(print_buf, 0, buf_len);
    cJSON_PrintPreallocated(out, print_buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(root_pretty);
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}