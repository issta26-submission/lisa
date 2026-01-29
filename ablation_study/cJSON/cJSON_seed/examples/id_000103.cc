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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "strings", str_array);

    // step 2: Configure
    char jsonbuf[] = "  { \"note\": \"  spaced value \" }  ";
    cJSON_Minify(jsonbuf);
    cJSON *minified = cJSON_CreateString(jsonbuf);
    cJSON_AddItemToObject(root, "minified", minified);

    // step 3: Operate & Validate
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON *has_flag = cJSON_CreateNumber((double)has_strings);
    cJSON_AddItemToObject(root, "has_strings", has_flag);
    const char *min_text = cJSON_GetStringValue(minified);
    cJSON *mirror = cJSON_CreateString(min_text);
    cJSON_AddItemToObject(root, "mirror", mirror);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}