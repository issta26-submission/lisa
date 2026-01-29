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
//<ID> 1396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup_arr = NULL;
    double *numbers = NULL;
    char *printed = NULL;
    int arr_size = 0;
    int dup_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    numbers = (double *)cJSON_malloc(sizeof(double) * 4);
    memset(numbers, 0, sizeof(double) * 4);
    numbers[0] = 1.25;
    numbers[1] = 2.5;
    numbers[2] = 3.75;
    numbers[3] = 4.0;
    arr = cJSON_CreateDoubleArray((const double *)numbers, 4);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    dup_arr = cJSON_Duplicate(arr, 1);
    arr_size = cJSON_GetArraySize(arr);
    dup_size = cJSON_GetArraySize(dup_arr);
    printed = cJSON_PrintBuffered(root, 256, 1);
    (void)arr_size;
    (void)dup_size;

    // step 4: Cleanup
    cJSON_free(numbers);
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}