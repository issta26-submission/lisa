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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("sample");
    int size = 0;
    cJSON *second_item = NULL;
    const char *name_str = NULL;
    double second_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(array);
    second_item = cJSON_GetArrayItem(array, 1);
    second_val = cJSON_GetNumberValue(second_item);
    name_str = cJSON_GetStringValue(name);
    buffer[0] = name_str[0];
    buffer[1] = (char)('0' + (int)size);
    buffer[2] = (char)('0' + (int)second_val);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}