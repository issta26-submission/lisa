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
//<ID> 966
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
    cJSON *n3 = NULL;
    cJSON *meta = NULL;
    cJSON *retrieved_numbers = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *raw_text = "{\"rawKey\":123}";
    int arr_size = 0;
    double first_val = 0.0;
    int scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    n1 = cJSON_CreateNumber(3.14);
    n2 = cJSON_CreateNumber(42.0);
    n3 = cJSON_CreateNumber(-1.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 3);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddRawToObject(root, "raw", raw_text);

    // step 3: Operate / Validate
    retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(retrieved_numbers);
    first_item = cJSON_GetArrayItem(retrieved_numbers, 0);
    first_val = cJSON_GetNumberValue(first_item);
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = printed[0];
    scratch[1] = (char)('0' + (arr_size % 10));
    scratch[2] = (char)('0' + ((int)first_val % 10 + 10) % 10);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}