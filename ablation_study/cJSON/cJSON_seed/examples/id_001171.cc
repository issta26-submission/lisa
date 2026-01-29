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
//<ID> 1171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":42,\"name\":\"example\",\"flag\":false}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "raw_snippet", raw);
    cJSON_AddItemToObjectCS(root, "always_true", t);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *id_item = cJSON_GetObjectItem(parsed, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    cJSON_AddNumberToObject(root, "id_copy", id_val);
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}