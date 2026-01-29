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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *str = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "greeting", str);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(2.71828);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "pi", new_num);

    // step 3: Operate and Validate
    cJSON *num_item = cJSON_GetObjectItemCaseSensitive(root, "pi");
    cJSON *str_item = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    double value_after_replace = cJSON_GetNumberValue(num_item);
    cJSON_bool greeting_is_string = cJSON_IsString(str_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON *parsed = cJSON_Parse(unformatted);
    cJSON *parsed_num_item = cJSON_GetObjectItemCaseSensitive(parsed, "pi");
    double parsed_value = cJSON_GetNumberValue(parsed_num_item);
    int summary = (int)value_after_replace + (int)parsed_value + (int)greeting_is_string + root->type + parsed->type + (int)replaced;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}