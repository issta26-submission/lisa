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
//<ID> 960
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
    cJSON *single_num = NULL;
    cJSON *retrieved_single = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    int arr_size = 0;
    double first_val = 0.0;
    double single_val = 0.0;
    const char *arr_name = "numbers";
    const char *single_name = "pi";
    const char *raw_name = "meta";
    const char *raw_json = "{\"raw_key\":true}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, arr_name);
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    single_num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, single_name, single_num);
    cJSON_AddRawToObject(root, raw_name, raw_json);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    retrieved_single = cJSON_GetObjectItem(root, single_name);
    single_val = cJSON_GetNumberValue(retrieved_single);
    scratch = (char *)cJSON_malloc((size_t)32);
    memset(scratch, 0, (size_t)32);
    scratch[0] = (char)('0' + (arr_size % 10));
    scratch[1] = (char)('0' + ((int)first_val % 10));
    scratch[2] = (char)('0' + ((int)single_val % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}