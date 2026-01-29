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
//<ID> 637
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
    cJSON *numbers = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *num2 = cJSON_CreateNumber(4.56);
    cJSON_AddItemToArray(numbers, num1);
    cJSON_AddItemToArray(numbers, num2);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddStringToObject(root, "note", "initial payload");

    // step 2: Configure
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_numbers);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);
    sprintf((char *)buffer, "snapshot=%s", snapshot);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *parsed_dup = cJSON_GetObjectItem(parsed, "numbers_copy");
    int count = cJSON_GetArraySize(parsed_numbers);
    cJSON_DeleteItemFromObject(parsed, "note");

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;dup_exists=%p", version, count, (void *)parsed_dup);
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}