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
//<ID> 716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char parsed_json[] = "{\"parsed_num\":123,\"tag\":\"x\",\"arr\":[5,6,7]}";
    size_t parsed_len = sizeof(parsed_json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(parsed_json, parsed_len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    int numbers[3] = {100, 200, 300};
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObjectCS(root, "ints", int_arr);

    // step 3: Operate and Validate
    cJSON *parsed_arr = cJSON_GetObjectItemCaseSensitive(parsed, "arr");
    cJSON *detached = cJSON_DetachItemFromArray(parsed_arr, 1);
    cJSON_AddItemToArray(int_arr, detached);
    int ints_count = cJSON_GetArraySize(int_arr);
    cJSON_AddNumberToObject(root, "ints_count", (double)ints_count);
    int parsed_count = cJSON_GetArraySize(parsed_arr);
    cJSON_AddNumberToObject(root, "parsed_arr_count", (double)parsed_count);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}