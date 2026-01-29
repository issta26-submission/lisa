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
//<ID> 2348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *msg = cJSON_CreateString("Hello, world!");
    cJSON_AddItemToObjectCS(root, "message", msg);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *lang = cJSON_CreateString("en-US");
    cJSON_AddItemToObject(meta, "lang", lang);

    // step 3: Operate
    char *json = cJSON_PrintBuffered(root, 128, 1);
    cJSON *parsed = cJSON_Parse(json);
    cJSON *parsed_msg = cJSON_GetObjectItemCaseSensitive(parsed, "message");
    char *msg_str = cJSON_GetStringValue(parsed_msg);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (msg_str && msg_str[0]) ? msg_str[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}