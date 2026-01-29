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
//<ID> 1019
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"a\":null,\"b\":42,\"c\":\"hello\"}";
    size_t json_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, json_len);

    // step 2: Configure
    cJSON *out = cJSON_CreateObject();
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    cJSON *b_item = cJSON_GetObjectItem(root, "b");
    cJSON *c_item = cJSON_GetObjectItem(root, "c");
    cJSON_bool a_is_null = cJSON_IsNull(a_item);
    cJSON_AddBoolToObject(out, "a_was_null", a_is_null);
    cJSON *injected_str = cJSON_CreateString("injected_value");
    cJSON_AddItemToObject(out, "injected", injected_str);

    // step 3: Operate
    double b_val = cJSON_GetNumberValue(b_item);
    cJSON_AddNumberToObject(out, "b_doubled", b_val * 2.0);
    const char *c_str = cJSON_GetStringValue(c_item);
    cJSON_AddStringToObject(out, "copied_c", c_str);
    char *out_unformatted = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(out, print_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}