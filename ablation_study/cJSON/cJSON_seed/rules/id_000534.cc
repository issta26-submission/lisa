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
//<ID> 534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    const char *json = "{\"name\":\"example\",\"nested\":{\"inner\":\"value\"},\"arr\":[\"a\",\"b\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);

    // step 2: Setup / Configure
    cJSON *added = cJSON_CreateString("added_value");
    cJSON_AddItemToObject(root, "new", added);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *c_item = cJSON_CreateString("c");
    cJSON_AddItemToArray(arr, c_item);

    // step 3: Operate / Validate
    cJSON *nested = cJSON_GetObjectItemCaseSensitive(root, "nested");
    cJSON *inner = cJSON_GetObjectItemCaseSensitive(nested, "inner");
    char *inner_str = cJSON_GetStringValue(inner);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len < 256 ? printed_len : 256;
    memcpy(buffer, printed, copy_len);
    buffer[copy_len > 0 ? copy_len - 1 : 0] = inner_str ? inner_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}