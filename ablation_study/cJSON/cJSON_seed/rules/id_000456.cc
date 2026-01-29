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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *f = cJSON_CreateFalse();
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_arr = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(arr, f);
    cJSON_AddItemToObject(root, "flags", arr);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(printed);
    retr_arr = cJSON_GetObjectItem(parsed, "flags");
    arr_size = cJSON_GetArraySize(retr_arr);
    scratch[0] = (char)('0' + (arr_size % 10));
    scratch[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}