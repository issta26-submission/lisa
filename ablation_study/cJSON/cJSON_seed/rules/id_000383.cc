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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *greet_item = NULL;
    cJSON *strref = NULL;
    double num_val = 0.0;
    cJSON_bool israw = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "num", 42.0);
    strref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToObject(root, "greet", strref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "num");
    num_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "double_num", num_val * 2.0);
    greet_item = cJSON_GetObjectItem(parsed, "greet");
    israw = cJSON_IsRaw(greet_item);
    buffer[0] = (char)('0' + (int)num_val % 10);
    buffer[1] = (char)('0' + (int)israw);
    buffer[2] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "num");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}