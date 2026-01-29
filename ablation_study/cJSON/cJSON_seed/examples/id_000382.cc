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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"opt\": null, \"value\": 10}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("test_root");
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(5.0);
    double newnum = cJSON_SetNumberHelper(num, 123.456);
    cJSON_AddItemToObjectCS(root, "num", num);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    const char *ver = cJSON_Version();
    sprintf(status_buf, "%s|%g", ver ? ver : "unknown", newnum);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status);

    // step 3: Operate & Validate
    cJSON *opt_item = cJSON_GetObjectItem(parsed, "opt");
    cJSON *opt_was_null = cJSON_CreateBool(cJSON_IsNull(opt_item));
    cJSON_AddItemToObjectCS(root, "opt_was_null", opt_was_null);
    cJSON *parsed_value = cJSON_GetObjectItem(parsed, "value");
    double parsed_num = cJSON_GetNumberValue(parsed_value);
    cJSON *sum = cJSON_CreateNumber(parsed_num + newnum);
    cJSON_AddItemToObjectCS(root, "sum", sum);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}