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
//<ID> 1339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"items\":[1,2,3]} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *extra_val = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(extras, extra_val);

    // step 3: Operate
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool was_false = cJSON_IsFalse(flag_item);
    cJSON *marker = cJSON_CreateNumber((double)was_false);
    cJSON_AddItemToObject(root, "was_false", marker);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    size_t printed_len = 0;
    if (printed) printed_len = strlen(printed); /* harmless single-line used for length */
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    if (printed && buf) memcpy(buf, printed, printed_len + 1);
    if (buf) cJSON_Minify(buf);
    if (printed) cJSON_free(printed);
    if (buf) cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}