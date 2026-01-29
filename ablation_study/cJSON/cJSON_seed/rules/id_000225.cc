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
//<ID> 225
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
    cJSON *num3 = cJSON_CreateNumber(-3.14);
    cJSON *label = cJSON_CreateString("example");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *ver = cJSON_Version();
    double extracted = 0.0;
    double total = 0.0;
    const char *label_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", label);
    buffer[1] = ver[0];

    // step 3: Operate / Validate
    extracted = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    total = extracted + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    label_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    (void)total;
    (void)label_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}