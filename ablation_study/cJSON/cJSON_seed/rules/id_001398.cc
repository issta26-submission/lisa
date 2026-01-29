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
//<ID> 1398
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
    double *dnums = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    dnums = (double *)cJSON_malloc(sizeof(double) * 3);
    memset(dnums, 0, sizeof(double) * 3);
    dnums[0] = 1.1;
    dnums[1] = 2.2;
    dnums[2] = 3.3;
    arr = cJSON_CreateDoubleArray(dnums, 3);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "values", arr);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(dnums);
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}