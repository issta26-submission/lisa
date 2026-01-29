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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.5);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *sum_node = NULL;
    char *printed = NULL;
    const char *gstr = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double v3 = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate / Validate
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    sum = v1 + v2 + v3;
    sum_node = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_node);
    gstr = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    printed = cJSON_PrintUnformatted(root);
    char first_print = printed[0];
    char first_g = gstr[0];
    (void)first_print;
    (void)first_g;
    (void)v1;
    (void)v2;
    (void)v3;
    (void)sum;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}