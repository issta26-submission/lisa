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
//<ID> 1042
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"orig\",\"items\":[1,2,3]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "modified_name");
    cJSON *items_item = cJSON_GetObjectItem(root, "items");

    // step 3: Operate
    cJSON_bool is_array = cJSON_IsArray(items_item);
    cJSON *flag = cJSON_CreateBool(is_array);
    cJSON_AddItemToObjectCS(root, "items_are_array", flag);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(out, "name_copy", cJSON_CreateString(cJSON_GetStringValue(name_item)));
    char *compact = cJSON_PrintUnformatted(out);
    int buf_len = 256;
    char *print_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(print_buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(out, print_buf, buf_len, 0);

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);
    // API sequence test completed successfully
    return 66;
}