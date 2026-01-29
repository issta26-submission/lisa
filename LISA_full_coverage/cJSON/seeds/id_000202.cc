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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(100.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *num2 = cJSON_CreateNumber(200.5);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    char *buffered = cJSON_PrintBuffered(root, 128, 1);
    cJSON_Minify(buffered);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, array);
    int array_size = cJSON_GetArraySize(detached);
    cJSON *first_item = cJSON_GetArrayItem(detached, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *duplicate = cJSON_Duplicate(detached, 1);
    cJSON_bool equal = cJSON_Compare(detached, duplicate, 1);
    (void)array_size; (void)first_value; (void)equal;

    // step 4: Cleanup
    cJSON_free(buffered);
    cJSON_free(tmp_buf);
    cJSON_Delete(duplicate);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}