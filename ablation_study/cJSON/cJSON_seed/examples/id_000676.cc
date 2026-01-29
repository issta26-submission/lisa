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
//<ID> 676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON *n3 = cJSON_CreateNumber(-7.5);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(numbers));
    cJSON_AddItemReferenceToArray(numbers, n2);
    double set_result = cJSON_SetNumberHelper(n3, 100.5);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItemCaseSensitive(parsed, "numbers");
    int parsed_size = cJSON_GetArraySize(parsed_numbers);
    cJSON *first_item = cJSON_GetArrayItem(parsed_numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    (void)set_result;
    (void)parsed_size;
    (void)first_value;
    // API sequence test completed successfully
    return 66;
}