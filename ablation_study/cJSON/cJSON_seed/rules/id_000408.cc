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
//<ID> 408
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
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *num3 = cJSON_CreateNumber(3.0);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON *parsed = NULL;
    cJSON *flag_item = NULL;
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);

    // step 3: Operate / Validate
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    printed = cJSON_PrintBuffered(root, 128, 1);
    parsed = cJSON_Parse(printed);
    flag_item = cJSON_AddBoolToObject(parsed, "flag", 1);
    cJSON_AddItemToObject(parsed, "raw", raw_item);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)flag_item->type);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}