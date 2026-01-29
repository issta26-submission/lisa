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
//<ID> 1297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"count\":5,\"threshold\":10}}";
    cJSON *parsed = cJSON_Parse(json);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "parsed", parsed);
    cJSON *new_threshold = cJSON_CreateNumber(42.0);

    // step 3: Operate
    cJSON *config = cJSON_GetObjectItem(parsed, "config");
    cJSON *threshold = cJSON_GetObjectItem(config, "threshold");
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(config, threshold, new_threshold);
    (void)replace_result;

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}