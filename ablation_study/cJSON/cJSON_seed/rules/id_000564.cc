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
//<ID> 564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_str = "hello_cJSON";
    cJSON *array = cJSON_CreateArray();
    cJSON *str_ref = cJSON_CreateStringReference(static_str);
    char *printed = NULL;
    cJSON *item0 = NULL;
    cJSON_bool is_str = 0;
    const char *retrieved = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_ref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted((const cJSON *)array);
    item0 = cJSON_GetArrayItem((const cJSON *)array, 0);
    is_str = cJSON_IsString((const cJSON *)item0);
    retrieved = item0 ? cJSON_GetStringValue(item0) : NULL;
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';
    buffer[0] = retrieved ? retrieved[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}