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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "list", string_array);

    // step 2: Configure
    char jsonbuf[] = " {\n    \"note\" : \" spaced value \",\n    \"num\" : 42\n} ";
    cJSON_Minify(jsonbuf);
    cJSON *minified_note = cJSON_CreateString(jsonbuf);
    cJSON_AddItemToObject(root, "minified", minified_note);

    // step 3: Operate & Validate
    cJSON_bool has_list = cJSON_HasObjectItem(root, "list");
    cJSON *flag = cJSON_CreateNumber((double)has_list);
    cJSON_AddItemToObject(root, "list_exists", flag);
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *second_item = cJSON_GetArrayItem(got_list, 1);
    const char *second_text = cJSON_GetStringValue(second_item);
    cJSON *picked = cJSON_CreateString(second_text ? second_text : "");
    cJSON_AddItemToObject(root, "picked", picked);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}