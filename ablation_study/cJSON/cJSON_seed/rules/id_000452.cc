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
//<ID> 452
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
    cJSON *f = cJSON_CreateFalse();
    char *printed = NULL;
    char *buffer = NULL;
    size_t len = 0;
    cJSON *parsed = NULL;
    cJSON *retr_arr = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(arr, f);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    len = strlen(printed) + 1;
    buffer = (char *)cJSON_malloc(len);
    memset(buffer, 0, len);
    memcpy(buffer, printed, len - 1);
    buffer[len - 1] = '\0';
    cJSON_Minify(buffer);
    parsed = cJSON_Parse(buffer);
    retr_arr = cJSON_GetObjectItem(parsed, "flags");
    arr_size = cJSON_GetArraySize(retr_arr);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}