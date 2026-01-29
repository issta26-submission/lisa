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
//<ID> 998
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"example\",\"info\":{\"type\":\"demo\"},\"values\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    const char *const colors[] = { "red", "green", "blue" };
    cJSON *colors_array = cJSON_CreateStringArray(colors, 3);
    cJSON_AddItemToObject(root, "colors", colors_array);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_value);

    // step 3: Operate
    cJSON_bool has_info = cJSON_HasObjectItem(root, "info");
    cJSON_AddBoolToObject(root, "has_info", has_info);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}