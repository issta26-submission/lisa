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
//<ID> 1335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[false,true,123],\"name\":\"example\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON *synth = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "synthesized", synth);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first_item);
    cJSON *bool_from_check = cJSON_CreateBool(first_is_false);
    cJSON_AddItemToArray(synth, bool_from_check);
    cJSON *third_item = cJSON_GetArrayItem(items, 2);
    cJSON *third_dup = cJSON_Duplicate(third_item, 1);
    cJSON_AddItemToArray(synth, third_dup);

    // step 4: Validate & Cleanup
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    size_t printed_len = strlen(buf);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, buf, printed_len + 1);
    cJSON_Minify(copy);
    cJSON_free(copy);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}