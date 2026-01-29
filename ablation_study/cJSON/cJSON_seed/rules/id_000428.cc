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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON *label = cJSON_CreateString("example");
    int arr_size = 0;
    cJSON *item = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    int iv0 = 0;
    int iv1 = 0;

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(item);
    item = cJSON_GetArrayItem(arr, 1);
    val1 = cJSON_GetNumberValue(item);
    iv0 = (int)val0;
    iv1 = (int)val1;
    scratch[0] = (char)('0' + (iv0 % 10));
    scratch[1] = (char)(',');
    scratch[2] = (char)('0' + (iv1 % 10));
    scratch[3] = (char)(',');
    scratch[4] = (char)('0' + (arr_size % 10));
    scratch[5] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}