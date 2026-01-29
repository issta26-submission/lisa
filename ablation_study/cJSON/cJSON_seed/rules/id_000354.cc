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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    int count = 3;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *str_array = cJSON_CreateStringArray(strings, count);
    cJSON *parsed = NULL;
    cJSON *arr_from_parsed = NULL;
    cJSON *item0 = NULL;
    cJSON *parsed_item0 = NULL;
    char *printed = NULL;
    cJSON_bool is_str = 0;
    cJSON_bool is_str_parsed = 0;
    const char *s0 = NULL;
    size_t len = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "arr", str_array);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    len = printed ? strlen(printed) : 0;
    parsed = cJSON_ParseWithLength(printed, len);
    item0 = cJSON_GetArrayItem(str_array, 0);
    is_str = cJSON_IsString(item0);
    s0 = cJSON_GetStringValue(item0);
    arr_from_parsed = cJSON_GetObjectItem(parsed, "arr");
    parsed_item0 = cJSON_GetArrayItem(arr_from_parsed, 0);
    is_str_parsed = cJSON_IsString(parsed_item0);
    buffer[0] = (char)('0' + (int)is_str);
    buffer[1] = s0 ? s0[0] : 'x';
    buffer[2] = (char)('0' + (int)is_str_parsed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}