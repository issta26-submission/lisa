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
//<ID> 447
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
    cJSON *num = cJSON_CreateNumber(7.5);
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;
    cJSON *retrieved_nested = NULL;
    cJSON *retr_arr = NULL;
    cJSON *item0 = NULL;
    double val0 = 0.0;
    int arr_size = 0;

    // step 2: Setup / Configure
    nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddStringToObject(nested, "greeting", "hello");
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_ParseWithLengthOpts(printed, 128, &parse_end, 1);
    retrieved_nested = cJSON_GetObjectItem(parsed, "nested");
    retr_arr = cJSON_GetObjectItem(parsed, "values");
    arr_size = cJSON_GetArraySize(retr_arr);
    item0 = cJSON_GetArrayItem(retr_arr, 0);
    val0 = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)val0 % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}