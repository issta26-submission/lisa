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
//<ID> 231
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
    cJSON *str = cJSON_CreateString("example string");
    cJSON *truth = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int size = 0;
    cJSON *got = NULL;
    const char *sval = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToArray(arr, truth);
    cJSON_AddItemReferenceToArray(arr, str);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    got = cJSON_GetArrayItem(arr, 1);
    sval = cJSON_GetStringValue(got);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    (void)size;
    (void)sval;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}