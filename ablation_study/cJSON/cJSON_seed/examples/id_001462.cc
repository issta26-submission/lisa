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
//<ID> 1462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_text = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_text);
    const char *json_input = "{\"num\":42,\"str\":\"hello\"}";
    cJSON *root = cJSON_Parse(json_input);

    // step 2: Configure
    cJSON_AddItemToObject(root, "version", version_item);
    cJSON *added_string = cJSON_CreateString("added_value");
    cJSON_AddItemToObject(root, "added", added_string);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool num_is_number = cJSON_IsNumber(num_item);
    (void)num_is_number;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}