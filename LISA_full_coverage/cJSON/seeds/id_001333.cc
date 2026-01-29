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
//<ID> 1333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[true,false,3],\"name\":\"test\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON *new_array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(new_array, num_item);
    cJSON_AddItemToObject(root, "newnums", new_array);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first_item);
    (void)first_is_false;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = 0;
    printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    if (printed) {
        memcpy(buffer, printed, printed_len + 1);
    }
    cJSON_Minify(buffer);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}