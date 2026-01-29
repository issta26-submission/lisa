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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_numbers = NULL;
    cJSON *first_item = NULL;
    cJSON *count_item = NULL;
    double first_val = 0.0;
    double count_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_numbers = cJSON_GetObjectItem(parsed, "numbers");
    first_item = cJSON_GetArrayItem(retr_numbers, 0);
    first_val = cJSON_GetNumberValue(first_item);
    count_item = cJSON_GetObjectItem(parsed, "count");
    count_val = cJSON_GetNumberValue(count_item);
    buffer[0] = (char)('A' + ((int)first_val % 26));
    buffer[1] = (char)('0' + ((int)count_val % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}