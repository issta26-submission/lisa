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
//<ID> 1336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[false,true,null],\"count\":3}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *extras = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(extras, num_item);
    cJSON *str_item = cJSON_CreateString("added");
    cJSON_AddItemToArray(extras, str_item);
    cJSON_AddItemToObject(parsed, "extras", extras);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first);
    cJSON *flag_num = cJSON_CreateNumber((double)first_is_false);
    cJSON_AddItemToArray(items, flag_num);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(parsed, buf, buf_len, 1);
    cJSON *dup = cJSON_Duplicate(parsed, 1);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(dup);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    if (printed_len) memcpy(copy, printed, printed_len + 1);
    cJSON_Minify(copy);
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}