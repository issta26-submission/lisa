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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char greeting_text[] = "Hello, cJSON!";
    const char raw_text[] = "RAW:{\"x\":1}";
    cJSON *root = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString(greeting_text);
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON *raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "rawdata", raw);
    cJSON *status_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON_bool active_true = cJSON_IsTrue(status_item);
    cJSON_bool raw_is = cJSON_IsRaw(raw);
    char *greeting_val = cJSON_GetStringValue(greet);
    size_t buf_len = 64 + (size_t)active_true * 16 + (size_t)raw_is * 8;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *char_buf = (char *)buffer;
    char_buf[0] = greeting_val ? greeting_val[0] : '\0';
    char_buf[1] = (char)('0' + (active_true ? 1 : 0));
    char_buf[2] = (char)('0' + (raw_is ? 1 : 0));

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)snapshot;
    (void)char_buf;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}