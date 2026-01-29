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
//<ID> 1333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON *old_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "answer", old_num);
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(84.0);
    cJSON_ReplaceItemViaPointer(metrics, old_num, new_num);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(values);
    cJSON *retrieved_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *retrieved_answer = cJSON_GetObjectItem(retrieved_metrics, "answer");
    double answer_value = cJSON_GetNumberValue(retrieved_answer);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (arr_size % 10));
    ((char *)scratch)[2] = (char)((int)answer_value);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}