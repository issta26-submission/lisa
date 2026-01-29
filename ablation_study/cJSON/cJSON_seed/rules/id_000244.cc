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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(5.25);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    double n1 = 0.0;
    double n2 = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(values);
    item0 = cJSON_GetArrayItem(values, 0);
    item1 = cJSON_GetArrayItem(values, 1);
    n1 = cJSON_GetNumberValue(item0);
    n2 = cJSON_GetNumberValue(item1);
    sum = n1 + n2;
    cJSON_AddNumberToObject(metrics, "sum", sum);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)n1;
    (void)n2;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}