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
//<ID> 889
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    double scalar = 0.0;
    int arr_size = 0;
    int sum_int = 0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num0 = cJSON_CreateNumber(100.25);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(200.75);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.14);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    scalar = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "scalar"));
    sum_int = (int)(val0 + val1 + scalar);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('0' + (sum_int % 10));
    (void)buffer;
    (void)val0;
    (void)val1;
    (void)scalar;

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}