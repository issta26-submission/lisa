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
//<ID> 458
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(13.37);
    cJSON *flag = cJSON_CreateFalse();
    char *printed = NULL;
    cJSON *item0 = NULL;
    double val0 = 0.0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)val0 % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}