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
//<ID> 1035
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"raw\":\"<raw content>\",\"nested\":{\"val\":123},\"s\":\"hello\"}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *s_item = cJSON_GetObjectItem(root, "s");
    char *s_val = cJSON_GetStringValue(s_item);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObject(out, "source", cJSON_CreateString(s_val));
    cJSON *modifiable = cJSON_CreateString("temporary");
    cJSON_SetValuestring(modifiable, "modified");
    cJSON_AddItemToObject(out, "modified", modifiable);
    cJSON_AddNumberToObject(out, "raw_flag", (double)raw_flag);

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}