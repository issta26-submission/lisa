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
//<ID> 961
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *extra_num = NULL;
    cJSON *retr_extra = NULL;
    cJSON *first_in_arr = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    int arr_size = 0;
    double extra_val = 0.0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    num1 = cJSON_CreateNumber(3.1415);
    num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddRawToObject(root, "metadata", "{\"ok\":true}");
    extra_num = cJSON_CreateNumber(-7.25);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arr_size = cJSON_GetArraySize(arr);
    first_in_arr = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_in_arr);
    retr_extra = cJSON_GetObjectItem(root, "extra");
    extra_val = cJSON_GetNumberValue(retr_extra);
    scratch = (char *)cJSON_malloc((size_t)16);
    memset(scratch, 0, (size_t)16);
    scratch[0] = (char)('0' + (arr_size % 10));
    scratch[1] = (char)('0' + (((int)extra_val % 10) + 10) % 10);
    scratch[2] = (char)('0' + (((int)first_val % 10) + 10) % 10);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}