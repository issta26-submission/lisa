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
//<ID> 1331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"values\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON *values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    cJSON *array = cJSON_CreateArray();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "orig", parsed);
    cJSON_AddItemToObject(root, "extras", array);
    cJSON *num_flag = cJSON_CreateNumber((double)flag_is_false);
    cJSON_AddItemToArray(array, num_flag);
    cJSON *values_ref = cJSON_CreateArrayReference(values);
    cJSON_AddItemReferenceToArray(array, values_ref);

    // step 3: Operate
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    size_t printed_len = strlen(buf);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, buf, printed_len + 1);
    cJSON_Minify(copy);
    cJSON_free(copy);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}