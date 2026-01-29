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
//<ID> 508
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
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *num3 = cJSON_CreateNumber(3.25);
    cJSON *single = cJSON_CreateNumber(42.0);
    cJSON *dup = NULL;
    char *printed = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "single", single);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);
    buffer[0] = (char)('0' + (char)equal);
    buffer[1] = '\0';
    buffer[2] = (printed != NULL && printed[0] != '\0') ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}