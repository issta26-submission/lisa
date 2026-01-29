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
//<ID> 550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *pretty_json = "{\n  \"name\": \"example\",\n  \"value\": 42.0,\n  \"flag\": false\n}\n";
    size_t buf_len = strlen(pretty_json);
    char *buffer = (char *)cJSON_malloc(buf_len + 1);
    memcpy(buffer, pretty_json, buf_len + 1);
    cJSON_Minify(buffer);
    cJSON *root = cJSON_Parse(buffer);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double current_value = cJSON_GetNumberValue(value_item);
    double updated_value = cJSON_SetNumberHelper(value_item, current_value + 8.0);

    // step 3: Operate
    char *printed = cJSON_Print(root);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    const char *extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON_bool extracted_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    cJSON_bool extracted_enabled = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    (void)updated_value;
    (void)extracted;
    (void)extracted_name;
    (void)extracted_flag;
    (void)extracted_enabled;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}