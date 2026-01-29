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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *json = "{\"value\":123, \"keep\":1}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42);
    char *printed = NULL;
    cJSON *retrieved_value = NULL;
    double value_num = 0.0;
    int value_int = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON *added = cJSON_AddNumberToObject(root, "added", 3.14);
    (void)cJSON_AddItemToObject(root, "forty_two", num_item);

    // step 3: Operate / Validate
    retrieved_value = cJSON_GetObjectItem(parsed, "value");
    value_num = cJSON_GetNumberValue(retrieved_value);
    value_int = (int)value_num;
    buffer[0] = (char)('0' + (value_int % 10));
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "keep");
    printed = cJSON_PrintUnformatted(root);
    buffer[1] = printed && printed[0] ? printed[0] : 'X';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}