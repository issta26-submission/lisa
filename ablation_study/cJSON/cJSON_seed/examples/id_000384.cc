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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"count\":3,\"optional\":null}";
    const char *parse_end = NULL;
    cJSON *doc = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "doc", doc);
    cJSON *num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObjectCS(root, "value", num);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "maybe", maybe_null);

    // step 2: Configure
    double newnum = cJSON_SetNumberHelper(num, 42.0);
    const char *ver = cJSON_Version();
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(root, "version", verstr);
    char meta_buf[48];
    memset(meta_buf, 0, sizeof(meta_buf));
    sprintf(meta_buf, "parsed_at=%p", (void *)parse_end);
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    double current_val = cJSON_GetNumberValue(num);
    char status_buf[80];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%.0f|%.2f", (int)was_null, (int)newnum, current_val);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapstr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}