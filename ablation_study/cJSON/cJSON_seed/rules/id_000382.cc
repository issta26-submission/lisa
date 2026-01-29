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
//<ID> 382
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
    cJSON *raw_item = NULL;
    cJSON *str_ref = NULL;
    cJSON *num_item = NULL;
    cJSON_bool raw_flag = 0;
    double count_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddNumberToObject(root, "count", 7.0);
    str_ref = cJSON_CreateStringReference("hello_world");
    cJSON_AddItemToObject(root, "greeting", str_ref);
    raw_item = cJSON_AddRawToObject(root, "raw_payload", "{\"x\":1,\"y\":2}");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "count");
    count_val = cJSON_GetNumberValue(num_item);
    buffer[0] = (char)('0' + (int)count_val);
    raw_item = cJSON_GetObjectItem(parsed, "raw_payload");
    raw_flag = cJSON_IsRaw(raw_item);
    buffer[1] = (char)('0' + (int)raw_flag);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "version");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}