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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *items[] = { "alpha", "beta", "gamma" };
    int item_count = 3;
    cJSON *str_array = cJSON_CreateStringArray(items, item_count);
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *parsed = NULL;
    cJSON *first_item = NULL;
    cJSON_bool first_is_string = 0;
    const char *first_value = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    printed = cJSON_PrintUnformatted(str_array);
    printed_len = (printed != NULL) ? strlen(printed) : 0;
    parsed = cJSON_ParseWithLength(printed, printed_len);

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(parsed, 0);
    first_is_string = cJSON_IsString(first_item);
    first_value = cJSON_GetStringValue(first_item);
    buffer[0] = (char)('0' + (int)first_is_string);
    buffer[1] = (first_value != NULL && first_value[0] != '\0') ? first_value[0] : 'x';
    buffer[2] = (char)('0' + item_count);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(str_array);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}