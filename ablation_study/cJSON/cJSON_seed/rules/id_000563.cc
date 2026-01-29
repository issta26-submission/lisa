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
//<ID> 563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *array = cJSON_CreateArray();
    const char *static_text = "sample_text";
    cJSON *str_ref = cJSON_CreateStringReference(static_text);
    char *printed = NULL;
    cJSON *item0 = NULL;
    cJSON_bool is_str = 0;
    const char *got_str = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_ref);

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(array, 0);
    is_str = cJSON_IsString(item0);
    got_str = cJSON_GetStringValue(item0);
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 200 ? printed_len : 200;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = is_str ? (got_str ? got_str[0] : '\0') : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}