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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(items, 3);
    cJSON_AddItemToObject(root, "strings", str_array);

    // step 2: Configure
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "  {  \"greeting\"  :  \"hello world\"  }  ");
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *parsed_greet = cJSON_GetObjectItem(parsed, "greeting");
    const char *greet_text = cJSON_GetStringValue(parsed_greet);
    cJSON *greet_item = cJSON_CreateString(greet_text);
    cJSON_AddItemToObject(root, "greet_from_parsed", greet_item);
    cJSON_bool has_greet = cJSON_HasObjectItem(root, "greet_from_parsed");
    cJSON *has_num = cJSON_CreateNumber((double)has_greet);
    cJSON_AddItemToObject(root, "has_greet", has_num);

    // step 3: Operate & Validate
    cJSON *dup_arr = cJSON_Duplicate(str_array, 1);
    cJSON_AddItemToObject(root, "dup_strings", dup_arr);
    cJSON *first_item = cJSON_GetArrayItem(dup_arr, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *label = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_label", label);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}