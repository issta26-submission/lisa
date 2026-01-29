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
//<ID> 437
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
    cJSON *num1 = cJSON_CreateNumber(123.0);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON *inner = cJSON_CreateObject();
    cJSON *inner_num = cJSON_CreateNumber(9.0);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_values = NULL;
    cJSON *item0 = NULL;
    double first_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(inner, "inner_num", inner_num);
    cJSON_AddItemToObject(root, "inner", inner);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_values = cJSON_GetObjectItem(parsed, "values");
    item0 = cJSON_GetArrayItem(retr_values, 0);
    first_val = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + ((int)first_val % 10));
    buffer[1] = '_';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}