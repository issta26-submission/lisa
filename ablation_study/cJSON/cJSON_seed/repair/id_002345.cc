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
//<ID> 2345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_input = "{\"name\":\"alpha\",\"value\":123}";
    cJSON *parsed = cJSON_Parse(json_input);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *name_field = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(name_field);
    cJSON *name_item = cJSON_CreateString(name_str ? name_str : "");
    cJSON_AddItemToObjectCS(root, "copied_name", name_item);
    cJSON *static_item = cJSON_CreateString("static-label");
    cJSON_AddItemToObjectCS(root, "label", static_item);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(root->type & 0xFF);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}