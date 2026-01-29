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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(1.5);
    cJSON *num1 = cJSON_CreateNumber(2.75);
    cJSON *num2 = cJSON_CreateNumber(-3.0);
    cJSON *root_copy = NULL;
    cJSON *arr_copy = NULL;
    char *printed = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    arr_copy = cJSON_Duplicate(arr, 1);
    root_copy = cJSON_CreateObject();
    cJSON_AddItemToObject(root_copy, "values", arr_copy);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    equal = cJSON_Compare(root, root_copy, 1);
    buffer[0] = (char)('0' + (char)equal);
    buffer[1] = '\0';
    buffer[2] = printed[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(root_copy);

    // API sequence test completed successfully
    return 66;
}