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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\": {\"existing\": \"yes\"}, \"numbers\": [1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "new_strings", str_array);
    cJSON_bool has_new = cJSON_HasObjectItem(root, "new_strings");
    cJSON_AddBoolToObject(root, "has_new_strings", has_new);
    cJSON *first_item = cJSON_GetArrayItem(str_array, 0);
    char *first_val = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_string", first_val);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_bool has_root = cJSON_HasObjectItem(root, "root");
    cJSON_AddBoolToObject(root, "has_root", has_root);
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}