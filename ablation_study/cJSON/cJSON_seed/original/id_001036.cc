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
//<ID> 1036
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"initial\",\"value\":42}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON_SetValuestring(name_item, "modified_name");
    cJSON *raw_item = cJSON_CreateRaw("raw_content_here");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddStringToObject(out, "copied_name", cJSON_GetStringValue(name_item));
    cJSON_AddNumberToObject(out, "value_twice", cJSON_GetNumberValue(value_item) * 2.0);
    cJSON_AddBoolToObject(out, "raw_is_raw", raw_flag);
    char *printed_root = cJSON_Print(root);
    char *out_unformatted = cJSON_PrintUnformatted(out);
    int buf_size = 256;
    char *print_buf = (char *)cJSON_malloc((size_t)buf_size);
    memset(print_buf, 0, (size_t)buf_size);

    // step 4: Validate & Cleanup
    cJSON_free(printed_root);
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}