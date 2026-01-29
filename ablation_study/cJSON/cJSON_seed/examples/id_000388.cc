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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"test\",\"value\":10,\"optional\":null}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *ver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(root, "version", ver);

    // step 2: Configure
    cJSON *val_item = cJSON_GetObjectItem(parsed, "value");
    double newnum = cJSON_SetNumberHelper(val_item, 42.5);
    cJSON *adjusted = cJSON_CreateNumber(newnum);
    cJSON_AddItemToObjectCS(root, "adjusted_value", adjusted);
    cJSON *opt_item = cJSON_GetObjectItem(parsed, "optional");
    cJSON_bool was_null = cJSON_IsNull(opt_item);
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%f", (int)was_null, newnum);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status);

    // step 3: Operate & Validate
    cJSON *orig_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObjectCS(root, "original", orig_ref);
    cJSON *snapshot = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObjectCS(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}