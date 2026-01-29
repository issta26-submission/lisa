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
//<ID> 992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"example\",\"meta\":{\"version\":\"1.0\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);
    const char *colors[] = {"red", "green", "blue"};
    cJSON *colors_array = cJSON_CreateStringArray(colors, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "colors", colors_array);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddBoolToObject(root, "has_meta", has_meta);

    // step 3: Operate
    cJSON *first_color_item = cJSON_GetArrayItem(colors_array, 0);
    char *first_color_str = cJSON_GetStringValue(first_color_item);
    cJSON_AddStringToObject(root, "first_color", first_color_str);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}