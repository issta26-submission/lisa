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
//<ID> 622
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddNumberToObject(root, "total", 100.0);

    // step 2: Configure
    char status[256];
    memset(status, 0, sizeof(status));
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON_bool null_check = cJSON_IsNull(parsed_numbers);
    cJSON *first_item = cJSON_GetArrayItem(parsed_numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    void *buf = cJSON_malloc(64);
    memset(buf, 0, 64);
    sprintf((char *)buf, "ver=%s;null=%d;first=%.0f", version ? version : "null", (int)null_check, first_value);
    sprintf(status, "%s", (char *)buf);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "total");
    cJSON_free(snapshot);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}