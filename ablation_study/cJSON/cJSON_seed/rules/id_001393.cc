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
//<ID> 1393
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
    cJSON *dup_arr = NULL;
    char *printed = NULL;
    double *numbers = NULL;
    int arr_size = 0;
    const int count = 5;

    // step 2: Setup / Configure
    numbers = (double *)cJSON_malloc(sizeof(double) * count);
    memset(numbers, 0, sizeof(double) * count);
    numbers[0] = 1.25;
    numbers[1] = -3.5;
    numbers[2] = 0.0;
    numbers[3] = 42.125;
    numbers[4] = 3.14159;
    arr = cJSON_CreateDoubleArray((const double *)numbers, count);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    dup_arr = cJSON_Duplicate(arr, 1);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(numbers);
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}