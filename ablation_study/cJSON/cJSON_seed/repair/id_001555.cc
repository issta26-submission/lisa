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
//<ID> 1555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"greeting\":\"hello\",\"count\":5}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *old_greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON *mode_item = cJSON_CreateString("active");
    cJSON_AddItemToObjectCS(root, "mode", mode_item);

    // step 2: Configure
    cJSON *new_greeting = cJSON_CreateString("hi");
    cJSON_ReplaceItemViaPointer(root, old_greeting, new_greeting);

    // step 3: Operate and Validate
    cJSON *g = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool is_str = cJSON_IsString(g);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)is_str);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = mode_item->valuestring ? mode_item->valuestring[0] : 'a';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}