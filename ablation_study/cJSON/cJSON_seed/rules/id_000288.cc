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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    char *printed = NULL;
    int arr_size = 0;
    cJSON *found = NULL;
    cJSON *num = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddStringToObject(settings, "name", "example");
    cJSON_AddNumberToObject(settings, "version", 1.23);
    cJSON_AddItemToObject(settings, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateString("thirty"));

    // step 3: Operate / Validate
    found = cJSON_GetObjectItem(root, "settings");
    num = cJSON_GetObjectItem(found, "version");
    arr_size = cJSON_GetArraySize(values);
    printed = cJSON_Print(root);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = printed ? printed[0] : '\0';
    (void)num;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}