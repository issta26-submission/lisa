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
//<ID> 1477
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str_item);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "flag", cJSON_CreateFalse());
    cJSON_SetNumberHelper(num_item, 3.14159);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_val = cJSON_GetStringValue(first);
    double val = cJSON_GetNumberValue(num_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = first_val ? first_val[0] : 'X';
    scratch[1] = (char)('0' + ((int)val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)(cJSON_IsTrue(cJSON_GetObjectItem(root, "flag")) ? 'T' : 'F');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}