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
//<ID> 996
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char input_json[] = "{\"meta\": {\"title\": \"example\"}, \"data\": {}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(input_json, &parse_end, 1);

    // step 2: Configure
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON *second_item = cJSON_GetArrayItem(str_array, 1);
    const char *second_value = cJSON_GetStringValue(second_item);
    cJSON_AddStringToObject(root, "second_string", second_value);

    // step 3: Operate
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON_AddBoolToObject(root, "has_strings", has_strings);
    char *printed = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}