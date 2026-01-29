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
//<ID> 1172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"key1\":123.5,\"flag\":false,\"items\":[1,2]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "embedded_raw", raw_item);
    cJSON_AddItemToObjectCS(root, "assert_true", true_item);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_bool = cJSON_IsTrue(parsed_flag);
    cJSON_AddBoolToObject(root, "flag_copy", flag_bool);
    cJSON *num_item = cJSON_GetObjectItem(parsed, "key1");
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "key1_copy", num_val);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool eq = cJSON_Compare(root, dup, 1);
    cJSON *comparison = cJSON_CreateString(eq ? "equal" : "not_equal");
    cJSON_AddItemToObjectCS(root, "comparison", comparison);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}