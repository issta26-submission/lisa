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
//<ID> 1190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{\"top\":{\"keep\":\"yes\",\"remove\":\"no\"},\"list\":[\"a\",\"b\",\"c\"]}";
    cJSON_Minify(json_text);
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *top = cJSON_GetObjectItem(parsed, "top");
    cJSON *list = cJSON_GetObjectItem(parsed, "list");
    cJSON *newstr = cJSON_CreateString("new_element");
    cJSON_AddItemToArray(list, newstr);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(top, "enabled", false_item);
    const char *keep_val = cJSON_GetStringValue(cJSON_GetObjectItem(top, "keep"));
    cJSON *keep_copy = cJSON_CreateString(keep_val);
    cJSON_AddItemToObject(parsed, "keep_copy", keep_copy);

    // step 3: Operate
    cJSON_DeleteItemFromArray(list, 1);
    char *output = cJSON_PrintUnformatted(parsed);

    // step 4: Validate & Cleanup
    cJSON_free(output);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}