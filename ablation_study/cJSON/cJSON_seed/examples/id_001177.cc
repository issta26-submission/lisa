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
//<ID> 1177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"beta\",\"active\":true,\"nums\":[1,2]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("\"inline_raw\"");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "raw_key", raw);
    cJSON_AddItemToObjectCS(root, "true_key", true_item);
    cJSON_AddBoolToObject(root, "flag", 0);

    // step 3: Operate & Validate
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObjectCS(root, "name_copy", name_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}