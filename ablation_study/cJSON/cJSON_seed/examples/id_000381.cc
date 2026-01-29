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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"parsed\": 23.5}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(root, "initial", num);
    double newnum = cJSON_SetNumberHelper(num, 42.0);
    const char *ver = cJSON_Version();
    cJSON *ver_str = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_str);

    // step 3: Operate & Validate
    cJSON *parsed_item = cJSON_GetObjectItem(parsed, "parsed");
    double parsed_val = cJSON_GetNumberValue(parsed_item);
    double total = newnum + parsed_val;
    cJSON *sum = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", maybe_null);
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%g", (int)was_null, total);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}