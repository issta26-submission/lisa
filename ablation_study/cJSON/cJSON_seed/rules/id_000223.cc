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
//<ID> 223
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
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *label = cJSON_CreateString("values");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    double val1 = 0.0;
    double val2 = 0.0;
    char *strval = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    strval = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    printed = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = printed ? printed[0] : '\0';
    (void)val1;
    (void)val2;
    (void)strval;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}