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
//<ID> 1159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"enabled\":false,\"count\":3}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("alpha");
    cJSON *enabled = cJSON_CreateBool(0);
    cJSON *count = cJSON_CreateNumber(3.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(parsed, root, 1);
    cJSON *comparison = cJSON_CreateString(equal ? "equal" : "not_equal");
    cJSON_AddItemToObject(root, "comparison", comparison);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}