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
//<ID> 516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char json_literal[] = "{\"flag\":false,\"values\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_literal, (size_t)(sizeof(json_literal) - 1), &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num99 = cJSON_CreateNumber(99.0);
    cJSON *values = NULL;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "embedded", parsed);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "flag", true_item);
    values = cJSON_GetObjectItem(parsed, "values");
    size_before = cJSON_GetArraySize(values);
    cJSON_AddItemToArray(values, num99);
    size_after = cJSON_GetArraySize(values);

    // step 3: Operate / Validate
    buffer[0] = (char)('0' + (char)(size_before));
    buffer[1] = (char)('0' + (char)(size_after));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}