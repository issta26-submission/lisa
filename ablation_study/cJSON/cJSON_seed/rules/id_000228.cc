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
//<ID> 228
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
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *label = cJSON_CreateString("values");
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    char *printed = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    char *sval = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(first);
    v2 = cJSON_GetNumberValue(second);
    sval = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = sval ? sval[0] : '\0';
    buffer[1] = printed ? printed[0] : '\0';
    (void)v1;
    (void)v2;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}