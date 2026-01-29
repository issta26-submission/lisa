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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *enabled_item = NULL;
    cJSON *count_item = NULL;
    cJSON *replacement = NULL;
    double count_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 1.0);
    cJSON_AddTrueToObject(root, "enabled");
    replacement = cJSON_CreateNumber(4.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "count", replacement);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    enabled_item = cJSON_GetObjectItem(parsed, "enabled");
    count_item = cJSON_GetObjectItem(parsed, "count");
    count_val = cJSON_GetNumberValue(count_item);
    buffer[0] = (char)('0' + (int)count_val);
    buffer[1] = (char)('0' + (int)cJSON_IsTrue(enabled_item));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}