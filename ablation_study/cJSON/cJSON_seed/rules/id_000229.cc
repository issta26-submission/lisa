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
//<ID> 229
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON *msg = cJSON_CreateString("hello world");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    char *sval = NULL;
    cJSON *tmp1 = NULL;
    cJSON *tmp2 = NULL;
    cJSON *tmp_msg = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "message", msg);

    // step 3: Operate / Validate
    tmp1 = cJSON_GetArrayItem(arr, 0);
    tmp2 = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(tmp1);
    v2 = cJSON_GetNumberValue(tmp2);
    sum = v1 + v2;
    tmp_msg = cJSON_GetObjectItem(root, "message");
    sval = cJSON_GetStringValue(tmp_msg);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)sum;
    (void)sval;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}