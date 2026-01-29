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
//<ID> 629
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
    int numbers_arr[] = { 7, 11, 13, 17 };
    cJSON *numbers = cJSON_CreateIntArray(numbers_arr, 4);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON_AddStringToObject(root, "creator", "tester");

    // step 2: Configure
    cJSON_AddNumberToObject(root, "constant", 3.14159);
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    sprintf((char *)buffer, "%s", snapshot);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    cJSON_bool null_check = cJSON_IsNull(cJSON_GetObjectItem(parsed, "maybe"));
    cJSON_DeleteItemFromObject(parsed, "constant");
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;null=%d;buf=%s",
            version ? version : "null",
            parsed_count,
            (int)null_check,
            (char *)buffer);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}