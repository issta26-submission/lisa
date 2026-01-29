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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", array);

    // step 2: Configure
    cJSON *item0 = cJSON_CreateNumber(10.0);
    cJSON *item1 = cJSON_CreateNumber(20.0);
    cJSON *item2 = cJSON_CreateNumber(30.5);
    cJSON_AddItemToArray(array, item0);
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);

    // step 3: Operate
    cJSON *got_array = cJSON_GetObjectItem(root, "data");
    cJSON *second_item = cJSON_GetArrayItem(got_array, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_item);
    (void)second_invalid;
    char *printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);

    // step 4: Validate & Cleanup
    cJSON *first_item = cJSON_GetArrayItem(got_array, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first_item);
    (void)first_invalid;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}