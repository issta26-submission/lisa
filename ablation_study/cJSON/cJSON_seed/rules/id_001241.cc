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
//<ID> 1241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *flag = NULL;
    cJSON *arr = NULL;
    cJSON *temp = NULL;
    char *unfmt = NULL;
    char buffer[256];
    cJSON_bool prealloc_result = 0;
    cJSON_bool is_true = 0;
    double num_value = 0.0;
    int arr_size = 0;

    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14159);
    flag = cJSON_CreateTrue();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    temp = cJSON_GetObjectItem(root, "value");
    num_value = cJSON_GetNumberValue(temp);
    is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "list"));
    unfmt = cJSON_PrintUnformatted(root);
    prealloc_result = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)num_value;
    (void)is_true;
    (void)arr_size;
    (void)prealloc_result;

    // step 4: Cleanup
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}