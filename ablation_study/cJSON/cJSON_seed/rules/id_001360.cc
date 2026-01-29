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
//<ID> 1360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *new_raw = NULL;
    const char *alpha_value = NULL;
    char *buffer = NULL;
    char *printed = NULL;
    cJSON_bool was_raw = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "alpha", "one");
    cJSON_AddStringToObject(root, "temp", "to_replace");

    // step 3: Operate / Validate
    buffer = (char *)cJSON_malloc((size_t)9);
    buffer[0] = 'r';
    buffer[1] = 'a';
    buffer[2] = 'w';
    buffer[3] = '_';
    buffer[4] = 'v';
    buffer[5] = 'a';
    buffer[6] = 'l';
    buffer[7] = '1';
    buffer[8] = '\0';
    new_raw = cJSON_CreateRaw(buffer);
    replaced = cJSON_ReplaceItemInObject(root, "temp", new_raw);
    was_raw = cJSON_IsRaw(new_raw);
    alpha_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "alpha"));
    printed = cJSON_PrintUnformatted(root);
    (void)alpha_value;
    (void)was_raw;
    (void)replaced;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}