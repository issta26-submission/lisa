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
//<ID> 226
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
    cJSON *str = cJSON_CreateString("hello");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    double val0 = 0.0;
    double val1 = 0.0;
    const char *version = cJSON_Version();
    (void)version;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "greeting", str);

    // step 3: Operate / Validate
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    char *g = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    buffer[1] = g ? g[0] : '\0';
    (void)val0;
    (void)val1;
    (void)g;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}