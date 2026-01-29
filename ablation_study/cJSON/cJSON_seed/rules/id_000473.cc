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
//<ID> 473
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
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retr_items = NULL;
    cJSON *greet_item = NULL;
    const char *greet_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 0);
    parsed = cJSON_Parse(printed);
    retr_items = cJSON_GetObjectItem(parsed, "items");
    arr_size = cJSON_GetArraySize(retr_items);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';
    greet_item = cJSON_GetObjectItem(parsed, "greeting");
    greet_str = cJSON_GetStringValue(greet_item);
    buffer[2] = greet_str[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}