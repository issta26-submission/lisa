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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *tmp_buf = (char *)cJSON_malloc(16);
    memset(tmp_buf, 0, 16);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON *parsed = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *item = NULL;
    int arr_size = 0;
    int item_value = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_array = cJSON_GetObjectItem(parsed, "numbers");
    arr_size = cJSON_GetArraySize(retrieved_array);
    item = cJSON_GetArrayItem(retrieved_array, 1);
    item_value = (int)cJSON_GetNumberValue(item);
    tmp_buf[0] = (char)('0' + arr_size);
    tmp_buf[1] = ':';
    tmp_buf[2] = (char)('0' + (item_value / 10));
    tmp_buf[3] = (char)('0' + (item_value % 10));
    tmp_buf[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}