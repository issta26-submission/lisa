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
//<ID> 387
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
    cJSON *raw_item = cJSON_CreateRaw("embedded_raw");
    cJSON *strref = cJSON_CreateStringReference("label_value");
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *raw_from_parsed = NULL;
    cJSON_bool raw_flag = 0;
    double count_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddItemToObject(root, "label", strref);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "count");
    count_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "next", count_val + 1.0);
    raw_from_parsed = cJSON_GetObjectItem(parsed, "raw");
    raw_flag = cJSON_IsRaw(raw_from_parsed);
    buffer[0] = (char)('0' + (int)count_val);
    buffer[1] = (char)('0' + (int)raw_flag);
    buffer[2] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "label");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}