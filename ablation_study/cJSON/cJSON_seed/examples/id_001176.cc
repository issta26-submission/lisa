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
//<ID> 1176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":3.14,\"flag\":true,\"meta\":\"rawtext\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    cJSON_AddNumberToObject(root, "alpha_copy", alpha_val);
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_bool = cJSON_IsTrue(flag_item);
    cJSON_AddBoolToObject(root, "flag_copy", flag_bool);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":42}");
    cJSON_AddItemToObjectCS(root, "raw_cs", raw);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "always_true", t);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(parsed, root, 1);
    cJSON_AddBoolToObject(root, "equal", equal);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}