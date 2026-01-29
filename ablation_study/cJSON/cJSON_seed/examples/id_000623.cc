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
//<ID> 623
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
    int nums[] = { 10, 20, 30, 40 };
    cJSON *numbers = cJSON_CreateIntArray(nums, 4);
    cJSON *name = cJSON_CreateString("fuzzer");
    cJSON *nothing = cJSON_AddNullToObject(root, "nothing");
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddNumberToObject(root, "count", 4.0);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *parsed_nothing = cJSON_GetObjectItem(parsed, "nothing");
    int parsed_size = cJSON_GetArraySize(parsed_numbers);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(parsed_numbers, 0));

    // step 3: Operate & Validate
    cJSON_bool is_null = cJSON_IsNull(parsed_nothing);
    void *status_buf = cJSON_malloc(256);
    memset(status_buf, 0, 256);
    sprintf((char *)status_buf,
            "ver=%s;size=%d;first=%.0f;isnull=%d",
            version ? version : "null",
            parsed_size,
            first_val,
            (int)is_null);
    cJSON_DeleteItemFromObject(parsed, "name");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(status_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}