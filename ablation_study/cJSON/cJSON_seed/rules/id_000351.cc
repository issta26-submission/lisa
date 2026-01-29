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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *names[] = { "Alice", "Bob", "Carol" };
    cJSON *str_array = cJSON_CreateStringArray(names, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *parsed_array = NULL;
    cJSON *item1 = NULL;
    cJSON_bool is_str = 0;
    const char *item_val = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "names", str_array);
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_ParseWithLength(printed, (size_t)1024);

    // step 3: Operate / Validate
    parsed_array = cJSON_GetObjectItem(parsed, "names");
    item1 = cJSON_GetArrayItem(parsed_array, 1);
    is_str = cJSON_IsString(item1);
    item_val = cJSON_GetStringValue(item1);
    buffer[0] = (char)('0' + (int)is_str);
    buffer[1] = item_val ? item_val[0] : '\0';
    buffer[2] = printed ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}