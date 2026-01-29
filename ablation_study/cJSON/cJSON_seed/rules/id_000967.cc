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
//<ID> 967
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *ans = NULL;
    cJSON *dup = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    double first_val = 0.0;
    double ans_val = 0.0;
    int arr_size = 0;
    cJSON_bool cmp = 0;
    const int scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    n1 = cJSON_CreateNumber(3.1415);
    n2 = cJSON_CreateNumber(2.71828);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    ans = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", ans);
    cJSON_AddRawToObject(root, "metadata", "{\"valid\":true,\"source\":\"sensor\"}");

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    ans_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    dup = cJSON_Duplicate(root, 1);
    cmp = cJSON_Compare(root, dup, 1);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = (char)('0' + (arr_size % 10));
    scratch[1] = (char)('0' + ((int)first_val % 10));
    scratch[2] = (char)('0' + ((int)ans_val % 10));
    scratch[3] = (char)('0' + (int)cmp);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}