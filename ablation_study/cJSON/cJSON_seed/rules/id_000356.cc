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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *items[] = { "alpha", "beta", "gamma" };
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateStringArray(items, 3);
    cJSON *parsed = NULL;
    cJSON *arr_parsed = NULL;
    cJSON *first = NULL;
    cJSON_bool first_is_string = 0;
    const char *first_val = NULL;
    size_t len = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    printed = cJSON_PrintUnformatted(root);
    len = (printed != NULL) ? strlen(printed) : 0;
    parsed = cJSON_ParseWithLength(printed, len);

    // step 3: Operate / Validate
    arr_parsed = cJSON_GetObjectItem(parsed, "items");
    first = cJSON_GetArrayItem(arr_parsed, 0);
    first_is_string = cJSON_IsString(first);
    first_val = cJSON_GetStringValue(first);
    buffer[0] = first_val[0];
    buffer[1] = (char)('0' + (int)first_is_string);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}