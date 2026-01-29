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
//<ID> 1554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"name\":\"alice\",\"age\":30}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObjectCS(root, "imported", parsed);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("override");
    cJSON *target = cJSON_GetObjectItem(root, "imported");
    cJSON_ReplaceItemViaPointer(root, target, replacement);

    // step 3: Operate and Validate
    cJSON *imported_item = cJSON_GetObjectItem(root, "imported");
    cJSON *flag_num = cJSON_CreateNumber((double)cJSON_IsString(imported_item));
    cJSON_AddItemToObjectCS(root, "is_string", flag_num);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = strlen(out);
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out[0];
    scratch[1] = (char)('0' + (int)cJSON_IsString(imported_item));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}