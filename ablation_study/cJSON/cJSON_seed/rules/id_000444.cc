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
//<ID> 444
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *num = cJSON_CreateNumber(314.15);
    cJSON *ref = cJSON_CreateObjectReference(num);
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *item0 = NULL;
    int arr_size = 0;
    double val0 = 0.0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(child, "msg", "hello");
    cJSON_AddItemReferenceToArray(arr, ref);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_ParseWithLengthOpts(printed, strlen(printed), &parse_end, 1);
    retrieved_arr = cJSON_GetObjectItem(parsed, "values");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    val0 = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)val0 % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(num);

    // API sequence test completed successfully
    return 66;
}