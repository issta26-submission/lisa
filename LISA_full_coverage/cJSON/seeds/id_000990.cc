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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char input_json[] = "{\"name\":\"example\",\"items\":[]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(input_json, &parse_end, 1);

    // step 2: Configure
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON *first_item = cJSON_GetArrayItem(str_array, 0);
    char *first_value = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_string", first_value);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddBoolToObject(root, "has_items", has_items);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}