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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *retrieved = NULL;
    cJSON *newnum = NULL;
    cJSON *updated = NULL;
    double original_val = 0.0;
    double updated_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddTrueToObject(root, "enabled");
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    parsed = cJSON_Parse(buffer);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(parsed, "count");
    original_val = cJSON_GetNumberValue(retrieved);
    newnum = cJSON_CreateNumber(original_val + 10.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "count", newnum);
    updated = cJSON_GetObjectItem(parsed, "count");
    updated_val = cJSON_GetNumberValue(updated);
    buffer[0] = (char)('0' + (int)updated_val);
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}