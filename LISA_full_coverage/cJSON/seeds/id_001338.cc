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
//<ID> 1338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"items\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *num = cJSON_CreateNumber(99);
    cJSON_AddItemToArray(extras, num);

    // step 3: Operate
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON *flag_echo = cJSON_CreateBool(flag_is_false);
    cJSON_AddItemToObject(root, "flag_echo", flag_echo);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "dup", dup);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}