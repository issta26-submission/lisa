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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char json_str[] = "{\"value\":10,\"nested\":{\"a\":1}}";
    size_t json_len = sizeof(json_str) - 1;
    cJSON *root = cJSON_ParseWithLength(json_str, json_len);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value = cJSON_GetNumberValue(value_item);
    cJSON *created_num = cJSON_CreateNumber(3.14);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    char *printed = NULL;
    cJSON *added_num = NULL;
    cJSON *sum_item = NULL;
    double sum_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "manual_num", created_num);
    added_num = cJSON_AddNumberToObject(root, "sum", value + 3.14);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    sum_item = cJSON_GetObjectItem(root, "sum");
    sum_val = cJSON_GetNumberValue(sum_item);
    buffer[0] = (char)('0' + (int)sum_val % 10);
    buffer[1] = (char)('A' + ((int)sum_val % 26));
    buffer[2] = printed ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}