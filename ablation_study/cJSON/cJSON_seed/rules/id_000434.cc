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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON *obj = cJSON_CreateObject();
    cJSON *inner_num = cJSON_CreateNumber(7.5);
    cJSON *retr_arr = NULL;
    cJSON *retr_item = NULL;
    cJSON *retr_inner = NULL;
    char *printed = NULL;
    double extracted = 0.0;
    double inner_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "object", obj);
    cJSON_AddItemToObject(obj, "inner", inner_num);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    retr_arr = cJSON_GetObjectItem(root, "array");
    retr_item = cJSON_GetArrayItem(retr_arr, 1);
    extracted = cJSON_GetNumberValue(retr_item);
    retr_inner = cJSON_GetObjectItem(obj, "inner");
    inner_val = cJSON_GetNumberValue(retr_inner);
    buffer[0] = (char)('0' + ((int)extracted % 10));
    buffer[1] = (char)('A' + ((int)inner_val % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}