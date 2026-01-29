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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"parsed_null\": null, \"value\": 7}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *initial_num = cJSON_CreateNumber(3.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "initial_value", initial_num);
    double newnum = cJSON_SetNumberHelper(initial_num, 4.5);
    cJSON *version_str = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(root, "cjson_version", version_str);

    // step 3: Operate & Validate
    cJSON *parsed_val_item = cJSON_GetObjectItem(parsed, "value");
    double parsed_val = cJSON_GetNumberValue(parsed_val_item);
    cJSON *sum_item = cJSON_CreateNumber(newnum + parsed_val);
    cJSON_AddItemToObjectCS(root, "sum", sum_item);
    cJSON *parsed_null_item = cJSON_GetObjectItem(parsed, "parsed_null");
    cJSON_bool was_null = cJSON_IsNull(parsed_null_item);
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%s|%d", cJSON_Version(), (int)was_null);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapstr);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}