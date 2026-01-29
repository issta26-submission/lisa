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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, s1);
    cJSON *n1 = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, n1);

    // step 2: Configure
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "  { \"note\" : null, \"num\": 42 }  ");
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *parsed_note = cJSON_GetObjectItem(parsed, "note");
    cJSON *raw_item = cJSON_CreateRaw("true");
    cJSON_AddItemToObject(parsed, "raw_added", raw_item);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_null = cJSON_IsNull(first);
    cJSON *flag_num = cJSON_CreateNumber((double)first_null);
    cJSON_AddItemToObject(root, "first_is_null", flag_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}