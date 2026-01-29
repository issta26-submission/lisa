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
//<ID> 1044
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"user\":\"alice\",\"flags\":[true,false],\"count\":5}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *user_item = cJSON_GetObjectItem(root, "user");
    cJSON_SetValuestring(user_item, "alice_modified");
    cJSON *flags_item = cJSON_GetObjectItem(root, "flags");
    cJSON_bool flags_is_array = cJSON_IsArray(flags_item);
    cJSON *out = cJSON_CreateObject();
    cJSON *flag_bool = cJSON_CreateBool(flags_is_array);
    cJSON_AddItemToObjectCS(out, "flags_is_array", flag_bool);
    cJSON *copied_user = cJSON_CreateString(cJSON_GetStringValue(user_item));
    cJSON_AddItemToObjectCS(out, "user_copied", copied_user);

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(out, print_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}