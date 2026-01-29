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
//<ID> 381
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
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *strref = NULL;
    cJSON *parsed_raw = NULL;
    cJSON_bool is_raw = 0;
    double num_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    strref = cJSON_CreateStringReference("hello_ref");
    cJSON_AddItemToObject(root, "greeting", strref);
    cJSON_AddRawToObject(root, "rawdata", "{\"inner\":true}");
    cJSON_AddNumberToObject(child, "sub", 3.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "count");
    num_val = cJSON_GetNumberValue(num_item);
    parsed_raw = cJSON_GetObjectItem(parsed, "rawdata");
    is_raw = cJSON_IsRaw(parsed_raw);
    buffer[0] = (char)('0' + (int)num_val);
    buffer[1] = (char)('0' + (int)is_raw);
    buffer[2] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "greeting");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}