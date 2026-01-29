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
//<ID> 688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alice\",\"age\":30,\"nested\":{\"greet\":\"hello\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_before = cJSON_GetStringValue(name_item);
    char *set_ret_name = cJSON_SetValuestring(name_item, "bob");
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *greet_item = cJSON_GetObjectItem(nested, "greet");
    char *greet_before = cJSON_GetStringValue(greet_item);

    // step 3: Operate
    char *set_ret_greet = cJSON_SetValuestring(greet_item, "hi");
    char *name_after = cJSON_GetStringValue(name_item);
    char *greet_after = cJSON_GetStringValue(greet_item);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}