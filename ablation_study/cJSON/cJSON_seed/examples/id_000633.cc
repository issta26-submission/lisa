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
//<ID> 633
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
    cJSON *one = cJSON_CreateNumber(1.0);
    cJSON *two = cJSON_CreateNumber(2.0);

    // step 2: Configure
    cJSON_AddItemToArray(numbers, one);
    cJSON_AddItemToArray(numbers, two);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddStringToObject(root, "creator", "tester");
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *parsed_creator = cJSON_GetObjectItem(parsed, "creator");
    cJSON *three = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(parsed_numbers, three);
    cJSON_DeleteItemFromObject(parsed, "creator");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(parsed_numbers, 0));
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;first=%.1f", version ? version : "null", parsed_count, first_value);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup_numbers);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}