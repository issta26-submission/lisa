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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *dup_arr = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    char *printed_dup = NULL;
    cJSON_bool equal = 0;
    double first_value = 0.0;
    int size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "count", 2);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    dup_arr = cJSON_Duplicate(arr, 1);
    printed_dup = cJSON_Print(dup_arr);
    equal = cJSON_Compare(arr, dup_arr, 1);
    size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    buffer[0] = (char)('0' + (char)equal);
    buffer[1] = (char)('0' + (char)((int)first_value));
    buffer[2] = (char)('0' + (char)size);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}