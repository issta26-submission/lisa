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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = NULL;
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_ref = cJSON_CreateNumber(123.75);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_nested = NULL;
    cJSON *retr_arr = NULL;
    cJSON *item0 = NULL;
    const char *parse_end = NULL;
    int arr_size = 0;
    double value0 = 0.0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", "example");
    nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddItemToObject(nested, "values", arr);
    cJSON_AddItemReferenceToArray(arr, num_ref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);
    retr_nested = cJSON_GetObjectItem(parsed, "nested");
    retr_arr = cJSON_GetObjectItem(retr_nested, "values");
    arr_size = cJSON_GetArraySize(retr_arr);
    item0 = cJSON_GetArrayItem(retr_arr, 0);
    value0 = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)value0 % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(num_ref);

    // API sequence test completed successfully
    return 66;
}