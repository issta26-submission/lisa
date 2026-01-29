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
//<ID> 506
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
    cJSON *num1 = cJSON_CreateNumber(3.14159);
    cJSON *num2 = cJSON_CreateNumber(3.14159);
    cJSON *num3 = cJSON_CreateNumber(2.71828);
    char *printed = NULL;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "pi", num2);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cmp_equal = cJSON_Compare(num1, num2, 1);
    buffer[0] = (char)('0' + (char)cmp_equal);
    buffer[1] = (printed != NULL) ? printed[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}