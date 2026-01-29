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
//<ID> 1430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"num\":42,\"obj\":{\"key\":\"value\"},\"arr\":[1,2]}";
    size_t json_len = strlen(json_text);
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    const char *raw_fragment = "{ \"rawkey\": 123 }";

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddRawToObject(dup, "raw_added", raw_fragment);
    cJSON *child_obj = cJSON_GetObjectItem(dup, "obj");
    cJSON *detached = cJSON_DetachItemViaPointer(dup, child_obj);

    // step 3: Operate & Validate
    char *temp_print = cJSON_PrintUnformatted(dup);
    size_t temp_len = strlen(temp_print);
    int buf_len = (int)(temp_len + 32);
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool ok = cJSON_PrintPreallocated(dup, buffer, buf_len, 0);
    (void)ok;
    (void)buffer;
    cJSON_free(temp_print);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}