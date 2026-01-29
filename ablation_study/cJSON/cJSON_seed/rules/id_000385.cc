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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *greeting_ref = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    double num_val = 0.0;
    cJSON_bool raw_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "initial", 3.14);
    greeting_ref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToObject(root, "greeting", greeting_ref);
    raw_flag = cJSON_IsRaw(greeting_ref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "initial");
    num_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "doubled", num_val * 2.0);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "greeting");

    // step 4: Cleanup
    buffer[0] = (char)('0' + (int)num_val);
    buffer[1] = (char)('0' + (int)(num_val * 2.0));
    buffer[2] = (char)('0' + (int)raw_flag);
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}