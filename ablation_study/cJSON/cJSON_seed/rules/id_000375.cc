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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = NULL;
    size_t printed_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *replacement = NULL;
    double orig_val = 0.0;
    double replaced_val = 0.0;
    const char *num_key = "value";
    const char *bool_key = "enabled";

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, num_key, 123.456);
    cJSON_AddTrueToObject(root, bool_key);
    printed = cJSON_Print(root);
    printed_len = strlen(printed);
    buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    parsed = cJSON_Parse(buffer);

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(parsed, num_key);
    orig_val = cJSON_GetNumberValue(num_item);
    replacement = cJSON_CreateNumber(orig_val * 2.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, num_key, replacement);
    num_item = cJSON_GetObjectItem(parsed, num_key);
    replaced_val = cJSON_GetNumberValue(num_item);
    bool_item = cJSON_GetObjectItem(parsed, bool_key);
    buffer[0] = (char)('0' + (int)(replaced_val / orig_val));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}