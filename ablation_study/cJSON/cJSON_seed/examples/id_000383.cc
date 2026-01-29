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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[1,2], \"name\":\"alpha\"}";
    const char *parse_end = 0;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    const char *ver = cJSON_Version();
    cJSON *ver_str = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(root, "version", ver_str);
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 123.456);
    cJSON_AddItemToObjectCS(root, "count", num);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "maybe", maybe_null);
    cJSON *parsed_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObjectCS(root, "parsed_ref", parsed_ref);

    // step 2: Configure
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    double num_val = cJSON_GetNumberValue(num);
    int ended = (parse_end != 0);
    sprintf(status_buf, "%s|%d|%d|%.3f", ver, (int)was_null, ended, num_val);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}